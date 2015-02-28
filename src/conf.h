/*
 * conf.h
 *
 *  Created on: 28 feb 2015
 *      Author: andreas
 */

#ifndef CONF_H_
#define CONF_H_

#define DEBUG

#ifdef __AVR_ATtiny13A__


/* Digital Switch Monitor */
#define DSWM_PORT       PORTB
#define DSWM_PIN        PINB
#define DSWM_OPEN_BIT   PB1
#define DSWM_CLOSED_BIT PB2

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





#else
#error "Not supported MCU."
#endif
#endif /* CONF_H_ */
