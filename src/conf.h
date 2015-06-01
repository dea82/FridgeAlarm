/*
 * conf.h
 *
 *  Created on: 28 feb 2015
 *      Author: andreas
 */
#include <avr/io.h>

#ifndef CONF_H_
#define CONF_H_

#define DEBUG
#define REDUCE_PWR 1

#define INPUT    0
#define OUTPUT   1
#define PULLUP   1
#define NOPULLUP 0

#define CONF_IO(CFG, IO, EXT)                               \
({                                                          \
    if((IO) == INPUT)                                       \
    {                                                       \
    	GET_DDR(CFG) = GET_DDR(CFG) & ~GET_MASK(CFG);       \
        if((EXT) == PULLUP)                                 \
            GET_PORT(CFG) = GET_PORT(CFG) | GET_MASK(CFG);  \
        else if ((EXT) == NOPULLUP)                         \
            GET_PORT(CFG) = GET_PORT(CFG) & ~GET_MASK(CFG); \
    }                                                       \
    else if((IO) == OUTPUT)                                 \
    {                                                       \
        GET_DDR(CFG) = GET_DDR(CFG) | GET_MASK(CFG);        \
        GET_PORT(CFG)=(GET_PORT(CFG)&~GET_MASK(CFG)) |      \
            ((EXT)<<GET_BIT(CFG));                          \
    }                                                       \
})                                                          \

#define IO_SET(CFG)  ({ GET_PORT(CFG) |=  GET_MASK(CFG); })
#define IO_CLR(CFG)  ({ GET_PORT(CFG) &= ~GET_MASK(CFG); })

#define GET_DDR(P,...) (DDR ## P)
#define GET_PORT(P,...) (PORT ## P)
#define GET_PIN(P,...)  (PIN  ## P)
#define GET_BIT(P, bit) (bit)
#define GET_MASK(P, bit) (1 << (bit))
#define GET_STATUS(CFG) (GET_PIN(CFG)&GET_MASK(CFG))

#ifdef __AVR_ATtiny13__

#define TICK 16

/*************************************************
 * PIN CONFIGURATION
 *************************************************/
#define DSEN_CFG      B,3
#define BUTT_CFG      B,0
#define RED_LED_CFG   B,1
#define GREEN_LED_CFG B,2
#define BUZZ_CFG      B,4




/* Buzzer */
#define BUZZER_PORT PORTB
#define BUZZER_DDR  DDRB
#define BUZZER_BIT  DDB4

/* LED */
typedef enum {STATUS_LED, DEBUG_LED, LED_TYPE_SIZE} tLedType_E;
#define NOF_LED        LED_TYPE_SIZE
#define LED_PORT       PORTB
#define LED_DDR        DDRB
#define STATUS_LED_BIT DDB3
#define DEBUG_LED_BIT  DDB0
#define STATUS_LEDD PORTB, DDB3




#else
#error "Not supported MCU."
#endif

#endif /* CONF_H_ */
