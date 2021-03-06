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

/**
 * @file
 */

#include "pwrd.h"

#include <avr/interrupt.h>
#include <avr/io.h>

#include "butt.h"
#include "wdtc.h"



static tPwrd_SleepMode_E sleepMode_E;

inline void Pwrd_Wakeup(void)
{
    /**
     * If wakeup was caused by button interrupt the watchdog timer can have any value.
     *  Make sure to reset it before activating WDT reset otherwise we will have
     *  might have a reset during next cycle.
     */
    asm volatile("wdr"::);
    Butt_DisableInterrupt();
    Wdtc_SetTimer(WDTC_16MS_E, FALSE);
}

void Pwrd_Sleep(void)
{
    /* If the controls find it ok to go to sleep, door closed for a long time then set
     * the WDT to 8 seconds. Enable the button interrupt to be able to wake it up from
     * deep sleep.*/
    if (sleepMode_E == PWRD_LONG_DEEP_SLEEP_E)
    {
        Wdtc_SetTimer(WDTC_8S_E, TRUE);
        Butt_EnableInterrupt();
    }
    else if (sleepMode_E == PWRD_INFINITE_SLEEP_E)
    {
        Wdtc_SetTimer(WDTC_OFF_E, FALSE);
        Butt_EnableInterrupt();
    }
    else
    {
        Wdtc_SetTimer(WDTC_16MS_E, TRUE);
    }
    /* Determine if timer shall be running in sleep */
    if (sleepMode_E == PWRD_SLEEP_WITH_TIMER_RUNNING_E)
    {
        /* Sleep mode idle */
        MCUCR = 0;
    }
    else
    {
        /* Sleep mode power-down */
        MCUCR = _BV(SM1);
    }

    cli(); /* Timed sequence make sure it's not disturbed */
#if defined(__AVR_ATtiny85__)
    MCUCR |= (_BV(BODS) | _BV(SE) | _BV(BODSE));
    MCUCR &= ~_BV(BODSE);
#elif defined(__AVR_ATtiny13A__)
    MCUCR |= _BV(SE);
    BODCR = _BV(BODS) | _BV(BODSE);
    BODCR = _BV(BODS);
#else
#error "Sleep module does not support selected MCU."
#endif
    sei(); /* To be able to wake-up */
    asm volatile("sleep"::);
}

void Pwrd_SetSleepMode(const tPwrd_SleepMode_E mode_E)
{
    sleepMode_E = mode_E;
}
