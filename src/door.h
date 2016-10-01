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
 * @brief Door sensor module
 *
 * @details This module handles the the hall sensor ADC conversion including validation
 * of sensor value. It stores/reads sensor value to/from EEPROM. Finally
 * it also creates door state with help of sensor value.
 */

#ifndef DOOR_H_
#define DOOR_H_

#include "type.h"

typedef enum
{
  DOOR_CLOSED_E,/**< Door considered closed */
  DOOR_OPEN_E	/**< Door considered open */
} tDoor_Position_E;

typedef struct
{
    tDoor_Position_E position_E; /**< Door state */
    tU16 ticksInState_U16; /**< Numer of ticks in state */
}tDoor_State_str ;

/** @brief Initializes door module. Must be called before Door_Loop. */
void Door_Init(void);
/** @brief Updates door state. */
void Door_Loop(void);
tDoor_State_str Door_GetState_str(void);
tB Door_StoreClosedPos_B(void);

#endif /* DOOR_H_ */
