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
#include "dsen.h"
#include "ledc.h"
#include "pwrd.h"
#include "type.h"
#include "uart.h"

/* Assembly function. */
#if CPU_LOAD
#include "cpul_asm.h"
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
    Butt_init();
    Dsen_init();
    /* Initialize controls */
    Cont_init();
    /* Initialize actuators */
#if !UART_ENABLE    
    Buzz_init();
#endif
    Ledc_init();

#if UART_ENABLE
    Uart_Enable();
#endif

    /* Enable global interrupt otherwise dsen will not be able to wake up
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
        Butt_enableInterrupt();
        Ledc_setOrange();
        Pwrd_setSleepMode(PWRD_INFINITE_SLEEP_E);
        Pwrd_sleep();
    }

    for (;;)
    {
#if CPU_LOAD
        Cpul_startPoint(_BV(CS12) | _BV(CS11) | _BV(CS10));
#endif
        /* Startup */
        Pwrd_wakeup();

        /* Sensors */
        Butt_loop();
        Dsen_loop();

        /* Controls*/
        Cont_loop();

        /* Actuators */
#if !UART_ENABLE        
        Buzz_loop();
#endif
        Ledc_loop();

        /* Powerdown */
        Pwrd_sleep();
    }

    return 0;
}
