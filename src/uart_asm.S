#define _SFR_ASM_COMPAT 1
#define __SFR_OFFSET 0

#include <avr/io.h>
/* Include header with defined BAUD_RATE */
#include "conf.h"

#define bitcnt  r25
#define data    r24
#define loopcnt r23

/* Max baud 83333 @ 1 Mhz */
/* Min baud  1292 @ 1 Mhz */

/* Number of cycles per bit to achive correct baudrate. */
#define CYCLES_PER_BIT ((F_CPU + (BAUD_RATE/2))/BAUD_RATE)

/* Static number of cycles in transmit function between each bit. */
#define CYCLES_IN_TRANSMIT 10

/* Number of cycles that needs to be delayed (wasted). */
#define DELAY_CYCLES (CYCLES_PER_BIT - CYCLES_IN_TRANSMIT)

/* Calculate number of delay loops needed. */
#define DELAY_LOOPS ((DELAY_CYCLES + 1) / 3)

/* Check if baudrate can be achived. */
#if DELAY_LOOPS > 255 || DELAY_LOOPS < 1
    #error Specified baud rate can't be achived.
#endif

/* Calculate number of "balance" cycles needed. Delay function can only delay with at multiple of 3. This handles 1-2 cycles. */
#define EXTRA_DELAYS ((DELAY_CYCLES + 1) % 3)


    .global transmit
transmit:
    cli                     ; Disable interrupt
    ldi     bitcnt, 10      ; Number of bits to be sent 1 start bit 8 data bits 1 stop bit.
    com     data            ; Invert data and set carry

putchar0:
    ; Total cycles 10
    brcc    putchar1                            ; ### true = 2, false = 1
    cbi     PORTB, PORTB0   ; Transmit 0        ; ### 2
    rjmp    putchar2        ; else              ; ### 2
putchar1:
    sbi     PORTB, PORTB0   ; Transmit 1        ; ### 2
    nop                     ; to equalize delay ; ### 1
putchar2:
    ldi     loopcnt, DELAY_LOOPS                ; ### 1
1:
    dec     loopcnt                             ; ### 1
    brne    1b                                  ; ### true = 2, false = 1

    lsr     data            ; Get next bit      ; ### 1
    dec     bitcnt          ; decrese cnt       ; ### 1
#if EXTRA_DELAYS > 0   
    nop
#endif
#if EXTRA_DELAYS > 1   
    nop
#endif
    brne    putchar0        ; send next         ; ### true = 2, false = 1
    reti                    ; return and enable interrupt again

