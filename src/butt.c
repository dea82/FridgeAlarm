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
 */
#include "butt.h"

#include <avr/io.h>

#include "conf.h"

static const tU08 filterTime_U08c = 70;  /*  70 ms */

static tButt_State_str buttState_str;

void Butt_Init(void)
{
    /* Pin change interrupt on button - this does not enables the interrupt,
       this is done later. */
    PCINT(BUTT_CFG);
}

void Butt_Loop(void)
{
    static tButt_State_str buttonRawOld_str;
    tButt_State_E buttonRaw_E =
      (GET_STATUS(BUTT_CFG) ? BUTT_RELEASED_E : BUTT_PRESSED_E);

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
    if ((buttonRawOld_str.tickInState_U08 >= filterTime_U08c / tick_U08c) &&
        (buttState_str.state_E != buttonRaw_E))
    {
        buttState_str.state_E = buttonRaw_E;
        buttState_str.tickInState_U08 = 0;
    }
    else
    {
        INC_U08(buttState_str.tickInState_U08);
    }

    buttonRawOld_str.state_E = buttonRaw_E;
}

tButt_State_str Butt_GetState_str(void)
{
    return buttState_str;
}

void Butt_EnableInterrupt(void)
{
    /* This enables the pin change interrupt. */
    GIMSK = _BV(PCIE);
}

void Butt_DisableInterrupt(void)
{
    /* Disable interrupt */
    GIMSK = 0;

    /* Important to clear interrupt flag (by writing ONE!),
     * otherwise an interrupt will be triggered as soon as global interrupt flag
     *  is set again. */
    GIFR = _BV(PCIF);
}
