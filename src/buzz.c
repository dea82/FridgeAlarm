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


    //TODO: Make sure to connect PWM to correct IO pin.
#if 0

    TCCR0B = (1 << CS01) | (1 << CS00);
    // Set to 'Fast PWM' mode
    TCCR0A |= (1 << WGM01) | (1 << WGM00) | (1 << COM0B1);

    OCR0B = 15;
#endif

}

void Buzz_loop(void)
{
#if 0
    static tU16 counter_U16;

    switch (soundType_E)
    {
    case BUZZ_OFF_E:
        IO_CLR(BUZZ_CFG);
        break;
    case BUZZ_ON_E:
        IO_SET(BUZZ_CFG);
        break;
    case BUZZ_ALARM_E:
        if (counter_U16 > 1000)
        {
            counter_U16 = 0;
        }
        else
        {
            counter_U16++;
        }
        if (counter_U16 < 500)
        {
            IO_SET(BUZZ_CFG);
        }
        else
        {
            IO_CLR(BUZZ_CFG);
        }
        break;
    }
#endif
}

void Buzz_setSound(const tSoundType_E soundTypeReq_E)
{
    soundType_E = soundTypeReq_E;
}
