/*
 * dswm.h
 *
 *  Created on: 20 feb 2015
 *      Author: andreas
 */

#ifndef DSWM_H_
#define DSWM_H_

typedef enum {OPEN_VALIDATING_E, OPEN_CONFIRMED_E, CLOSED_VALIDATING_E, CLOSED_CONFIRMED_E, UNKNOWN_E} tDoorState_E;
typedef struct {
    tDoorState_E state_E;
    unsigned int timeInState_U16;
} tDoorState_str;

void Dswm_init(void);
void Dswm_loop(void);
tDoorState_E Dswm_getDoorState(void);

#endif /* DSWM_H_ */
