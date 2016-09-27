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
    .section .text

;############################################################
;#  FUNCTION: Mcul_StartMeasurement
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
    .global Mcul_StartMeasurement
Mcul_StartMeasurement:
    out     TCCR1, r1   ; Stop timer
    out     TCNT1, r1   ; Reset timer
    in      r18, TIFR   ; Clear overflow flag by setting bit
    sbr     r18, TOV1
    out     TIFR, r18


    ; Start timer
    out     TCCR1, r24
    ret

;############################################################
;#  FUNCTION: Mcul_StopMeasurement
;#
;#  SUMMARY
;#  This function will stop the timer used for calulation
;#  which can be used for CPU load calculation, benchmarks
;#  etc.
;#
;#  INPUT
;#  -
;#
;#  RETURN
;#  -
;#
;############################################################
    .global Mcul_StopMeasurement
Mcul_StopMeasurement:
    out     TCCR1, r1     ;  Stop timer as fast as possible
    ret
    .end
#endif
