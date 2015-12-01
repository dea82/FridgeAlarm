#define _SFR_ASM_COMPAT 1
#define __SFR_OFFSET 0

#include <avr/io.h>

#define bitcnt  r25
#define data    r24
#define temp    r23

/*

    us per bit @ B baud = 1/B * 1000000 = bit_time [us]

    cycle time = 1/F_CPU * 1000000 = cycle_time [us]


    nof_cycles_per_bit = bit_time / cycle_time + 0.5F = (F_CPU + (BAUD/2))/BAUD


*/


#define CLOCK 1600000
#define BAUD_RATE 19200


#define BIT_COUNT (((CLOCK + (BAUD_RATE/2))/BAUD_RATE - 8 + 1) / 3)


    .global transmit
transmit:
    ldi     bitcnt, 10      ; Number of bits to be sent 1 start bit 8 data bits 1 stop bit.
    com     data            ; Invert data
    sec                     ; Set carry to send start bit

putchar0:
    brcc    putchar1                            ; ### true = 2, false = 1
    sbi     PORTB, PORTB0   ; Transmit 1        ; ### 2
    rjmp    putchar2        ; else              ; ### 2
putchar1:
    cbi     PORTB, PORTB0   ; Transmit 0        ; ### 2
    nop                     ; to equalize delay ; ### 1
putchar2:
    ldi     temp, BIT_COUNT ;                   ; ### 1
1:
    dec     temp            ;                   ; ### 1
    brne    1b              ;                   ; ### true = 2, false = 1


    lsr     data            ; Get next bit      ; ### 1
    dec     bitcnt          ; decrese cnt       ; ### 1
    brne    putchar0        ; send next         ; ### true = 2, false = 1
                            ; else
    ret                     ; return

