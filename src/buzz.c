/*
 * buzz.c
 *
 *  Created on: 18 feb 2015
 *      Author: andreas
 */

#include <avr/io.h>

#include "buzz.h"
#include "conf.h"

static tSoundType_E soundType_E;

void Buzz_init(void)
{
    CONF_IO(BUZZ_CFG, OUTPUT, 0);
}

void Buzz_loop(void)
{
    switch (soundType_E)
    {
    case BUZZ_OFF_E:
        IO_CLR(BUZZ_CFG);
        break;
    default:
        IO_SET(BUZZ_CFG);
        break;
    }
}

void Buzz_setSound(tSoundType_E soundTypeReq_E)
{
    soundType_E = soundTypeReq_E;
}
