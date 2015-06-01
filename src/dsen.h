/*
 * hsen.h
 *
 *  Created on: 23 maj 2015
 *      Author: andreas
 */

#ifndef DSEN_H_
#define DSEN_H_

typedef enum
{
  DSEN_CLOSED_E,
  DSEN_PARTIALLY_OPEN_E,
  DSEN_OPEN_E
} tDoorState_E;

void Dsen_init(void);
void Dsen_loop(void);
tDoorState_E Dsen_getDoorState_E(void);

#endif /* DSEN_H_ */
