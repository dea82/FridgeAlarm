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

/* Time the button shall be pressed for a calibration. [ms] */
#define CALIBRATION_TIME_BUTTON 4000
/* Time before the system goes to power save mode when door is closed. [ms] */
#define DEEP_SLEEP_TIME 2000
/* Time before the alarm goes of with open door. [ms] */
#define ALARM_OPEN 40000
/* Lights on after door closed [ms] */
#define LIGHTS_ON_DOOR_CLOSED 2000
/* Minimum time system awake. This it only to give a small flash every 8 second. [ms] */
#define MIN_TIME_AWAKE 100

/* Milliseconds between each loop. */
#define TICK 16

/* Inherited defines */
#define UART_ENABLE (DEBUG_ENABLE | CPU_LOAD)

/* Rationale why Timer0 and PRADC is not disabled:
 * The module is disconnected and all registers connected
 * to the module is frozen and not accessible,
 * Buzz_init and Dsen_init will fail to set registers. */
#if defined(__AVR_ATtiny85__)

#if CPU_LOAD
/* See comment above why these bits are choosen. */
#define PRR_INIT _BV(PRUSI)
#else
#define PRR_INIT (_BV(PRUSI) | _BV(PRTIM1))
#endif

#elif defined(__AVR_ATtiny13A__)

#define PRR_INIT (0)
#define WDIF WDTIF
#define WDIE WDTIE

#endif

#if defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny13A__)

/*************************************************
 * PIN CONFIGURATION
 *************************************************/
#define DOOR_CFG        B,3
#define BUTT_CFG        B,5
#define RED_LED_CFG     B,2
#define GREEN_LED_CFG   B,1
#define BUZZ_CFG        B,0
#define DOOR_SWITCH_CFG B,4
#define UART_CFG        B,0

/* Uart and Buzzer shares pin. To be able to have multi-function
 * on pin buzzer must be disconnected from pin before transmitting. */
#define UART_PRE_TX Buzz_pauseOn()
#define UART_POST_TX Buzz_pauseOff()

#define DDRB_INIT   0b00010111
#define PORTB_INIT  0b00100000

#else
#error "Not supported MCU."
#endif

#endif /* CONF_H_ */
