/*
 * type.h
 *
 *  Created on: 28 feb 2015
 *      Author: andreas
 */
#include <limits.h>

#ifndef TYPE_H_
#define TYPE_H_

/*** TYPE DEFINITIONS ***/

#define  FALSE      0
#define  TRUE       !FALSE

typedef unsigned char tB;

typedef unsigned char tU08;
#define MAX_U08 UCHAR_MAX

typedef unsigned int  tU16;
#define MAX_U16 UINT_MAX

/*** PORT MANIPULATION DEFINITIONS ***/

#define INPUT    0
#define OUTPUT   1
#define PULLUP   1
#define NOPULLUP 0


#define CONF_IO(CFG, IO, EXT)                               \
({                                                          \
    if((IO) == INPUT)                                       \
    {                                                       \
        GET_DDR(CFG) = GET_DDR(CFG) & ~GET_MASK(CFG);       \
        if((EXT) == PULLUP)                                 \
            GET_PORT(CFG) = GET_PORT(CFG) | GET_MASK(CFG);  \
        else if ((EXT) == NOPULLUP)                         \
            GET_PORT(CFG) = GET_PORT(CFG) & ~GET_MASK(CFG); \
    }                                                       \
    else if((IO) == OUTPUT)                                 \
    {                                                       \
        GET_DDR(CFG) = GET_DDR(CFG) | GET_MASK(CFG);        \
        GET_PORT(CFG)=(GET_PORT(CFG)&~GET_MASK(CFG)) |      \
            ((EXT)<<GET_BIT(CFG));                          \
    }                                                       \
})                                                          \

#define IO_SET(CFG)  ({ GET_PORT(CFG) |=  GET_MASK(CFG); })
#define IO_CLR(CFG)  ({ GET_PORT(CFG) &= ~GET_MASK(CFG); })

#define GET_DDR(P,...) (DDR ## P)
#define GET_PORT(P,...) (PORT ## P)
#define GET_PIN(P,...)  (PIN  ## P)
#define GET_BIT(P, bit) (bit)
#define GET_MASK(P, bit) (1 << (bit))
#define GET_STATUS(CFG) (GET_PIN(CFG)&GET_MASK(CFG))

/*** MATH MACRO DEFINITIONS ***/

#define ABS(a)     (((a) < 0) ? -(a) : (a))
#define INC_U08(a) ((a) == MAX_U08 ? MAX_U08 : (a)++)
#define INC_U16(a) ((a) == MAX_U16 ? MAX_U16 : (a)++)

#endif /* TYPE_H_ */
