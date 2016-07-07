/*
The MIT License (MIT)

Copyright (c) 2015-2016 Andreas L.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#ifndef TYPE_H_
#define TYPE_H_

#include <limits.h>

/*** TYPE DEFINITIONS ***/

#define  FALSE      0
#define  TRUE       !FALSE

#ifndef __ASSEMBLER__

typedef unsigned char tB;

typedef unsigned char tU08;
#define MAX_U08 UCHAR_MAX

typedef unsigned int  tU16;
#define MAX_U16 UINT_MAX

typedef signed int tS16;

#endif

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

#define IO_PIN(CFG)  (GET_BIT(CFG))
#define IO_PORT(CFG) (GET_PORT(CFG))


#define GET_DDR(P,...) (DDR ## P)
#define GET_PORT(P,...) (PORT ## P)
#define GET_PIN(P,...)  (PIN  ## P)
#define GET_BIT(P, bit) (bit)
#define GET_MASK(P, bit) (1 << (bit))
#define GET_STATUS(CFG) (GET_PIN(CFG)&GET_MASK(CFG))

/* MATH MACRO DEFINITIONS */

#define ABS(a)     (((a) < 0) ? -(a) : (a))
#define INC_U08(a) ((a) == MAX_U08 ? MAX_U08 : (a)++)
#define INC_U16(a) ((a) == MAX_U16 ? MAX_U16 : (a)++)
#define LO(a)      ((tU08)((a) & 0xFF))
#define HI(a)      ((tU08)((a) >> 8) )

/* Pin interrupt macro */
#if defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny13A__)
#define PCINT(CFG)   ({ PCMSK |= GET_MASK(CFG); })
#endif


/* Register redefinitions */
#ifndef EEPE
#define EEPE EEWE
#endif
#ifndef EEMPE
#define EEMPE EEMWE
#endif
#ifndef BODS
#define BODS BPDS
#endif
#ifndef BODSE
#define BODSE BPDSE
#endif

#endif /* TYPE_H_ */
