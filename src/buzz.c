/*
The MIT License (MIT)

Copyright (c) 2015-2016 Andreas L.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "buzz.h"

#include <avr/io.h>

#include "conf.h"
#include "type.h"

#define BUZZ_ALARM_PERIOD_TIME 1000 /* [ms]*/

static tSoundType_E soundType_E;

static void turnOn(void);
static void turnOff(void);

inline void Buzz_init(void)
{
/* BuzzerFreq = F_CPU / (2 * Prescaler * (1 + OCR0A))
 *   
 * In this case gives a buzzer frequency of 1690 Hz
 */    
#if F_CPU == 8000000
    OCR0A = 36;
#elif F_CPU == 9600000
    OCR0A = 43;     
#else
#error "Correct compare value to CPU speed."
#endif
}

inline void Buzz_loop(void)
{
    static tU08 counter_U08;

    if (soundType_E == BUZZ_ON_E)
    {
        turnOn();
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
            turnOn();
        }
        else
        {
            turnOff();
        }
    }
    else
    {
        turnOff();
        counter_U08 = 0;
    }
}

static void turnOn(void)
{
    /* Enable module before accessing registers */
    PRR = PRR_INIT & ~_BV(PRTIM0);
    /* Set to 'CTC' mode, toggle on match */
    TCCR0A = _BV(COM0A0) | _BV(WGM01);
    /* Start clock - prescaler 64 */
    TCCR0B = _BV(CS01) | _BV(CS00);
}

static void turnOff(void)
{
    /* Stop clock */
    TCCR0B = 0;
    /* Normal port operation, disconnect OC0A with potential of leaving
     * pin state high. */
    TCCR0A = 0;
    /* Disconnect module */
    PRR = PRR_INIT | _BV(PRTIM0);
}

void Buzz_setSound(const tSoundType_E soundTypeReq_E)
{
    soundType_E = soundTypeReq_E;
}
