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

#define __SFR_OFFSET 0

#include <avr/io.h>
/* Include header with defined BAUD_RATE */
#include "conf.h"
#include "type.h"

#define bitcnt  r25
#define data    r24
#define loopcnt r23

/* Max baud 83333 @ 1 Mhz */
/* Min baud  1292 @ 1 Mhz */

/* Number of clock cycles per bit to achive correct baudrate. */
#define CYCLES_PER_BIT ((F_CPU + (BAUD_RATE/2))/BAUD_RATE)

/* Static number of clock cycles in transmit function between each bit. */
#define CYCLES_IN_TRANSMIT 10

/* Number of clock cycles that needs to be delayed (wasted). */
#define DELAY_CYCLES (CYCLES_PER_BIT - CYCLES_IN_TRANSMIT)

/* Calculate number of delay loops needed.
 * The one "1" is  compensation for the last loop which is only 2 clock cycles long, instead of normal 3 clock cycles.
 */
#define DELAY_LOOPS ((DELAY_CYCLES + 1) / 3)

/* Check if baudrate can be achived. */
#if DELAY_LOOPS > 255 || DELAY_LOOPS < 1
    #error Specified baud rate can't be achived.
#endif

/* Calculate number of "balance" clock cycles needed. Delay function can only delay with at multiple of 3. This handles 1-2 clock cycles. */
#define EXTRA_DELAYS ((DELAY_CYCLES + 1) % 3)

    .global transmit
transmit:
    cli                     ; Disable interrupt
    ldi     bitcnt, 10      ; Number of bits to be sent 1 start bit 8 data bits 1 stop bit.
    com     data            ; Invert data and set carry

putchar0:
    ; Total clock cycles 10
    brcc    putchar1                            ; ### true = 2, false = 1
    cbi     IO_PORT(UART_CFG), IO_PIN(UART_CFG) ; Transmit 0        ; ### 2
    rjmp    putchar2                            ; else              ; ### 2
putchar1:
    sbi     IO_PORT(UART_CFG), IO_PIN(UART_CFG) ; Transmit 1        ; ### 2
    nop                                         ; to equalize delay ; ### 1
putchar2:
    ldi     loopcnt, DELAY_LOOPS                ; ### 1
delay:
    dec     loopcnt                             ; ### 1
    brne    delay                               ; ### true = 2, false = 1

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
