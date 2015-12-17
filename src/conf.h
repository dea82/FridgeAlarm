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
#define ALARM_OPEN 40000
/* Lights on after door closed [ms] */
#define LIGHTS_ON_DOOR_CLOSED 2000
/* Minimum time system awake. This it only to give a small flash every 8 second. [ms] */
#define MIN_TIME_AWAKE 100

/* Milliseconds between each loop. */
#define TICK 16

#define UART_ENABLE 1
#define BAUD_RATE 57600
#define CPU_LOAD 0

#define MEASURE_CPU_LOAD (CPU_LOAD & UART_ENABLE)



#define GET_PORT(P,...) (PORT ## P)

#if defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny13__) || defined(__AVR_ATtiny13A__)

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

#else
#error "Not supported MCU."
#endif

#if defined(__AVR_ATtiny13__) || defined(__AVR_ATtiny13A__)
#define WDT_INT WDTIE
#define WDIF_C WDTIF
#elif defined(__AVR_ATtiny85__)
#define WDT_INT WDIE
#define WDIF_C WDIF
#endif

#endif /* CONF_H_ */
