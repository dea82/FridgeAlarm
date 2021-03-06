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

/**
 * @file
 *
 */

#include "ledc.h"

#include "conf.h"
#include "type.h"

static const tU08 blinkPeriodTime_U08c = 120; /* ms */

static tLedc_State_E ledState_E = LEDC_OFF_E;

void Ledc_Init(void)
{
}
/**
 * @brief Turns led on or off by request
 *
 */
void Ledc_Loop(void)
{
    static tU08 counter_U08;
    if (++counter_U08 > blinkPeriodTime_U08c / tick_U08c)
    {
        counter_U08 = 0;
    }
    switch (ledState_E)
    {
    case LEDC_OFF_E:
        IO_CLR(GREEN_LED_CFG);
        IO_CLR(RED_LED_CFG);
        break;
    case LEDC_GREEN_E:
        IO_SET(GREEN_LED_CFG);
        IO_CLR(RED_LED_CFG);
        break;
    case LEDC_RED_E:
        IO_CLR(GREEN_LED_CFG);
        IO_SET(RED_LED_CFG);
        break;

    case LEDC_ORANGE_E:
        IO_SET(GREEN_LED_CFG);
        IO_SET(RED_LED_CFG);
        break;
    case LEDC_GREEN_BLINK_E:
        if (counter_U08 > (blinkPeriodTime_U08c / tick_U08c / HALF_DIVISOR))
        {
            IO_SET(GREEN_LED_CFG);
        }
        else
        {
            IO_CLR(GREEN_LED_CFG);
        }
        IO_CLR(RED_LED_CFG);
        break;
    case LEDC_RED_BLINK_E:
        if (counter_U08 > (blinkPeriodTime_U08c / tick_U08c / HALF_DIVISOR))
        {
            IO_SET(RED_LED_CFG);
        }
        else
        {
            IO_CLR(RED_LED_CFG);
        }
        IO_CLR(GREEN_LED_CFG);
        break;
    }
}

void Ledc_SetState(const tLedc_State_E state_E)
{
    ledState_E = state_E;
}

void Ledc_SetOrange(void)
{
    IO_SET(GREEN_LED_CFG);
    IO_SET(RED_LED_CFG);
}
