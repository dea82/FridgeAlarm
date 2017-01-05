#include "gpio.h"

void setGpioPinHi(const GPIOPin_t * const GPIOPin)
{
    *GPIOPin->port |= (1 << GPIOPin->pin);
}

void setGpioPinLo(const GPIOPin_t * const GPIOPin)
{
    *GPIOPin->port &= ~(1 << GPIOPin->pin);
}
