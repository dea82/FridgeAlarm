#include <avr/eeprom.h>
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

#include "conf.h"
#include "dsen.h"
#include "type.h"
#include "uart.h"

tU16 EEMEM doorClosed_EE;

static tDsen_doorState_str doorState_str;
static tU16 doorClosed_U16;

static tB withinRange_B(const tU16 sensorValue_U16);
static tU16 getDoorRawPos_U16(void);

inline void Dsen_init(void)
{
//    CONF_IO(DSEN_CFG, INPUT, NOPULLUP);

#if F_CPU == 9600000
    /* Prescaler for ADC clock is set to 64, which gives a ADC clock of 150 kHz. */
    ADCSRA = (1 << ADPS2) | (1 << ADPS1);
#elif F_CPU == 1000000
    /* Prescaler for ADC clock is set to 8, which gives a ADC clock of 125 kHz. */
    ADCSRA = (1 << ADPS1) | (1 << ADPS0);
#elif F_CPU == 1200000
    /* Prescaler for ADC clock is set to 8, which gives a ADC clock of 150 kHz. */
    ADCSRA = (1 << ADPS1) | (1 << ADPS0);
#elif F_CPU == 4800000
    /* Prescaler for ADC clock is set to 32, which gives a ADC clock of 150 kHz. */
    ADCSRA = (1 << ADPS2) | (1 << ADPS0);
#elif F_CPU == 600000
    /* Prescaler for ADC clock is set to 4, which gives a ADC clock of 150 kHz. */
    ADCSRA = (1 << ADPS1);
#else
#error "Prescaler for ADC conversion is not supported for this CPU clock."
#endif

    // TODO: Replace with macro
    /* Connect pin to ADC with 10-bit precision */
    ADMUX = (_BV(MUX1) | _BV(MUX0));

    doorClosed_U16 = eeprom_read_word(&doorClosed_EE);
}

inline void Dsen_loop(void)
{
    tU16 sensorValue_U16 = getDoorRawPos_U16();
    Uart_Transmit(0x10);
    //Uart_Transmit_B(0x31);

    tDsen_doorState_E newDoorState_E;
    if (doorClosed_U16 < 512)
    {
        if (sensorValue_U16 > doorClosed_U16 + 50)
        {
            newDoorState_E = DSEN_OPEN_E;
        }
        else
        {
            newDoorState_E = DSEN_CLOSED_E;
        }
    }
    else
    {
        if (sensorValue_U16 < doorClosed_U16 - 50)
        {
            newDoorState_E = DSEN_OPEN_E;
        }
        else
        {
            newDoorState_E = DSEN_CLOSED_E;
        }
    }

    if (doorState_str.doorState_E == newDoorState_E)
    {
        INC_U16(doorState_str.timeInState_U16);
    }
    else
    {
        doorState_str.timeInState_U16 = 0;
    }
    doorState_str.doorState_E = newDoorState_E;

    /* Sensor outputs Vcc/2 when no magnet is present. This will make
     * it nosensitive for magnet mounting direction. */
//sensorValue_U16 = ABS(sensorValue_U16 - 512);
}
static tU16 getDoorRawPos_U16(void)
{
    tU16 sensorValue_U16;
    // Turn on sensor
    IO_SET(DSEN_SWITCH_CFG);
    PRR &= ~_BV(PRADC);
    DIDR0 &= ~_BV(ADC3D);
    _delay_us(5);
    /* Start conversion */
    ADCSRA |= (1 << ADEN) | (1 << ADSC);

    /* Wait for conversion */
    //TODO: Implement sleep while ADC
    while (ADCSRA & _BV(ADSC))
    {
    }
    //TODO: Make another conversion just to be sure? Is the first conversion incorrect? Make mean value?

    ADCSRA &= ~(_BV(ADEN));
    // Turn off sensor
    IO_CLR(DSEN_SWITCH_CFG);
    PRR |= _BV(PRADC);
    DIDR0 |= _BV(ADC3D);

    sensorValue_U16 = ADCL;
    sensorValue_U16 |= ((ADCH & 0x3) << 8);

    return sensorValue_U16;
}

tDsen_doorState_str Dsen_getDoorState_str(void)
{
    return doorState_str;
}

static tB withinRange_B(const tU16 sensorValue_U16)
{
    tB ret_B = FALSE;
    if ((ABS(sensorValue_U16 - 512)) > 40)
    {
        ret_B = TRUE;
    }
    else
    {
        ret_B = FALSE;
    }

    return ret_B;
}

tB Dsen_storeClosedPos(void)
{
    tU16 sensorValue_U16 = getDoorRawPos_U16();
    tB calibrationOk_B = FALSE;
    if (withinRange_B(sensorValue_U16))
    {
        eeprom_write_word(&doorClosed_EE, sensorValue_U16);
        doorClosed_U16 = sensorValue_U16;
        calibrationOk_B = TRUE;
    }
    return calibrationOk_B;
}
