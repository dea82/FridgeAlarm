typedef enum
{
    PWRD_SHORT_DEEP_SLEEP_E,
    PWRD_LONG_DEEP_SLEEP_E,
    PWRD_SLEEP_WITH_TIMER_RUNNING_E,
    PWRD_INFINITE_SLEEP_E
} tPwrd_sleepMode_E;

void Pwrd_sleep(void);

void Pwrd_wakeup(void);

void Pwrd_setSleepMode(const tPwrd_sleepMode_E mode_E);
