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

#include "uart.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#include "conf.h"
#include "type.h"

/* Assembly function. */
extern void transmit(unsigned char);

void Uart_Enable(void)
{
    CONF_IO(UART_CFG, OUTPUT, 1);
}

void Uart_TransmitChar(const char data_U08)
{
    transmit(data_U08);
}

void Uart_TransmitInt(const tU16 data_U16)
{
    Uart_TransmitChar(HI(data_U16));
    Uart_TransmitChar(LO(data_U16));
}

void Uart_TransmitStr(const char *str)
{
    while (*str)
    {
        Uart_TransmitChar(*str++);
    }
}
