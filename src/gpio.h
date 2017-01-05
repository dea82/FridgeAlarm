#include "stdint.h"

typedef struct
{
    volatile uint8_t * const port;
    const uint8_t pin;
} GPIOPin_t;

typedef const struct
{
    GPIOPin_t P0;
    GPIOPin_t P1;
    GPIOPin_t P2;
} port_t;

typedef const struct
{
    port_t portB;
} MCU_t;

void setGpioPinHi(const GPIOPin_t * const GPIOPin);
void setGpioPinLo(const GPIOPin_t * const GPIOPin);
