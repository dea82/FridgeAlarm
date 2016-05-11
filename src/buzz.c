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

#define BUZZ_ALARM_PERIOD_TIME 1000 /* [ms]*/

static tSoundType_E soundType_E;

inline void Buzz_init(void)
{
#if !UART_ENABLE
    /* Set to 'CTC' mode, toggle on match */
    TCCR0A |= _BV(WGM01) | _BV(COM0A0);
#endif
#if F_CPU == 8000000
    /* BuzzerFreq = F_CPU / (2 * Prescaler * (1 + OCR0A))
     *
     * In this case gives a buzzer frequency of 1690 Hz
     */
    OCR0A = 36;
#else
#error "Correct compare value to CPU speed."
#endif
}

inline void Buzz_loop(void)
{
    static tU08 counter_U08;

    if (soundType_E == BUZZ_ON_E)
    {
        /* Enable module before accessing registers */
        PRR &= ~_BV(PRTIM0);
        TCCR0B = _BV(CS01) | _BV(CS00);
        counter_U08 = 0;
    }
    else if (soundType_E == BUZZ_ALARM_E)
    {
        if (++counter_U08 > BUZZ_ALARM_PERIOD_TIME / TICK)
        {
            counter_U08 = 0;
        }
        if (counter_U08 > BUZZ_ALARM_PERIOD_TIME / TICK / 2)
        {
            /* Enable module before accessing registers */
            PRR &= ~_BV(PRTIM0);
            TCCR0B = _BV(CS01) | _BV(CS00);
        }
        else
        {
            /* Stop clock before disabling module */
            TCCR0B = 0;
            PRR |= _BV(PRTIM0);
        }

    }
    else
    {
        /* Stop clock before disabling module */
        TCCR0B = 0;
        PRR |= _BV(PRTIM0);
        counter_U08 = 0;
    }
}

void Buzz_setSound(const tSoundType_E soundTypeReq_E)
{
    soundType_E = soundTypeReq_E;
}
