/*
 * conf.c
 *
 *  Created on: 1 mars 2017
 *      Author: andreas
 */
#include "conf.h"

/* Milliseconds between each loop. [ms] */
const tU08 tick_U08c = 16;

/* Time the button shall be pressed for a calibration. [ms] */
const tU16 calibrationTimeButton_U16c = 4000;

/* Time before the alarm goes of with open door. [ms] */
const tU16 alarmOpen_U16c = 40000;

/* Lights on after door closed [ms] */
const tU16 lightsOnDoorClosed_U16c = 2000;

/* Minimum time system awake. This it only to give a small flash every 8 second. [ms] */
const tU08 minTimeAwake_U08c = 100;

/* Address where calibrated door closed position is stored. */
const tU08 calibratedClosedPosAddress_U08c = 0;
