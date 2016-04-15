; This is a module for measuring time between two points.

#define _SFR_ASM_COMPAT 1
#define __SFR_OFFSET 0

#include <avr/io.h>

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
;#  -
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
    
    
    ; Start timer with prescaler 64
    ldi     r18, 7        
    out     TCCR1, r18    
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
;#  FUNCTION: Cpul_getMaxCycles                               
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
    .global Cpul_getMaxCycles
Cpul_getMaxCycles:
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
