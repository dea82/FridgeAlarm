/*
 * uart.c
 *
 *  Created on: 2 nov 2015
 *      Author: andreas
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#include "conf.h"
#include "type.h"

/* Assembly function. */
extern void transmit (unsigned char);

void Uart_Enable(void)
{
    CONF_IO(UART_CFG, OUTPUT, 1);
}

void Uart_Transmit(char data_U08)
{
    transmit(data_U08);
}

void Uart_TransmitInt(tU08 data_U16)
{
    Uart_Transmit(HI(data_U16));
    Uart_Transmit(LO(data_U16));
}
