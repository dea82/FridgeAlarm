/*
 * butt.h
 *
 *  Created on: 24 maj 2015
 *      Author: andreas
 */

#ifndef BUTT_H_
#define BUTT_H_

typedef enum
{
    BUTT_RELEASED_E, BUTT_PRESSED_E
}tButt_State_E;

void Butt_init(void);
void Butt_loop(void);
void Butt_updateState(void);
tButt_State_E Butt_getState(void);
void Butt_enableInterrupt(void);
void Butt_disableInterrupt(void);

#endif /* BUTT_H_ */
