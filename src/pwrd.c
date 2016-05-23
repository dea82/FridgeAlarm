/*
The MIT License (MIT)

Copyright (c) 2015-2016 Andreas L.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "pwrd.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>

#include "butt.h"
#include "wdtc.h"

static tPwrd_sleepMode_E sleepMode_E;

inline void Pwrd_wakeup(void)
{
    Wdtc_SetTimer(WDTC_16MS_E, FALSE);
}

void Pwrd_sleep(void)
{
    /* If the controls find it ok to go to sleep, door closed for a long time then set
     * the WDT to 8 seconds. Enable the button interrupt to be able to wake it up from
     * deep sleep.*/
    if (sleepMode_E == PWRD_LONG_DEEP_SLEEP_E)
    {
        Wdtc_SetTimer(WDTC_8S_E, TRUE);
        Butt_enableInterrupt();
    }
    else if (sleepMode_E == PWRD_INFINITE_SLEEP_E)
    {
        Wdtc_SetTimer(WDTC_OFF_E, FALSE);
        Butt_enableInterrupt();
    }
    else
    {
        Wdtc_SetTimer(WDTC_16MS_E, TRUE);
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
    //TODO:10 Is it ok to keep Sleep Enabled
    //MCUCR &= ~_BV(SE);
    Butt_disableInterrupt();
}

void Pwrd_setSleepMode(const tPwrd_sleepMode_E mode_E)
{
    sleepMode_E = mode_E;
}
