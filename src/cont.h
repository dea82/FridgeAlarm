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


typedef enum
{
    CONT_SHORT_DEEP_SLEEP_E,
    CONT_LONG_DEEP_SLEEP_E,
    CONT_SLEEP_WITH_TIMER_RUNNING_E
} tSleepMode_E;

void Cont_init(void);
void Cont_loop(void);
tSleepMode_E Cont_sleepMode_E(void);

#endif /* CONT_H_ */
