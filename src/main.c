/*
 * main.c
 *
 *  Created on: 18 feb 2015
 *      Author: andreas
 */

/* TODO: Cleanup! */
/*** Standard library ***/
#include <util/delay.h>

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

    /* Power reduction - turn off digital input buffers except for button */
    DIDR0 = _BV(AIN0D) | _BV(AIN1D) | _BV(ADC1D) | _BV(ADC2D) | _BV(ADC3D);

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

        /* Interrupt is always off here. WDT and PC_INT routines take care of that. */
        //enableWatchdog(WDTO_16MS_E);
        /* Make sure to disable button interrupt before continue. The MCU is awake. */
        //Butt_disableInterrupt();
        /* Ready to fire off interrupt again, but it should not happen until it's at sleep. */

        /* Sensors */
        Butt_loop();
        Dsen_loop();

        /* Controls*/
        Cont_loop();

        /* Actuators */
        Buzz_loop();
        Ledc_loop();
        Pwrd_loop();
    }

    return 0;
}
