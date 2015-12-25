/*
 * main.c
 *
 *  Created on: 18 feb 2015
 *      Author: andreas
 */

/*** Standard library ***/
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <util/delay.h>

/*** User library ***/
#include "butt.h"
#include "buzz.h"
#include "conf.h"
#include "cont.h"
#include "dsen.h"
#include "ledc.h"
#include "type.h"
#include "uart.h"

typedef enum
{
    WDTO_16MS_E, WDTO_8S_E,
} tWatchdogTimeout_E;

/* Assembly function. */
#if MEASURE_CPU_LOAD
extern void startTimer(void);
extern unsigned char stopTimer(void);
#endif

static void enableWatchdog(const tWatchdogTimeout_E time_E);
static void powerDown(tSleepMode_E sleepMode_E);

ISR(WDT_vect, ISR_NAKED)
{
    /* Just for wake-up, turn-off interrupt and reinitialize them again in main loop.
     * Interrupt flag is cleared by MCU when entering the interrupt routine. */
    asm volatile("ret");
}

ISR(PCINT0_vect, ISR_ALIASOF(WDT_vect));

int main(void)
{
    /* Check for WDT reset (save code size and assume true) - if a runaway pointer enables it,
     * then it must be disabled here because it's kept after a reset! Ref. AVR132 chap 2.4. */
    WDTCR = 0;

    /* Reduce power consumption - switch of Analog Comparator */
    ACSR |= _BV(ACD);

    /* Power reduction */
#if MEASURE_CPU_LOAD
    PRR |= _BV(PRUSI) | _BV(PRTIM0);
#else
    PRR |= _BV(PRUSI) | _BV(PRTIM0) | _BV(PRTIM1); //TODO: Does not work _BV(PRADC), why?
#endif
    DIDR0 |= _BV(AIN0D) | _BV(AIN1D) | _BV(ADC1D) | _BV(ADC2D); //TODO: Add ADC3D

    DDRB = 0b00010111;
    PORTB = 0b00100000;

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
#if MEASURE_CPU_LOAD
        startTimer();
#endif
        /* Interrupt is always off here. WDT and PC_INT routines take care of that. */
        enableWatchdog(WDTO_16MS_E);
        /* Make sure to disable button interrupt before continue. The MCU is awake. */
        Butt_disableInterrupt();
        /* Ready to fire off interrupt again, but it should not happen until it's at sleep. */
        sei();

        /* Sensors */
        Butt_loop();
        Dsen_loop();

        /* Controls*/
        Cont_loop();

        /* Actuators */
        Ledc_loop();
        Buzz_loop();

        powerDown(Cont_sleepMode_E());
    }

    return 0;
}

static void enableWatchdog(const tWatchdogTimeout_E time_E)
{
    wdt_reset();

    switch (time_E)
    {
    case WDTO_16MS_E:
        WDTCR = _BV(WDIF_C) | (_BV(WDT_INT) | _BV(WDCE));
        break;
    case WDTO_8S_E:
        WDTCR = _BV(WDIF_C)
                | (_BV(WDT_INT) | _BV(WDCE) | _BV(WDP3) | _BV(WDP0));
        break;
    default:
        break;
    }
}

static void powerDown(tSleepMode_E sleepMode_E)
{
#if MEASURE_CPU_LOAD
    static unsigned char maxCycles_U08;
#endif
    /* If the controls find it ok to go to sleep, door closed for a long time then set
     * the WDT to 8 seconds. Enable the button interrupt to be able to wake it up from
     * deep sleep.*/
    if (sleepMode_E == CONT_LONG_DEEP_SLEEP_E)
    {
        enableWatchdog(WDTO_8S_E);
        Butt_enableInterrupt();
    }

    if (sleepMode_E == CONT_SLEEP_WITH_TIMER_RUNNING_E)
    {
        set_sleep_mode(SLEEP_MODE_IDLE);
    }
    else
    {
        set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    }
#if MEASURE_CPU_LOAD
    maxCycles_U08 = (stopTimer() > maxCycles_U08) ?  stopTimer() : maxCycles_U08;
    Uart_TransmitChar(maxCycles_U08);
#endif
    sleep_mode()
    ;
}

