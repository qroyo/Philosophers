# 🍝 Philosophers

> Concurrent thread synchronization — the Dining Philosophers problem in C with POSIX threads.

A multithreaded simulation of the classic [Dining Philosophers Problem](https://en.wikipedia.org/wiki/Dining_philosophers_problem), written in pure C using `pthreads`. Each philosopher is a thread, each fork is a shared resource protected by a mutex. The simulation must run without deadlocks, data races, or starvation — and detect death within 10 ms.

This project was built at [42 Lyon](https://42lyon.fr) and demonstrates real-time concurrency concepts directly applicable to **RTOS** and **embedded systems** development.

---

## 📐 Architecture

```
┌──────────┐     ┌──────────┐     ┌──────────┐
│ Philo 1  │     │ Philo 2  │     │ Philo N  │    N threads (workers)
│  (even)  │     │  (odd)   │ ... │          │
└────┬─────┘     └────┬─────┘     └────┬─────┘
     │                │                │
     ▼                ▼                ▼
┌─────────────────────────────────────────────┐
│           Shared Resources (t_entry)        │
│  fork[0..N-1]  — one mutex per fork         │
│  last_eat[0..N-1] — last meal timestamp     │
│  alive / stop  — simulation state flags     │
└─────────────────────┬───────────────────────┘
                      │
                      ▼
               ┌──────────────┐
               │   Watchdog   │    1 thread (monitor)
               │ check_philo  │
               └──────────────┘
```

| Component | Role | RTOS Equivalent |
|-----------|------|-----------------|
| Philosopher thread | Periodic task (eat → sleep → think) | Task / Thread |
| Fork mutex | Shared resource lock | Mutex / Semaphore |
| Watchdog thread | Deadline monitor | Watchdog timer |
| `is_alive` mutex | State flag protection | Critical section |
| `wrt` mutex | Serialized console output | UART / Log mutex |
| `start` mutex barrier | Synchronized launch | Barrier / Event group |
| `ft_usleep` busy-wait | Precision timing with early exit | Software timer with tick polling |

---

## 🔒 Concurrency Strategies

### Deadlock Prevention — Asymmetric Fork Ordering

Philosophers are split into two groups to break the circular wait condition:

```
Even ID (0, 2, 4…) → picks LEFT fork first, then RIGHT
Odd  ID (1, 3, 5…) → picks RIGHT fork first, then LEFT
```

If the second fork is unavailable, the first is **released immediately** (no hold-and-wait).

### Starvation Prevention — Forced Think Delay

After eating and sleeping, every philosopher **must think** before retrying.  
With an odd number of philosophers, an additional delay of `time_to_eat / 2` is added during the think phase — this ensures no philosopher is consistently starved by its neighbors.

### Data Race Protection — Lock Ordering

All mutex acquisitions follow a **strict order** to prevent deadlocks:

```
is_alive → wrt       (state check then print)
Unlock in reverse:    wrt → is_alive
```

### Death Detection — Watchdog Thread

A dedicated watchdog thread cycles through all philosophers, comparing `gettimeofday()` against each philosopher's `last_eat` timestamp. Detection precision is within **10 ms** of `time_to_die`. Once death is detected, the `alive` flag is set to `0` and **no further messages are printed**.

---

## 🚀 Build & Run

### Prerequisites

- `cc` (gcc/clang)
- `make`
- POSIX-compliant OS (Linux / macOS)

### Compile

```bash
make        # build
make re     # clean rebuild
make clean  # remove objects
make fclean # remove objects + binary
```

### Run

```bash
./philo <nb_philos> <time_to_die> <time_to_eat> <time_to_sleep> [nb_meals]
```

| Argument | Description | Unit |
|----------|-------------|------|
| `nb_philos` | Number of philosophers (1–1000) | — |
| `time_to_die` | Max time without eating before death | ms |
| `time_to_eat` | Duration of a meal | ms |
| `time_to_sleep` | Duration of sleep | ms |
| `nb_meals` | *(optional)* Stop after each philo ate this many times | — |

### Examples

```bash
./philo 5 800 200 200        # 5 philos, runs until one dies
./philo 5 800 200 200 3      # 5 philos, stops after 3 meals each
./philo 4 410 200 200        # 4 philos, stable (no death)
./philo 1 800 200 200        # 1 philo, dies (only 1 fork)
```

### Output Format

```
<timestamp_ms> <philo_id> <action>
```

```
0 1 has taken a fork
0 1 has taken a fork
0 1 is eating
200 1 is sleeping
400 1 is thinking
```

---

## 🧪 Tests

The project includes two test suites: **pytest** (Python) and **Robot Framework**.

### Run All Tests

```bash
make test
```

### Run Individually

```bash
make test-pytest    # pytest only
make test-robot     # Robot Framework only
make test-clean     # remove test artifacts
```

### Test Coverage

| Category | Tests | Description |
|----------|-------|-------------|
| **Basic behavior** | 5 | 1-philo death, stable simulation, many philos, tight timing |
| **Parsing** | 8 | Invalid args, zero values, bounds, arg count |
| **Timing** | 3 | Death within 10ms, no message after death, execution time |

### Requirements

```bash
pip install pytest robotframework
```

---

## 📁 Project Structure

```
.
├── Makefile
├── README.md
├── includes/
│   └── philo.h              # Types, constants, prototypes
├── srcs/
│   ├── main.c               # Entry point, thread creation, cleanup
│   ├── parsing.c             # Argument validation
│   ├── init.c                # Init orchestration, philo ID assignment
│   ├── init_mutex.c          # Mutex init/destroy
│   ├── init_destroy_fork.c   # Fork mutex init/destroy
│   ├── init_free_malloc.c    # Memory allocation/free
│   ├── routine.c             # Main philosopher loop, fork acquisition
│   ├── eat_sleep_think.c     # State transitions, protected prints
│   ├── threads.c             # Watchdog, timing utilities
│   └── ft_usleep.c           # Precision sleep with early exit
└── test/
    ├── pytest/
    │   └── test_philo.py     # Python test suite
    └── robot/
        └── philo_tests.robot # Robot Framework test suite
```

---

## 🧠 Key Design Decisions

| Decision | Rationale |
|----------|-----------|
| One mutex per fork | Fine-grained locking — reduces contention vs. a global lock |
| Separate `alive` / `stop` flags | Distinguishes "philo died" from "all fed" — cleaner state machine |
| Asymmetric fork ordering | Breaks circular wait without a global arbitrator |
| Busy-wait `ft_usleep` with 500µs granularity | Balances CPU usage and timing precision (similar to RTOS tick) |
| Watchdog as a dedicated thread | Decouples monitoring from worker logic — mirrors hardware watchdog pattern |
| Strict lock/unlock ordering | Prevents nested deadlocks across all code paths |

---

## 📊 RTOS Concepts Demonstrated

- **Mutual exclusion** — POSIX mutexes protecting shared state
- **Deadlock prevention** — Resource ordering (Coffman condition breaking)
- **Starvation avoidance** — Fair scheduling via think delays
- **Real-time deadline monitoring** — Watchdog thread with <10ms detection
- **Deterministic timing** — `gettimeofday` + busy-wait polling loop
- **Synchronized startup** — Mutex barrier for simultaneous thread launch
- **Graceful shutdown** — Flag-based cooperative termination (no `pthread_cancel`)
- **Resource cleanup** — All mutexes destroyed, all memory freed on exit

---

## 📝 License

This project was developed as part of the 42 curriculum.

---

*Built by [qroyo](https://github.com/qroyo) — 42 Lyon*
