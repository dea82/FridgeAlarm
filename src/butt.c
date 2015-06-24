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

static tButt_State_str buttState_str;

void Butt_init(void)
{
    /* Activate internal pull-up */
    CONF_IO(BUTT_CFG, INPUT, PULLUP);

    /* Pin change interrupt on button - this does not enables the interrupt! */
    PCMSK = _BV(PCINT4);
}

void Butt_loop(void)
{
    static tButt_State_str buttonRawOld_str;
    tU08 buttonRaw_E = (
    GET_STATUS(BUTT_CFG) ? BUTT_RELEASED_E : BUTT_PRESSED_E);

    /* Is raw button glitching */
    if (buttonRawOld_str.state_E == buttonRaw_E)
    {
        INC_U08(buttonRawOld_str.tickInState_U08);
    }
    else
    {
        buttonRawOld_str.tickInState_U08 = 0;
    }

    /* Check if raw value is stable */
    if (buttonRawOld_str.tickInState_U08 >= FILTER_TIME / TICK)
    {

        if (buttState_str.state_E == buttonRaw_E)
        {
            INC_U08(buttState_str.tickInState_U08);
        }
        else
        {
            buttState_str.tickInState_U08 = 0;
        }
        buttState_str.state_E = buttonRaw_E;
    }

    buttonRawOld_str.state_E = buttonRaw_E;

}

tButt_State_str Butt_getState_str(void)
{
    return buttState_str;
}

void Butt_enableInterrupt(void)
{
    GIMSK = _BV(PCIE);
}

void Butt_disableInterrupt(void)
{
    GIMSK = 0;
}
