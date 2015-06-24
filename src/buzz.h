/*
 * buzz.h
 *
 *  Created on: 18 feb 2015
 *      Author: andreas
 */

#ifndef BUZZ_H_
#define BUZZ_H_
typedef enum
{
    BUZZ_OFF_E,
    BUZZ_ON_E,
    BUZZ_ALARM_E
} tSoundType_E;
void Buzz_init(void);
void Buzz_loop(void);
void Buzz_setSound(const tSoundType_E soundTypeReq_E);

#endif /* BUZZ_H_ */
