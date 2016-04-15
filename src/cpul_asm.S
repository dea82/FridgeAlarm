; This is a module for measuring time between two points.

#define _SFR_ASM_COMPAT 1
#define __SFR_OFFSET 0

#include <avr/io.h>

    .section .data
max_cycles:
    .data 0
    
    .section .text
    
    .global startTimer
startTimer:
    out     TCCR1, r1   ; Stop timer                        ;
    out     TCNT1, r1   ; Reset timer                       ;
    ldi     r18, 7                                          ;
    out     TCCR1, r18    ; Start timer with prescaler 64   ;
    ret                                                     ;#4

    .global stopTimer
stopTimer:                                                  ;#3
    out     TCCR1, R1                                       ;#1
    in      r24, TCNT1                                      ;
    ret                                                     ;

    .end
