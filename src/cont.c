/*
 * cont.c
 *
 *  Created on: 31 maj 2015
 *      Author: andreas
 */

#include "butt.h"
#include "conf.h"
#include "cont.h"
#include "dsen.h"
#include "type.h"

void Cont_loop(tLockMode_E lockMode_E)
{
    if(lockMode_E == MCU_LOAD_E)
    {
        CONF_IO(RED_LED_CFG, OUTPUT, 1);
        CONF_IO(GREEN_LED_CFG, OUTPUT, 0);
    }
    else if(Butt_getState() == BUTT_PRESSED_E)
    {
        CONF_IO(RED_LED_CFG, OUTPUT, 1);
        CONF_IO(GREEN_LED_CFG, OUTPUT, 0);
    }
    else if(Dsen_getDoorState_E() == DSEN_CLOSED_E)
    {
        CONF_IO(RED_LED_CFG, OUTPUT, 1);
        CONF_IO(GREEN_LED_CFG, OUTPUT, 0);
    }
    else
    {
        CONF_IO(RED_LED_CFG, OUTPUT, 0);
        CONF_IO(GREEN_LED_CFG, OUTPUT, 1);
    }
}
