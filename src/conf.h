/*
 * conf.h
 *
 *  Created on: 28 feb 2015
 *      Author: andreas
 */
#include <avr/io.h>
#include "type.h"

#ifndef CONF_H_
#define CONF_H_

/* Time the button shall be pressed for a calibration. [ms] */
#define CALIBRATION_TIME_BUTTON 4000
/* Time before the system goes to power save mode when door is closed. [ms] */
#define DEEP_SLEEP_TIME 8000
/* Time before the alarm goes of with open door. [ms] */
#define ALARM_OPEN 5000
/* Lights on after door closed [ms] */
#define LIGHTS_ON_DOOR_CLOSED 4000
/* Minimum time system awake. This it only to give a small flash every 8 second. [ms] */
#define MIN_TIME_AWAKE 250

/* Milliseconds between each loop. */
#define TICK 16

#if defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny13__) || defined(__AVR_ATtiny13A__)

/*************************************************
 * PIN CONFIGURATION
 *************************************************/
#define DSEN_CFG      B,3
#define BUTT_CFG      B,4
#define RED_LED_CFG   B,1
#define GREEN_LED_CFG B,2
#define BUZZ_CFG      B,0

#else
#error "Not supported MCU."
#endif

#if defined(__AVR_ATtiny13__) || defined(__AVR_ATtiny13A__)
    #define WDT_INT WDTIE
#elif defined(__AVR_ATtiny85__)
    #define WDT_INT WDIE
#endif

#endif /* CONF_H_ */
