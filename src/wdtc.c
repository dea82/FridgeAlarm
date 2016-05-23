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

#include "wdtc.h"

#include <avr/io.h>
#include <avr/wdt.h>

#include "conf.h"
#include "type.h"

void Wdtc_SetTimer(const tWdtc_Timeout_E timeout_E, const tB interrupt_B)
{
    switch(timeout_E)
    {
        case WDTC_OFF_E:
            /* Timed sequence to turn WDT off. */
            WDTCR = _BV(WDCE) | _BV(WDE);
            WDTCR = _BV(WDCE);
            break;
        case WDTC_16MS_E:
            WDTCR = _BV(WDIF) | (_BV(WDIE) && interrupt_B) | _BV(WDE);
            break;
        case WDTC_8S_E:
            WDTCR = _BV(WDIF) | (_BV(WDIE) && interrupt_B) | _BV(WDE) | _BV(WDP3) | _BV(WDP0);
            break;
    }
}
