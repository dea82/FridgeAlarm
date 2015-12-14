/*
 * uart.h
 *
 *  Created on: 2 nov 2015
 *      Author: andreas
 */

#ifndef UART_H_
#define UART_H_

void Uart_Enable(void);
void Uart_TransmitChar(const char data_U08);
void Uart_TransmitInt(const tU16 data_U16);
void Uart_TransmitStr(const const char *str);
#endif /* UART_H_ */
