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
#if CPU_LOAD_MEASUREMENT_ENABLE
extern void Cpul_startPoint(tU08);
extern void Cpul_stopPoint(void);
extern char Cpul_getMaxCycles_U08(void);
#endif

/* Declaring main as OS_main saves some register pushing to stack. */
int main(void) __attribute__((OS_main));
int main(void)
{
    /* ATTENTION! Initialization code can be found in boot.S */

    /* Check for WDT reset (save code size and assume true) - if a runaway pointer enables it,
     * then it must be disabled here because it's kept after a reset! Ref. AVR132 chap 2.4.
     */
    Cont_storeStatusRegister(MCUSR);
    MCUSR = 0;
    WDTCR = _BV(WDCE) | _BV(WDE);
    WDTCR = 0;


    /* Pwer saving - switch of analog comparator */
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

    for (;;)
    {
#if CPU_LOAD_MEASUREMENT_ENABLE
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
