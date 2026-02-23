*** Settings ***
Library           Process
Library           OperatingSystem
Suite Setup       Compile Project

*** Variables ***
${PROJECT_ROOT}   ${CURDIR}${/}..${/}..${/}
${EXEC_PATH}      ${PROJECT_ROOT}philo
${TIMEOUT}        30s

*** Keywords ***
Compile Project
    [Documentation]    Compile le projet avant de lancer les tests
    Run Process    make -C ${PROJECT_ROOT}    shell=True    timeout=60s
    Should Exist   ${EXEC_PATH}

Run Philo
    [Arguments]    @{args}
    ${result}=    Run Process    ${EXEC_PATH}    @{args}    timeout=${TIMEOUT}    shell=False    stdout=stdout.txt    stderr=stderr.txt
    ${stdout}=    Get File    stdout.txt
    ${stderr}=    Get File    stderr.txt
    RETURN    ${result}    ${stdout}    ${stderr}

Run Philo Raw
    [Documentation]    Run philo without argument unpacking (for wrong arg count tests)
    [Arguments]    @{full_cmd}
    ${result}=    Run Process    @{full_cmd}    timeout=${TIMEOUT}    shell=False    stdout=stdout.txt    stderr=stderr.txt
    RETURN    ${result}

*** Test Cases ***
# =========================
# Tests de base
# =========================
1 Philosopher Dies
    [Tags]    basic
    ${result}    ${stdout}    ${stderr}=    Run Philo    1    800    200    200
    Should Contain    ${stdout}    died

Simulation Stable With Meals
    [Tags]    basic
    ${result}    ${stdout}    ${stderr}=    Run Philo    5    800    200    200    3
    Should Be Equal As Integers    ${result.rc}    0
    Should Not Contain    ${stdout}    died

2 Philosophers No Death
    [Tags]    basic
    ${result}    ${stdout}    ${stderr}=    Run Philo    2    800    200    200    5
    Should Be Equal As Integers    ${result.rc}    0
    Should Not Contain    ${stdout}    died

Many Philosophers
    [Tags]    basic
    ${result}    ${stdout}    ${stderr}=    Run Philo    200    800    200    200    3
    Should Be Equal As Integers    ${result.rc}    0
    Should Not Contain    ${stdout}    died

Philosopher Dies Tight Timing
    [Tags]    basic
    ${result}    ${stdout}    ${stderr}=    Run Philo    4    310    200    100
    Should Contain    ${stdout}    died

# =========================
# Tests parsing / arguments
# =========================
Non Digit Argument
    [Tags]    parsing
    ${result}    ${stdout}    ${stderr}=    Run Philo    5    abc    200    200
    Should Not Be Equal As Integers    ${result.rc}    0

Zero Philosophers
    [Tags]    parsing
    ${result}    ${stdout}    ${stderr}=    Run Philo    0    800    200    200
    Should Not Be Equal As Integers    ${result.rc}    0

Negative Time
    [Tags]    parsing
    ${result}    ${stdout}    ${stderr}=    Run Philo    5    -800    200    200
    Should Not Be Equal As Integers    ${result.rc}    0

Too Many Philosophers
    [Tags]    parsing
    ${result}    ${stdout}    ${stderr}=    Run Philo    1001    800    200    200
    Should Not Be Equal As Integers    ${result.rc}    0
    Should Contain    ${stdout}    Too many

Empty Argument
    [Tags]    parsing
    ${result}    ${stdout}    ${stderr}=    Run Philo    5    ${EMPTY}    200    200
    Should Not Be Equal As Integers    ${result.rc}    0

Wrong Arg Count
    [Tags]    parsing
    ${result}=    Run Philo Raw    ${EXEC_PATH}    5    800
    Should Not Be Equal As Integers    ${result.rc}    0

Zero Time To Eat
    [Tags]    parsing
    ${result}    ${stdout}    ${stderr}=    Run Philo    5    800    0    200
    Should Not Be Equal As Integers    ${result.rc}    0

Zero Meals
    [Tags]    parsing
    ${result}    ${stdout}    ${stderr}=    Run Philo    5    800    200    200    0
    Should Not Be Equal As Integers    ${result.rc}    0

# =========================
# Tests timing / performance
# =========================
Death Timestamp Within 10ms
    [Tags]    timing
    ${result}    ${stdout}    ${stderr}=    Run Philo    1    200    200    200
    ${lines}=    Evaluate    $stdout.strip().split("\\n")
    ${death}=    Evaluate    [l for l in $lines if 'died' in l][0]
    ${timestamp}=    Evaluate    int($death.split()[0])
    Should Be True    ${timestamp} <= 210

No Message After Death
    [Tags]    timing
    ${result}    ${stdout}    ${stderr}=    Run Philo    4    200    200    200
    ${lines}=    Evaluate    $stdout.strip().split("\\n")
    ${death_idx}=    Evaluate    next((i for i, l in enumerate($lines) if 'died' in l), None)
    IF    $death_idx is not None
        ${last_idx}=    Evaluate    len($lines) - 1
        Should Be Equal As Integers    ${death_idx}    ${last_idx}
    END

Execution Time Reasonable
    [Tags]    timing
    ${start}=    Evaluate    time.time()    modules=time
    ${result}    ${stdout}    ${stderr}=    Run Philo    5    800    200    200    3
    ${elapsed}=    Evaluate    time.time() - ${start}    modules=time
    Should Be Equal As Integers    ${result.rc}    0
    Should Be True    ${elapsed} < 10

