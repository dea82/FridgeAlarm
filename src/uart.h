/*
 * uart.h
 *
 *  Created on: 2 nov 2015
 *      Author: andreas
 */

#ifndef UART_H_
#define UART_H_

void Uart_Enable(void);
void Uart_TransmitU08(char data_U08);
void Uart_TransmitInt(tU08 data_U16);
#endif /* UART_H_ */
