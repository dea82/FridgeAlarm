#define _SFR_ASM_COMPAT 1
#define __SFR_OFFSET 0

#include <avr/io.h>

.org 0x0000
.section .init0
eor r1,r1
rjmp Init
ret         ; PCINT0
Init:
;Check for WDT reset (save code size and assume true) - if a runaway pointer enables it,
;then it must be disabled here because it's kept after a reset! Ref. AVR132 chap 2.4. */
out WDTCR, r1
rjmp __do_clear_bss
nop
nop
nop
reti        ; ADC Conversion Complete
nop
nop
nop
ret         ; WDT timeout
; Initialization of SRAM is done here automatically by avr-gcc.
.section .init9
rjmp main
