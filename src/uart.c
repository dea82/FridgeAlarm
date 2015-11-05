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

static tU08 Uart_Status_U08;
static tU08 Uart_Counter_U08;
static tU08 Uart_Data_U08;
static tU08 Uart_Buffer_U08;

static void transmitHandler(void);
static void clearTimer(void);

ISR(TIM0_COMPA_vect)
{
    if (!(Uart_Counter_U08 & 0x1))
    {
        transmitHandler();
    }
}

void Uart_Enable(void)
{
    CONF_IO(UART_CFG, OUTPUT, 1);
    /* Clear status register. */
    Uart_Status_U08 = 0;
    Uart_Counter_U08 = 0;

#if BAUD == 9600
    OCR0A = 103;
#else
#error "Not supported baud rate.";
#endif

    //CTC-mode
    TCCR0A = (1 << WGM01);

}

tB Uart_Transmit_B(uint8_t data_U08)
{

    tB ret_B;
    if (Uart_Status_U08 & _BV(SW_UART_TX_BUFFER_FULL))
    {
        ret_B = FALSE;
    }
    else
    {
        Uart_Status_U08 |= _BV(SW_UART_TX_BUFFER_FULL);
        Uart_Buffer_U08 = data_U08;

        if (Uart_Counter_U08 == 0)
        {
            Uart_Data_U08 = Uart_Buffer_U08;
            Uart_Status_U08 &= ~_BV(SW_UART_TX_BUFFER_FULL);
            clearTimer();
            /* Clear interrupt flag */
            TIFR = (1 << OCF0A);
            /* Enable interrupt */
            TIMSK = (1 << OCIE0A);
            Uart_Counter_U08 = 2;
            IO_CLR(UART_CFG);
            /* Start timer */
            TCCR0B = (1 << CS00);
        }
        ret_B = TRUE;
    }
    return ret_B;
}

static void transmitHandler(void)
{

    if (Uart_Counter_U08 <= 16)
    {
        if (Uart_Data_U08 & 0x1)
        {
            IO_SET(UART_CFG);
        }
        else
        {
            IO_CLR(UART_CFG);
        }
        Uart_Data_U08 >>= 1;
        Uart_Counter_U08 += 2;
    }
    else if (Uart_Counter_U08 == 18)
    {
        IO_SET(UART_CFG);
        Uart_Counter_U08 += 2;
    }
    else if (Uart_Counter_U08 == 20)
    {
        IO_SET(UART_CFG);
        Uart_Counter_U08 += 2;
    }
    else
    {
        if (Uart_Status_U08 & _BV(SW_UART_TX_BUFFER_FULL))
        {
            Uart_Data_U08 = Uart_Buffer_U08;
            Uart_Status_U08 &= ~_BV(SW_UART_TX_BUFFER_FULL);
            IO_CLR(UART_CFG);
        }
        else
        {
            /* Disable interrupt */
            TIMSK = 0;
        }
        Uart_Counter_U08 = 0;
    }
}

static void clearTimer(void)
{
    /* Stops timer. */
    TCCR0B = 0;
    /* Clear timer. */
    TCNT0 = 0;
}

