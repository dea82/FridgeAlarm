/*
 * conf.h
 *
 *  Created on: 28 feb 2015
 *      Author: andreas
 */
#include <avr/io.h>

#ifndef CONF_H_
#define CONF_H_

/* Time the button shall be pressed for a calibration. [ms] */
#define CALIBRATION_TIME_BUTTON 4000
/* Time before the system goes to power save mode when door is closed. [ms] */
#define DEEP_SLEEP_TIME 2000
/* Time before the alarm goes of with open door. [ms] */
#define ALARM_OPEN 5000
/* Lights on after door closed [ms] */
#define LIGHTS_ON_DOOR_CLOSED 2000
/* Minimum time system awake. This it only to give a small flash every 8 second. [ms] */
#define MIN_TIME_AWAKE 100

/* Milliseconds between each loop. */
#define TICK 16

/* Debug options */
#define UART_DEBUG_PRINTOUT_ENABLE 0
#define CPU_LOAD_MEASUREMENT_ENABLE 0
#define BAUD_RATE 57600


/* Inherited defines */
#define UART_ENABLE (UART_DEBUG_PRINTOUT_ENABLE | CPU_LOAD_MEASUREMENT_ENABLE)


/* Rationale why Timer0 and PRADC is not disabled:
 * The module is disconnected and all registers connected
 * to the module is frozen and not accessible,
 * Buzz_init and Dsen_init will fail to set registers. */
#if defined(__AVR_ATtiny85__)

#if CPU_LOAD_MEASUREMENT_ENABLE

/* See comment above why these bits are choosen. */
#define PRR_INIT _BV(PRUSI)
#else
#define PRR_INIT (_BV(PRUSI) | _BV(PRTIM1))
#endif

#elif defined(__AVR_ATtiny13A__)

#define PRR_INIT (0)
#define WDIF WDTIF
#define WDIE WDTIE

#endif


#define GET_PORT(P,...) (PORT ## P)

#if defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny13A__)

/*************************************************
 * PIN CONFIGURATION
 *************************************************/
#define DSEN_CFG        B,3
#define BUTT_CFG        B,5
#define RED_LED_CFG     B,2
#define GREEN_LED_CFG   B,1
#define BUZZ_CFG        B,0
#define DSEN_SWITCH_CFG B,4
#define UART_CFG        B,0

#define DDRB_INIT   0b00010111
#define PORTB_INIT  0b00100000

#else
#error "Not supported MCU."
#endif

#endif /* CONF_H_ */
