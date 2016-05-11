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
#if CPU_LOAD_MEASUREMENT_ENABLE
extern void Cpul_startPoint(tU08);
extern void Cpul_stopPoint(void);
extern char Cpul_getMaxCycles_U08(void);
#endif

static void enableWatchdog(const tWatchdogTimeout_E time_E);
static void powerDown(const tSleepMode_E sleepMode_E);

/* Declaring main as OS_main saves some register pushing to stack. */
int main(void) __attribute__((OS_main));
int main(void)
{
    /* ATTENTION! Initialization code can be found in boot.S */

    /* Reduce power consumption - switch of Analog Comparator */
    ACSR |= _BV(ACD);

    /* Turns off Timer0, (Timer1) and USI */
    PRR = PRR_INIT;

    /* Power reduction - turn off digital input buffers except for button */
    DIDR0 = _BV(AIN0D) | _BV(AIN1D) | _BV(ADC1D) | _BV(ADC2D) | _BV(ADC3D);

    /* Reset due to low battery or other causes that should not occur */
    if (!(MCUSR & _BV(PORF)))
    {
      CONF_IO(RED_LED_CFG, OUTPUT, 1);
      CONF_IO(GREEN_LED_CFG, OUTPUT, 1);
      for(;;){}
    }
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
        Buzz_loop();
        Ledc_loop();

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
        WDTCR = _BV(WDIF) | (_BV(WDIE) | _BV(WDCE));
        break;
    case WDTO_8S_E:
        WDTCR = _BV(WDIF) | (_BV(WDIE) | _BV(WDCE) | _BV(WDP3) | _BV(WDP0));
        break;
    default:
        break;
    }
}

static void powerDown(const tSleepMode_E sleepMode_E)
{
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
#if CPU_LOAD_MEASUREMENT_ENABLE
    Cpul_stopPoint();

    Uart_TransmitChar(Cpul_getMaxCycles_U08());
#endif
    MCUSR |= (_BV(BODS) | _BV(SE) | _BV(BODSE));
    MCUSR &= ~_BV(BODSE);
    sei();
    asm volatile("sleep"::);
    MCUSR &= ~_BV(SE);
}
