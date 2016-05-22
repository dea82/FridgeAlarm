#define _SFR_ASM_COMPAT 1
#define __SFR_OFFSET 0

#include <avr/io.h>
#include "conf.h"

.org 0x0000
.section .init0
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
