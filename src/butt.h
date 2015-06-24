/*
 * butt.h
 *
 *  Created on: 24 maj 2015
 *      Author: andreas
 */

#ifndef BUTT_H_
#define BUTT_H_

#include "type.h"

typedef enum
{
    BUTT_RELEASED_E, BUTT_PRESSED_E
}tButt_State_E;

typedef struct
{
    tButt_State_E state_E;
    tU08 tickInState_U08;
} tButt_State_str;

void Butt_init(void);
void Butt_loop(void);
tButt_State_str Butt_getState_str(void);
tB Butt_pressFlank_str(void);
void Butt_enableInterrupt(void);
void Butt_disableInterrupt(void);

#endif /* BUTT_H_ */
