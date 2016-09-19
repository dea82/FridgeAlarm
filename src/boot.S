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

#define _SFR_ASM_COMPAT 1
#define __SFR_OFFSET 0

#include <avr/io.h>
#include "conf.h"

.org 0x0000
; Important to use vectors instead of init0 due to progmem data
; is placed before init sections.
.section .vectors
eor r1,r1
rjmp Init
reti         ; PCINT0
Init:
; Power reduction - turn off digital input buffers except for button
ldi r16, _BV(AIN0D) | _BV(AIN1D) | _BV(ADC1D) | _BV(ADC2D) | _BV(ADC3D)
out DIDR0, r16
;Turns off (Timer1) and USI
ldi r16, PRR_INIT
out PRR, r16
rjmp __do_clear_bss
#if defined(__AVR_ATtiny85__)
reti        ; ADC Conversion Complete
nop
nop
nop
reti        ; WDT timeout
#elsif defined(__AVR_ATtiny13A__)
reti        ; WDT timeout
reti        ; ADC Conversion Complete
#endif
; Initialization of SRAM is done here automatically by avr-gcc.
.section .init9
rjmp main
