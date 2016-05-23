;The MIT License (MIT)
;
;Copyright (c) 2015-2016 Andreas L.
;
;Permission is hereby granted, free of charge, to any person obtaining a copy
;of this software and associated documentation files (the "Software"), to deal
;in the Software without restriction, including without limitation the rights
;to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
;copies of the Software, and to permit persons to whom the Software is
;furnished to do so, subject to the following conditions:
;
;The above copyright notice and this permission notice shall be included in
;all copies or substantial portions of the Software.
;
;THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
;IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
;FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
;AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
;LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
;OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
;THE SOFTWARE.

; This is a module for measuring time between two points.

#define _SFR_ASM_COMPAT 1
#define __SFR_OFFSET 0

#include <avr/io.h>

#if defined(__AVR_ATtiny85__)
    .section .data
maxCycles:
    .data 0

    .section .text

;############################################################
;#  FUNCTION: Cpul_startPoint
;#
;#  SUMMARY
;#  This function will start the timer used for calulation
;#  which can be used for CPU load calculation, benchmarks
;#  etc.
;#
;#  INPUT
;#  TCCR1 register (prescaler)
;#
;#  RETURN
;#  -
;#
;############################################################
    .global Cpul_startPoint
Cpul_startPoint:
    out     TCCR1, r1   ; Stop timer
    out     TCNT1, r1   ; Reset timer
    in      r18, TIFR   ; Clear overflow flag by setting bit
    sbr     r18, TOV1
    out     TIFR, r18


    ; Start timer
    out     TCCR1, r24
    ret

;############################################################
;#  FUNCTION: Cpul_stopPoint
;#
;#  SUMMARY
;#  This function will stop the timer used for calulation
;#  which can be used for CPU load calculation, benchmarks
;#  etc. It will also store the maximum number of cycles in
;#  RAM variable "maxCycles". If also handles overflow of
;#  counter and saturates counter value to 0xFF.
;#
;#  INPUT
;#  -
;#
;#  RETURN
;#  -
;#
;############################################################
    .global Cpul_stopPoint
Cpul_stopPoint:
    out     TCCR1, r1     ;  Stop timer as fast as possible
    in      r18, TCNT1    ;  Read counter value
    in      r19, TIFR     ;  Do we have an overflow?
    sbrc    r19, TOV1
    ldi     r18, 0xFF     ;  Overflow has occured set to max
    lds     r19, maxCycles;  What is the max value stored?
    cp      r18, r19
    brlo    Cpul_stopPoint_1
    sts     maxCycles, r18
Cpul_stopPoint_1:
    ret

;############################################################
;#  FUNCTION: Cpul_getMaxCycles_U08
;#
;#  SUMMARY
;#  Returns maximum number of cycles identified.
;#
;#  INPUT
;#  -
;#
;#  RETURN
;#  maxCycles (one byte, r24)
;#
;############################################################
    .global Cpul_getMaxCycles_U08
Cpul_getMaxCycles_U08:
    lds r24, maxCycles
    ret

;############################################################
;#  FUNCTION: Cpul_clearMaxCycles
;#
;#  SUMMARY
;#  Clears maximum number of cycles identified.
;#
;#  INPUT
;#  -
;#
;#  RETURN
;#  -
;#
;############################################################
    .global Cpul_clearMaxCycles
Cpul_clearMaxCycles:
    sts maxCycles, r1
    ret

    .end
#endif
