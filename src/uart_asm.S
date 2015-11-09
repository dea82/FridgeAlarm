#include <avr/io.h>
#include "uart_asm.h"
//.include "tn13Adef.inc"

.extern Uart_Counter_U08

.global startTransmit
//.global TIM0_COMPA_vect


startTransmit:

    movw ZL,r24
    ld r18, Z
    out ASM_PORT, 0
    st Z, 0


/*
TIM0_COMPA_vect:

    ldi R18, 137
    out _SFR_IO_ADDR(OCR0A), R18

    reti
*/
