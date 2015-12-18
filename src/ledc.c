/*
 * ledc.c
 *
 *  Created on: 19 feb 2015
 *      Author: andreas
 */

#include <avr/io.h>


#include "conf.h"
#include "type.h"
#include "ledc.h"

#define BLINK_PERIOD_TIME 120 /* ms */

static tLedState_E ledState_E = LEDC_OFF_E;

inline void Ledc_init(void)
{
}

inline void Ledc_loop(void)
{
    static tU08 counter_U08;
    if (counter_U08++ > BLINK_PERIOD_TIME / TICK)
    {
        counter_U08 = 0;
    }
    switch (ledState_E)
    {
    case LEDC_OFF_E:
        IO_CLR(GREEN_LED_CFG);
        IO_CLR(RED_LED_CFG);
        break;
  
    case LEDC_GREEN_E:
        IO_SET(GREEN_LED_CFG);
        IO_CLR(RED_LED_CFG);
        break;

    case LEDC_RED_E:
        IO_CLR(GREEN_LED_CFG);
        IO_SET(RED_LED_CFG);
        break;
 
    case LEDC_GREEN_BLINK_E:
        if (counter_U08 > BLINK_PERIOD_TIME / TICK / 2)
        {
            IO_SET(GREEN_LED_CFG);
        }
        else
        {
            IO_CLR(GREEN_LED_CFG);
        }
        IO_CLR(RED_LED_CFG);
        break;

    case LEDC_RED_BLINK_E:
        if (counter_U08 > BLINK_PERIOD_TIME / TICK / 2)
        {
            IO_SET(RED_LED_CFG);
        }
        else
        {
            IO_CLR(RED_LED_CFG);
        }
        IO_CLR(GREEN_LED_CFG);
        break;

    default:
        IO_CLR(GREEN_LED_CFG);
        IO_CLR(RED_LED_CFG);
    }
}

void Ledc_setState(const tLedState_E state_E)
{
    ledState_E = state_E;
}
