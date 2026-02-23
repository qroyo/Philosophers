import subprocess
import pytest
import os
import time

# Chemin vers le binaire, relatif à la racine du projet
PROJECT_ROOT = os.path.join(os.path.dirname(__file__), "..", "..")
EXEC_PATH = os.path.join(PROJECT_ROOT, "philo")
TIMEOUT = 30  # secondes max par test


@pytest.fixture(scope="session", autouse=True)
def compile_philo():
    """Compile le projet avant de lancer les tests."""
    print("\nCompilation du projet...")
    result = subprocess.run(["make", "-C", PROJECT_ROOT], capture_output=True, text=True)
    assert result.returncode == 0, f"Compilation échouée:\n{result.stderr}"
    assert os.path.exists(EXEC_PATH), "Le binaire philo n'a pas été généré !"


def run_philo(*args, timeout=TIMEOUT):
    """Helper : lance ./philo avec les arguments donnés et un timeout."""
    cmd = [EXEC_PATH] + [str(a) for a in args]
    return subprocess.run(cmd, capture_output=True, text=True, timeout=timeout)


# ============================================================
# Tests de base : comportement attendu
# ============================================================

class TestBasicBehavior:
    def test_one_philosopher_dies(self):
        """1 philo ne peut pas manger (1 seule fork) → doit mourir."""
        proc = run_philo(1, 800, 200, 200)
        assert "died" in proc.stdout, "Le philosophe unique aurait dû mourir"

    def test_simulation_stable_with_meals(self):
        """5 philos, paramètres confortables, 3 repas → terminaison sans mort."""
        proc = run_philo(5, 800, 200, 200, 3)
        assert proc.returncode == 0, f"Crash ou erreur (rc={proc.returncode})"
        assert "died" not in proc.stdout, "Un philosophe est mort inopinément"

    def test_two_philosophers_no_death(self):
        """2 philos avec timing confortable → pas de mort."""
        proc = run_philo(2, 800, 200, 200, 5)
        assert proc.returncode == 0
        assert "died" not in proc.stdout

    def test_many_philosophers(self):
        """200 philos, 3 repas → doit terminer sans mort."""
        proc = run_philo(200, 800, 200, 200, 3, timeout=60)
        assert proc.returncode == 0
        assert "died" not in proc.stdout

    def test_philosopher_dies_tight_timing(self):
        """Timing très serré : time_to_die juste au-dessus de eat+sleep."""
        proc = run_philo(4, 310, 200, 100, timeout=10)
        # Avec 4 philos et 310ms to die, 200 eat, 100 sleep → mort probable
        assert "died" in proc.stdout


# ============================================================
# Tests de parsing / arguments invalides
# ============================================================

class TestParsing:
    def test_non_digit_argument(self):
        """Lettres au lieu de chiffres → erreur."""
        proc = run_philo(5, "abc", 200, 200)
        assert proc.returncode != 0

    def test_zero_philosophers(self):
        """0 philosophe → erreur."""
        proc = run_philo(0, 800, 200, 200)
        assert proc.returncode != 0

    def test_negative_time(self):
        """Temps négatif → erreur (rejeté par ft_check_digit)."""
        proc = run_philo(5, "-800", 200, 200)
        assert proc.returncode != 0

    def test_too_many_philosophers(self):
        """Plus de MAX_PHILOSOPHERS → erreur."""
        proc = run_philo(1001, 800, 200, 200)
        assert proc.returncode != 0
        assert "Too many" in proc.stdout

    def test_empty_argument(self):
        """Argument vide → erreur."""
        proc = run_philo(5, "", 200, 200)
        assert proc.returncode != 0

    def test_wrong_arg_count(self):
        """Trop peu d'arguments → erreur."""
        proc = subprocess.run([EXEC_PATH, "5", "800"], capture_output=True, text=True, timeout=TIMEOUT)
        assert proc.returncode != 0

    def test_zero_time_to_eat(self):
        """time_to_eat = 0 → erreur."""
        proc = run_philo(5, 800, 0, 200)
        assert proc.returncode != 0

    def test_zero_meals(self):
        """nb_meal = 0 → erreur."""
        proc = run_philo(5, 800, 200, 200, 0)
        assert proc.returncode != 0


# ============================================================
# Tests de timing / performance
# ============================================================

class TestTiming:
    def test_death_timestamp_within_10ms(self):
        """Le message de mort doit arriver dans les 10ms après time_to_die."""
        proc = run_philo(1, 200, 200, 200)
        lines = proc.stdout.strip().split("\n")
        death_line = [l for l in lines if "died" in l]
        assert len(death_line) == 1, f"Attendu 1 mort, trouvé: {death_line}"
        timestamp = int(death_line[0].split()[0])
        assert timestamp <= 210, f"Mort à {timestamp}ms, attendu <= 210ms"

    def test_no_message_after_death(self):
        """Aucun message ne doit être imprimé après 'died'."""
        proc = run_philo(4, 200, 200, 200)
        lines = proc.stdout.strip().split("\n")
        death_idx = None
        for i, line in enumerate(lines):
            if "died" in line:
                death_idx = i
                break
        if death_idx is not None:
            remaining = lines[death_idx + 1:]
            assert len(remaining) == 0, f"Messages après la mort: {remaining}"

    def test_execution_time_reasonable(self):
        """5 philos, 3 repas → doit finir en temps raisonnable."""
        start = time.time()
        proc = run_philo(5, 800, 200, 200, 3)
        elapsed = time.time() - start
        assert proc.returncode == 0
        assert elapsed < 10, f"Trop lent: {elapsed:.1f}s"
