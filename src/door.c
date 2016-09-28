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

#include "door.h"

#include <avr/io.h>
#include <util/delay.h>

#include "conf.h"
#include "eepr.h"

/**
 * Due to ~0.135 V supply loss to sensor compared with supply to ADC-converter
 * the normal ADC-range will be 0-986 instead of 0-1023.
 */
#define UNAFFECTED_SENSOR_VALUE 493
/* Offset from stored position to trigger alarm. [-]*/
#define DOOR_CLOSED_OFFSET 20
/* Minimum accepted door position (calculated offset from 512) [-] */
#define MIN_CAL_DOOR_CLOSED_POS 60

static tDoor_State_str doorState_str;
static tU16 doorClosed_U16;
static tU16 doorPos_U16;

static tB withinRange_B(const tU16 sensorValue_U16);
static tU16 getDoorRawPos_U16(void);

/**
 * @brief Setup ADC conversion.
 */
void Door_Init(void)
{
#if F_CPU == 9600000
    /* Prescaler for ADC clock is set to 64, which gives a ADC clock of 150 kHz. */
    ADCSRA = (1 << ADPS2) | (1 << ADPS1);
#elif F_CPU == 8000000
    /* Prescaler for ADC clock is set to 64, which gives a ADC clock of 125 kHz. */
    ADCSRA = (1 << ADPS2) | (1 << ADPS1);
#else
#error "Prescaler for ADC conversion is not supported for this CPU clock."
#endif

    /* Connect pin to ADC with 10-bit precision */
    ADMUX = (_BV(MUX1) | _BV(MUX0));

    doorClosed_U16 = (Eepr_Read_U08(CALIBRATED_CLOSED_POS_ADDRESS) << 8);
    doorClosed_U16 += Eepr_Read_U08(CALIBRATED_CLOSED_POS_ADDRESS + 1);
}

void Door_Loop(void)
{
    doorPos_U16 = getDoorRawPos_U16();

    tDoor_Position_E newposition_E;
    /* Algorithm handles both direction of magnet field */
    if (doorClosed_U16 < UNAFFECTED_SENSOR_VALUE)
    {
        if (doorPos_U16 > doorClosed_U16 + DOOR_CLOSED_OFFSET)
        {
            newposition_E = DOOR_OPEN_E;
        }
        else
        {
            newposition_E = DOOR_CLOSED_E;
        }
    }
    else
    {
        if (doorPos_U16 < doorClosed_U16 - DOOR_CLOSED_OFFSET)
        {
            newposition_E = DOOR_OPEN_E;
        }
        else
        {
            newposition_E = DOOR_CLOSED_E;
        }
    }

    if (doorState_str.position_E == newposition_E)
    {
        INC_U16(doorState_str.ticksInState_U16);
    }
    else
    {
        doorState_str.ticksInState_U16 = 0;
    }
    doorState_str.position_E = newposition_E;

}

/**
 * Make an AD conversion
 * @return  door position
 */
static tU16 getDoorRawPos_U16(void)
{
    tU16 sensorValue_U16;


    /* Prepare for sleep - check if timer is running.
     * Assume that timer is not running if module is disconnected. */
    if (PRR & _BV(PRTIM0))
    {
        /* Sleep mode ADC noise reduction */
        MCUCR = _BV(SE) | _BV(SM0);
    }
    else
    {
        /* Sleep mode idle */
        MCUCR = _BV(SE);
    }

    /* Turn on sensor */
    IO_SET(DOOR_SWITCH_CFG);

    /* Activate ADC module */
    PRR &= ~_BV(PRADC);

    /* According to (Allegro 1302) data sheet of sensor star-up time is 5 us.*/
    _delay_us(5);

    /* Clear any pending interrupt - should not happen */
    ADCSRA |= _BV(ADIF);
    /* Start conversion */
    ADCSRA |= _BV(ADEN) | _BV(ADSC) | _BV(ADIE);

    /* Necessary check if any other interrupt wakes the MCU. */
    while (ADCSRA & _BV(ADSC))
    {
        /* Wait for conversion */
        asm volatile("sleep"::);
    }

    /* Disable interrupt and ADC. */
    ADCSRA &= ~(_BV(ADEN) | _BV(ADIE));

    /* Turn off sensor */
    IO_CLR(DOOR_SWITCH_CFG);

    /* Power savings */
    PRR |= _BV(PRADC);

    sensorValue_U16 = ADCL;
    sensorValue_U16 |= ((ADCH & 0x3) << 8);

    return sensorValue_U16;
}

tDoor_State_str Door_GetState_str(void)
{
    return doorState_str;
}

static tB withinRange_B(const tU16 sensorValue_U16)
{
    tB ret_B = FALSE;
    /* Cast should not be a problem due to the 10-bit resolution. */
    if (ABS((tS16)sensorValue_U16 - UNAFFECTED_SENSOR_VALUE) > MIN_CAL_DOOR_CLOSED_POS)
    {
        ret_B = TRUE;
    }
    else
    {
        ret_B = FALSE;
    }

    return ret_B;
}

tB Door_StoreClosedPos_B(void)
{
    tB calibrationOk_B = FALSE;
    if (withinRange_B(doorPos_U16))
    {
        Eepr_Write(CALIBRATED_CLOSED_POS_ADDRESS, HI(doorPos_U16));
        Eepr_Write(CALIBRATED_CLOSED_POS_ADDRESS + 1, LO(doorPos_U16));
        doorClosed_U16 = doorPos_U16;
        calibrationOk_B = TRUE;
    }
    return calibrationOk_B;
}
