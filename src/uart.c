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


#define BAUD 9600

#define SW_UART_TX_BUFFER_FULL     1
#define SW_UART_RX_BUFFER_FULL     2
#define SW_UART_RX_BUFFER_OVERFLOW 3
#define SW_UART_FRAME_ERROR        4

extern void transmit (unsigned char);

void Uart_Enable(void)
{
    CONF_IO(UART_CFG, OUTPUT, 1);
}

void Uart_Transmit(char data_U08)
{
    transmit(data_U08);
}
