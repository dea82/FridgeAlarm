/*
 * main.c
 *
 *  Created on: 18 feb 2015
 *      Author: andreas
 */

#include <avr/interrupt.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <avr/wdt.h>

#include "butt.h"
#include "buzz.h"
#include "cont.h"
#include "dsen.h"
#include "ledc.h"
#include "conf.h"
#include "type.h"

typedef enum
{
    DOOR_OPEN_E, ALARM_E, DOOR_CLOSED_E, SLEEP_E
} tMainState_E;

typedef enum
{
    WDTO_16MS_E, WDTO_8S_E,
} tWatchdogTimeout_E;

typedef struct
{
    tMainState_E state_E;
    unsigned int timeInState_U16;
} tMainState_str;

static void enableWatchdog(tWatchdogTimeout_E time_E);
static void powerDown(void);


ISR(WDT_vect)
{

}

ISR(PCINT0_vect)
{
    /* Just for wake-up */
}

int main(void)
{
#if 0
    /* Find out what caused reset. */
    if (MCUSR & (1 << BORF))
    {
        lockMode_E = BOD_RESET_E;
    }
    else if (MCUSR & (1 << WDRF))
    {
        lockMode_E = WDT_RESET_E;
    }

    MCUSR = 0;
#endif
    Butt_init();
    Dsen_init();
    Buzz_init();
    Ledc_init();

#if defined(__AVR_ATtiny13A__)
    /* Reduce power consumption - turn off BOD during power down. */
    BODCR = (1 << BODS) | (1 << BODSE);
#elif defined(__AVR_ATtiny85__)
    MCUCR |= _BV(BODS) | _BV(BODSE);
#endif
    /* Reduce power consumption - switch of Analog Comparator */
    ACSR |= (1 << ACD);

    for (;;)
    {
        cli();
        enableWatchdog(WDTO_16MS_E);
        Butt_disableInterrupt();
        sei();
        /* Sensors */
        Butt_loop();
        Dsen_loop();
        /* Controls*/
        Cont_loop();
        /* Actuators */
        Ledc_loop();
        Buzz_loop();
        if (Cont_deepSleepOk_B())
        {
            enableWatchdog(WDTO_8S_E);
            Butt_enableInterrupt();
        }
        powerDown();
    }
    return 0;
}

static void enableWatchdog(tWatchdogTimeout_E time_E)
{
    wdt_reset();

    switch (time_E)
    {
    case WDTO_16MS_E:
        WDTCR = _BV(WDT_INT) | _BV(WDCE);
        break;
    case WDTO_8S_E:
        WDTCR = _BV(WDT_INT) | _BV(WDCE) | _BV(WDP3) | _BV(WDP0);
        break;
    default:
        break;
    }

}

static void powerDown(void)
{
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_mode();
}

