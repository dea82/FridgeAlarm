/*
 * cont.h
 *
 *  Created on: 31 maj 2015
 *      Author: andreas
 */

#ifndef CONT_H_
#define CONT_H_

typedef enum
{
    NO_LOCK_E, BOD_RESET_E, WDT_RESET_E, MCU_LOAD_E,
} tLockMode_E;


void Cont_loop(tLockMode_E lockMode_E);

#endif /* CONT_H_ */
