/*
 * buzz.c
 *
 *  Created on: 18 feb 2015
 *      Author: andreas
 */

#include <avr/io.h>

#include "buzz.h"
#include "conf.h"
#include "type.h"

static tSoundType_E soundType_E;

inline void Buzz_init(void)
{
#if !UART_ENABLE
        /* Set to 'CTC' mode, toggle on match */
        TCCR0A |= (1 << WGM01) | (1 << COM0A0);
#endif
    /* TODO: Change compare value*/
    OCR0A = 30;
}

inline void Buzz_loop(void)
{
#if 1
    static tU08 counter_U08;

    switch (soundType_E)
    {
    case BUZZ_OFF_E:
        TCCR0B = 0;
//        PRR |= _BV(PRTIM0);
        break;
    case BUZZ_ON_E:
//        PRR &= ~_BV(PRTIM0);
        TCCR0B = _BV(CS01) | _BV(CS00);
        break;
    case BUZZ_ALARM_E:
//        PRR &= ~_BV(PRTIM0);
        if (counter_U08 > 100)
        {
            counter_U08 = 0;
        }
        else
        {
            counter_U08++;
        }
        if (counter_U08 < 50)
        {
            TCCR0B = _BV(CS01) | _BV(CS00);
        }
        else
        {
            TCCR0B = 0;
        }
        break;
    }
#endif
}

void Buzz_setSound(const tSoundType_E soundTypeReq_E)
{
    soundType_E = soundTypeReq_E;
}
