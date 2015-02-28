/*
 * ledc.h
 *
 *  Created on: 19 feb 2015
 *      Author: andreas
 */

#ifndef LEDC_H_
#define LEDC_H_

#include "conf.h"
#include "type.h"

typedef enum {LED_OFF, LED_BLINK, LED_ON, LED_STATE_SIZE} tLedState_E;

void Ledc_init(void);
void Ledc_loop(void);
void Ledc_setState(tLedType_E ledType_E, tLedState_E ledState_E);

#endif /* LEDC_H_ */
