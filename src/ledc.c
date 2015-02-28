/*
 * ledc.c
 *
 *  Created on: 19 feb 2015
 *      Author: andreas
 */

#include <avr/io.h>
#include <avr/pgmspace.h>

#include "conf.h"
#include "ledc.h"

#define LED_PERIOD_TIME 2 /* [100ms] */

typedef struct
{
    tU08 ledOn_U08;
    tU08 ledOff_U08;
} tLedChararteristics;

static tLedState_E ledState_aE[LED_TYPE_SIZE];
PROGMEM const tLedChararteristics ledCharacteristics[LED_STATE_SIZE] =
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

}

void Ledc_setState(tLedType_E ledType_E, tLedState_E ledState_E)
{
    ledState_aE[ledType_E] = ledState_E;
}

static void setLedOn(tLedType_E led_E)
{
    switch(led_E)
    {
    case STATUS_LED:
        break;
    case DEBUG_LED:
        break;
    default:
        break;
    }
}

static void setLedOff(tLedType_E led_E)
{
    switch(led_E)
    {
    case STATUS_LED:
        break;
    case DEBUG_LED:
        break;
    default:
        break;
    }
}
