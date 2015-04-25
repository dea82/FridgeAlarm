/*
 * ledc.c
 *
 *  Created on: 19 feb 2015
 *      Author: andreas
 */

#include <avr/io.h>
#include <avr/pgmspace.h>

#include "conf.h"
#include "type.h"
#include "ledc.h"

#define LED_PERIOD_TIME 2 /* [100ms] */

typedef struct
{
    tU08 ledOn_U08;
    tU08 ledOff_U08;
} tLedChararteristics;

typedef struct
{
    tLedState_E ledState_E;
    tU08 ticksOnOrOff_U08;
} tLedState_str;

static tLedState_str ledState_astr[LED_TYPE_SIZE];


const PROGMEM const tLedChararteristics ledCharacteristics[LED_STATE_SIZE] =
{
{ 0, 1 },
{ 1, 0 },
{ 5, 5 } };

static void setLedOn(tLedType_E led_E);
static void setLedOff(tLedType_E led_E);

void Ledc_init(void)
{
    LED_DDR |= ((1 << STATUS_LED_BIT) | (1 << DEBUG_LED_BIT));
}

void Ledc_loop(void)
{
    tU08 i_U08;
    for (i_U08 = 0; i_U08 < NOF_LED; i_U08++)
    {
        if (ledCharacteristics[ledState_astr[i_U08].ledState_E].ledOn_U08 == 0)
        {
            setLedOff(i_U08);
        }
        else if (ledCharacteristics[ledState_astr[i_U08].ledState_E].ledOff_U08
                == 0)
        {
            setLedOn(LED_PORT);
        }
        else
        {
            if (ledState_astr[i_U08].ticksOnOrOff_U08 == 0)
            {
                //TODO: Check pin status, switch it and update counter
                if (1 == 1)
                {
                    ledState_astr[i_U08].ticksOnOrOff_U08 =
                            ledCharacteristics[ledState_astr[i_U08].ledState_E].ledOff_U08;
                }
                else
                {
                    ledState_astr[i_U08].ticksOnOrOff_U08 =
                            ledCharacteristics[ledState_astr[i_U08].ledState_E].ledOn_U08;
                }

            }
            ledState_astr[i_U08].ticksOnOrOff_U08--;
        }
    }
}

void Ledc_setState(tLedType_E ledType_E, tLedState_E ledState_E)
{
    if (ledState_E != ledState_astr[ledType_E].ledState_E)
    {
        /* Reset */
        ledState_astr[ledType_E].ticksOnOrOff_U08 = 0;
    }
    ledState_astr[ledType_E].ledState_E = ledState_E;
}

static void setLedOn(tLedType_E led_E)
{
    switch (led_E)
    {
    case STATUS_LED:
        LED_PORT |= (1 << STATUS_LED_BIT);
        break;
    case DEBUG_LED:
        LED_PORT |= (1 << DEBUG_LED_BIT);
        break;
    default:
        break;
    }
}

static void setLedOff(tLedType_E led_E)
{
    switch (led_E)
    {
    case STATUS_LED:
        LED_PORT &= ~(1 << STATUS_LED_BIT);
        break;
    case DEBUG_LED:
        LED_PORT &= ~(1 << DEBUG_LED_BIT);
        break;
    default:
        break;
    }
}
