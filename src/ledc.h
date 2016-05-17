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

typedef enum
{
    LEDC_OFF_E,
    LEDC_GREEN_E,
    LEDC_RED_E,
    LEDC_ORANGE_E,
    LEDC_GREEN_BLINK_E,
    LEDC_RED_BLINK_E,
} tLedState_E;

void Ledc_init(void);
void Ledc_loop(void);
void Ledc_setState(const tLedState_E state_E);

#endif /* LEDC_H_ */
