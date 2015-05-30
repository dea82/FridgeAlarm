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
    WDTO_16MS_E,
    WDTO_32MS_E,
    WDTO_64MS_E,
    WDTO_125MS_E,
    WDTO_250MS_E,
    WDTO_500MS_E,
    WDTO_1S_E,
    WDTO_2S_E,
    WDTO_4S_E,
    WDTO_8S_E
} tWatchdogTimeout_E;

typedef struct
{
    tMainState_E state_E;
    unsigned int timeInState_U16;
} tMainState_str;

static void enableWatchdogInt(tWatchdogTimeout_E time_E);
static void powerDown(void);

ISR(WDT_vect)
{
//    CONF_IO(GREEN_LED_CFG, OUTPUT, 0);
//    CONF_IO(RED_LED_CFG, OUTPUT, 1);
}

ISR(PCINT0_vect)
{

}

int main(void)
{
    tB lockMode_B = FALSE;

    if (MCUSR & ((1 << WDRF) | (1 << BORF)))
    {
        lockMode_B = TRUE;
    }

    Butt_init();
    Dsen_init();
    Buzz_init();
    Ledc_init();

    /* Important to disable the WDT reset even if not used due to flipping pointers. */
    WDTCR |= (1 << WDTIE) | (1 << WDCE);
    WDTCR |= (1 << WDP1) | (1 << WDP2);
    sei();

    for (;;)
    {
        CONF_IO(GREEN_LED_CFG, OUTPUT, 1);
        CONF_IO(RED_LED_CFG, OUTPUT, 0);
        wdt_reset();
        powerDown();
        CONF_IO(GREEN_LED_CFG, OUTPUT, 0);
        CONF_IO(RED_LED_CFG, OUTPUT, 1);
        _delay_ms(1000);
    }
    return 0;
}

static void enableWatchdogInt(tWatchdogTimeout_E time_E)
{

}

static void powerDown(void)
{
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_mode();
}

