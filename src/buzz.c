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

#define BUZZ_ALARM_PERIOD_TIME 150

static tSoundType_E soundType_E;

inline void Buzz_init(void)
{
    CONF_IO(BUZZ_CFG, OUTPUT, 0);
#if !UART_ENABLE
    /* Set to 'CTC' mode, toggle on match */
    TCCR0A |= (1 << WGM01) | (1 << COM0A0);
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

    switch (soundType_E)
    {
    case BUZZ_OFF_E:
        TCCR0B = 0;
        PRR |= _BV(PRTIM0);
        counter_U08 = 0;
        break;
    case BUZZ_ON_E:
        TCCR0B = _BV(CS01) | _BV(CS00);
        PRR &= ~_BV(PRTIM0);
        counter_U08 = 0;
        break;
    case BUZZ_ALARM_E:
        if (counter_U08++ > BUZZ_ALARM_PERIOD_TIME / TICK)
        {
            counter_U08 = 0;
        }
        if (counter_U08 > BUZZ_ALARM_PERIOD_TIME / TICK / 2)
        {
            TCCR0B = _BV(CS01) | _BV(CS00);
            PRR &= ~_BV(PRTIM0);
        }
        else
        {
            TCCR0B = 0;
            PRR |= _BV(PRTIM0);
        }
        break;
    }
}

void Buzz_setSound(const tSoundType_E soundTypeReq_E)
{
    soundType_E = soundTypeReq_E;
}
