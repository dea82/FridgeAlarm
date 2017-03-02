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

#ifndef CONF_H_
#define CONF_H_

#include <avr/io.h>

#include "type.h"
#ifndef __ASSEMBLER__
extern const tU08 tick_U08c;
extern const tU16 calibrationTimeButton_U16c;
extern const tU16 alarmOpen_U16c;
extern const tU08 tick_U08c;
extern const tU16 lightsOnDoorClosed_U16c;
extern const tU08 minTimeAwake_U08c;
extern const tU08 calibratedClosedPosAddress_U08c;
#endif

/* Rationale why Timer0 and PRADC is not disabled:
 * The module is disconnected and all registers connected
 * to the module is frozen and not accessible,
 * Buzz_Init and Dsen_init will fail to set registers. */
#if defined(__AVR_ATtiny85__)

#if WCET
/* See comment above why these bits are choosen. */
#define PRR_INIT _BV(PRUSI)
#else
#define PRR_INIT (_BV(PRUSI) | _BV(PRTIM1))
#endif

#elif defined(__AVR_ATtiny13A__)

#define PRR_INIT (0)
#define WDIF WDTIF
#define WDIE WDTIE
#else
#error "Conf module does not support selected MCU."
#endif

#if defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny13A__)

/*************************************************
 * PIN CONFIGURATION
 *************************************************/
#define DOOR_CFG        B,3
#define BUTT_CFG        B,5
#if WCET
#define RED_LED_CFG     B,1
#else
#define RED_LED_CFG     B,2
#endif
#if defined(__AVR_ATtiny85__)
#define GREEN_LED_CFG   B,1
#define BUZZ_CFG        B,0
#else
#define GREEN_LED_CFG   B,0
#define BUZZ_CFG        B,1
#endif
#define DOOR_SWITCH_CFG B,4
#define UART_CFG        B,2

#define DDRB_INIT   0b00010111
#define PORTB_INIT  0b00100000

#else
#error "Not supported MCU."
#endif

#endif /* CONF_H_ */
