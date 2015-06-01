/*
 * butt.c
 *
 *  Created on: 24 maj 2015
 *      Author: andreas
 */

#include <avr/io.h>

#include "butt.h"
#include "conf.h"
#include "type.h"

#define FILTER_TIME  70  /*  70 ms */

typedef struct
{
    tButt_State_E state_E;
    tU08 timeInState_U08;
} tButt_State_str;

static tButt_State_E buttState_E;

void Butt_init(void)
{
    /* Activate internal pull-up */
    CONF_IO(BUTT_CFG, INPUT, PULLUP);

    /* Pin change interrupt on button - this does not enables the interrupt! */
    PCMSK = _BV(PCINT0);
}

void Butt_loop(void)
{
    Butt_updateState();
}

void Butt_updateState(void)
{
    static tButt_State_str buttonRawOld_str;
    tU08 buttonRaw_E = (
            GET_STATUS(BUTT_CFG) ? BUTT_RELEASED_E : BUTT_PRESSED_E);

    /* Is raw button glitching */
    if (buttonRawOld_str.state_E == buttonRaw_E)
    {
        buttonRawOld_str.timeInState_U08++;
    }
    else
    {
        buttonRawOld_str.timeInState_U08 = 0;
    }

    /* Check if raw value is stable */
    if (buttonRawOld_str.timeInState_U08 >= FILTER_TIME/TICK)
    {
        buttState_E = buttonRaw_E;
    }

    buttonRawOld_str.state_E = buttonRaw_E;
}

tButt_State_E Butt_getState(void)
{
    return buttState_E;
}

void Butt_enableInterrupt(void)
{
    GIMSK = _BV(PCIE);
}

void Butt_disableInterrupt(void)
{
    GIMSK = 0;
}
