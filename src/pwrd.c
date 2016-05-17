#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>

#include "butt.h"
#include "pwrd.h"
#include "wdtc.h"

static tPwrd_sleepMode_E sleepMode_E;

void Prwd_init(void)
{

}

void Pwrd_loop(void)
{
    /* If the controls find it ok to go to sleep, door closed for a long time then set
     * the WDT to 8 seconds. Enable the button interrupt to be able to wake it up from
     * deep sleep.*/
    if (sleepMode_E == PWRD_LONG_DEEP_SLEEP_E)
    {
        Wdtc_SetWatchdog(WDTC_8S_E);
        Butt_enableInterrupt();
    }
    else if (sleepMode_E == PWRD_INFINITE_SLEEP_E)
    {
        Wdtc_SetWatchdog(WDTC_OFF_E);
        Butt_enableInterrupt();

    }
    /* Determine if timer shall be running in sleep */
    if (sleepMode_E == PWRD_SLEEP_WITH_TIMER_RUNNING_E)
    {
        set_sleep_mode(SLEEP_MODE_IDLE);
    }
    else
    {
        set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    }

#if CPU_LOAD_MEASUREMENT_ENABLE
    Cpul_stopPoint();

    Uart_TransmitChar(Cpul_getMaxCycles_U08());
#endif
    cli(); /* Timed sequence make sure it's not disturbed */
    MCUCR |= (_BV(BODS) | _BV(SE) | _BV(BODSE));
    MCUCR &= ~_BV(BODSE);
    sei(); /* To be able to wake-up */
    asm volatile("sleep"::);
    MCUCR &= ~_BV(SE);
    Wdtc_SetWatchdog(WDTC_16MS_E);
    Butt_disableInterrupt();
}

void Pwrd_setSleepMode(const tPwrd_sleepMode_E mode_E)
{
    sleepMode_E = mode_E;
}
