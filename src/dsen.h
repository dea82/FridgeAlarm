/*
 * hsen.h
 *
 *  Created on: 23 maj 2015
 *      Author: andreas
 */

#ifndef DSEN_H_
#define DSEN_H_

#include "type.h"

typedef enum
{
  DSEN_CLOSED_E,
  DSEN_OPEN_E
} tDsen_doorState_E;

typedef struct
{
    tDsen_doorState_E doorState_E;
    tU16 timeInState_U16;
}tDsen_doorState_str ;

void Dsen_init(void);
void Dsen_loop(void);
tDsen_doorState_str Dsen_getDoorState_str(void);
tB Dsen_calibrationNeeded_B(void);
tB Dsen_storeClosedPos_B(void);

#endif /* DSEN_H_ */
