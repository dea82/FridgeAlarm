/*
 * main.c
 *
 *  Created on: 18 feb 2015
 *      Author: andreas
 */

/* TODO:0 Cleanup! */
/*** Standard library ***/
//#include <util/delay.h>

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

tU08 resetRegister_U08;

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
    Buzz_init();
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
        Buzz_loop();
        Ledc_loop();

        /* Powerdown */
        Pwrd_sleep();
    }

    return 0;
}
