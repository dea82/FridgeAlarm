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
#include "cpul.h"
#include "uart.h"
#define ADD_TASK(TASK_NAME, TASK, PRESCALER)                                   \
({                                                                             \
    if(CPU_LOAD)                                                               \
    {                                                                          \
        Cpul_StartPoint(PRESCALER);                                            \
    }                                                                          \
    TASK();                                                                    \
    if(CPU_LOAD)                                                               \
    {                                                                          \
        tU08 cycles_U08 = Cpul_StopPoint_U08();	                               \
        tCpul_ResultBlock_str resultBlock_str =                                \
            Cpul_CreateResultBlock_str(PSTR(TASK_NAME), cycles_U08, PRESCALER);\
        Uart_TransmitBlock((tU08*)&resultBlock_str, sizeof(resultBlock_str));  \
    }                                                                          \
})
#else
#define ADD_TASK(TASK_NAME, TASK, PRESCALER)                                   \
({                                                                             \
    TASK();                                                                    \
})
#endif



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
         * from ADC Noise reduction mode. */
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
        ADD_TASK("BUTT", Butt_Loop, 1);
        ADD_TASK("DOOR", Door_Loop, 1);

        /* Controls */
        ADD_TASK("CONT", Cont_Loop, 1);

        /* Actuators */
        ADD_TASK("BUZZ", Buzz_Loop, 1);
        ADD_TASK("LEDC", Ledc_Loop, 1);

        /* Powerdown */
        Pwrd_Sleep();
    }

    return 0;
}
