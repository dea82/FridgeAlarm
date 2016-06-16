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
 * @brief      Button module
 *
 * @details    Purpose of this module is to provide a button state by reading a
 *             digital switch. It also provides a function to enable/disable
 *             button as an interrupt source however it does not provide any
 *             interrupt routine.
 */


#ifndef BUTT_H_
#define BUTT_H_

#include "type.h"

/**
 * The enumeration of button state
 */
typedef enum
{
    BUTT_RELEASED_E, /**< Button released */
    BUTT_PRESSED_E	/**< Button pressed */
}tButt_State_E;

/**
 * £Active button state and for how many ticks
 */
typedef struct
{
    tButt_State_E state_E; /**< Button state */
    tU08 tickInState_U08; /**< Ticks in state */
} tButt_State_str;

/**
 * @brief      Initializes button module
 */
void Butt_init(void);

/**
 * @brief      Updates button module's outputs
 * 
 * @note       This function shall be called periodically.
 *
 * @warning    Before making any call to this function make sure that
 * 			   Butt_init() has been called at least once.
 */
void Butt_loop(void);

/**
 * @brief      Get actual button state
 *
 * @return     Button state
 */
tButt_State_str Butt_getState_str(void);

/**
 * @brief      Enable interrupt for button state change
 */
void Butt_enableInterrupt(void);

/**
 * @brief      Disable interrupt for button state change
 */
void Butt_disableInterrupt(void);

#endif /* BUTT_H_ */
