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

/*** User library ***/
#include "butt.h"
#include "buzz.h"
#include "conf.h"
#include "cont.h"
#include "door.h"
#include "ledc.h"
#include "pwrd.h"
#include "type.h"
#if CPU_LOAD
#include <avr/pgmspace.h>
#include "wcet.h"
#endif

#if CPU_LOAD
/* Macro for putting string in flash if cpu load measurement is configured. */
#define PSTR_CPU_LOAD(STRING) PSTR(STRING)
#define UNUSED_CPU_LOAD
#else
/* Macro which enables build without including avr/pgmspace.h */
#define PSTR_CPU_LOAD(STRING) STRING
/* TODO: Find a better name and document! */
#define UNUSED_CPU_LOAD __attribute__ ((unused))
#endif

static void addTask(UNUSED_CPU_LOAD const char *taskName_c,
                    void (* const task_fptr)(void),
                    UNUSED_CPU_LOAD const tU08 prescaler_U08);

/**
 * [addTask description]
 * @param taskName_c [description]
 * @param task_fptr  [description]
 */
static void addTask(UNUSED_CPU_LOAD const char *taskName_c,
                    void (* const task_fptr)(void),
                    UNUSED_CPU_LOAD const tU08 prescaler_U08)
{
#if CPU_LOAD
    Wcet_StartMeasurement(prescaler_U08);
#endif
    task_fptr();
#if CPU_LOAD
    Wcet_StopMeasurement();
    Wcet_OutputResult(taskName_c, prescaler_U08);
#endif
}

/* Declaring main as OS_main saves some register pushing to stack. */
int main(void) __attribute__((OS_main));
int main(void)
{
    /* ATTENTION! Initialization code can be found in boot.S */

    /* Check for WDT reset (save code size and assume true) - if a runaway pointer enables it,
     * then it must be disabled here because it's kept after a reset! Ref. AVR132 chap 2.4.
     * Reset status register because it will cause a new WDT reset if WDRF is set. Make sure
     * to save it for later useage to detect if abnormal reset has occured.
     */
    {
        tU08 statusRegister_U08 = MCUSR;
        MCUSR = 0;
        WDTCR = _BV(WDCE) | _BV(WDE);
        WDTCR = 0;

        /* Power saving - switch of analog comparator */
        ACSR |= _BV(ACD);

        /* Initialize I/O Ports */
        DDRB = DDRB_INIT;
        PORTB |= PORTB_INIT; /* Optimization, only one bit is changed */

        /* Initialize sensors */
        Butt_Init();
        Door_Init();
        /* Initialize controls */
        Cont_Init();
        /* Initialize actuators */
        Buzz_Init();
        Ledc_Init();

        /* Enable global interrupt otherwise door will not be able to wake up
         * from ADC Noise reduction mode or infinite sleep mode. */
        asm volatile("sei"::);

        /**
         * Abnormal reset detected - goto infinite sleep with orange led
         * indicating that we have a system error. Put the MCU in "infinite"
         * sleep to save power, it could be BOD reset due to low batteries.
         * The MCU is however possible to wakeup with the button - then it
         * will go back to normal function.
         */
        if(!(statusRegister_U08 & _BV(PORF)))
        {
            Butt_EnableInterrupt();
            Ledc_SetOrange();
            Pwrd_SetSleepMode(PWRD_INFINITE_SLEEP_E);
            Pwrd_Sleep();
        }
    }

    for (;;)
    {

        /* Startup */
        Pwrd_Wakeup();

        /* Sensors */
        addTask(PSTR_CPU_LOAD("BUTT"), Butt_Loop, 1);
        addTask(PSTR_CPU_LOAD("DOOR"), Door_Loop, 1);

        /* Controls */
        addTask(PSTR_CPU_LOAD("CONT"), Cont_Loop, 1);

        /* Actuators */
        addTask(PSTR_CPU_LOAD("BUZZ"), Buzz_Loop, 1);
        addTask(PSTR_CPU_LOAD("LEDC"), Ledc_Loop, 1);

        /* Powerdown */
        Pwrd_Sleep();
    }

    return 0;
}
