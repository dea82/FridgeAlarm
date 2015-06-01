#include <avr/eeprom.h>
#include <avr/io.h>
#include <stdio.h>

#include "conf.h"
#include "dsen.h"
#include "type.h"

tU16 EEMEM doorClosed_U16;
tU16 EEMEM doorPartiallyOpen_U16;

static tDoorState_E doorState_E;

void Dsen_init(void)
{
    CONF_IO(DSEN_CFG, INPUT, NOPULLUP);



#if F_CPU == 9600000
    /* Prescaler for ADC clock is set to 64, which gives a ADC clock of 150 kHz. */
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1);
#else
#error "Prescaler for ADC conversion is not supported for this CPU clock."
#endif
    //TODO: increase resolution to 10-bit.
    /* Connect pin to ADC with 8-bit precision */
    ADMUX = (_BV(MUX1) | _BV(MUX0));
}

void Dsen_loop(void)
{
    tU16 sensorValue_U16;

    /* Start conversion */
    ADCSRA |= (1 << ADEN) | (1 << ADSC);

    /* Wait for conversion */
    while(ADCSRA & _BV(ADSC)){}

    ADCSRA &= ~(_BV(ADEN));

    sensorValue_U16 = ADCL;
    sensorValue_U16 |= ((ADCH & 0x3) << 8);

    if(sensorValue_U16 > 512)
    {
        doorState_E = DSEN_OPEN_E;
    }
    else
    {
        doorState_E = DSEN_CLOSED_E;
    }

}

tDoorState_E Dsen_getDoorState_E(void)
{
    return doorState_E;
}
