;
; ********************************************
; * Power module 						                 *
; * [Add more info on software version here] *
; * (C)2015 by Andreas Laghamn               *
; ********************************************
;
; Included header file for target AVR type
.NOLIST
.INCLUDE "tn13Adef.inc" ; Header for ATTINY13A
.LIST
;
; ============================================
;   H A R D W A R E   I N F O R M A T I O N
; ============================================
;
;.DEVICE ATtiny13A
; [Add all hardware information here]
;
; ============================================
;      P O R T S   A N D   P I N S
; ============================================
;
; [Add names for hardware ports and pins here]
; Format: .EQU Controlportout = PORTA
;         .EQU Controlportin = PINA
;         .EQU LedOutputPin = PORTA2
; ============================================
;  F I X + D E R I V E D   C O N S T A N T S
; ============================================
;
.EQU DDRB_INIT  = 0b00000111 ;
.EQU PORTB_INIT = 0b00010000 ;
; ============================================
;   R E G I S T E R   D E F I N I T I O N S
; ============================================
;
.DEF BUTTON_RAW_STATE_REG  = R2  ; Old button register 0 = Released, 1 = Pressed
.DEF BUTTON_RAW_TIMER = R3
.DEF BUTTON_STATE_REG = R4
.DEF BUTTON_STATE_TIMER = R5

.DEF TR1         = R16 ; Multipurpose register
.DEF TR2         = R17 ; Multipurpose register
.DEF TR3         = R18 ; Multipurpose register
;
; ============================================
;     E E P R O M   D E F I N I T I O N S
; ============================================
.ESEG
.ORG 0x0000
E2_DoorClosedPos:  .DW 0x0000
;
; ============================================
;       S R A M   D E F I N I T I O N S
; ============================================
;
.DSEG
.ORG  SRAM_START
DoorClosedPos: .BYTE 2
ButtonRawOldState: .BYTE 1
ButtonRawTimeInState: .BYTE 1
; Format: Label: .BYTE N ; reserve N Bytes from Label:
;
; ============================================
;   R E S E T   A N D   I N T   V E C T O R S
; ============================================
;
.CSEG
.ORG $0000
	rjmp Main          ; RESET        | External Pin, Power-on Reset, Brown-out Reset, Watchdog Reset
	reti 		       ; INT0         | External Interrupt Request 0
	rjmp ISR_PCINT0	   ; PCINT0       | Pin Change Interrupt Request 0
	reti               ; TIMER0_OVF   | Timer/Counter0 Overflow
	reti               ; EE_RDY       | EEPROM Ready
	reti 		       ; ANA_COMP     | Analog Comparator
	reti			   ; TIMER0_COMPA | Timer/Counter0 Compare Match A
	reti               ; TIMER0_COMPB | Timer/Counter0 Compare Match B
	reti               ; ADC          | ADC Conversion Complete
	rjmp ISR_WDT       ; WDT          | Watchdog Time-out

;
; ============================================
;     I N T E R R U P T   S E R V I C E S
; ============================================
;
ISR_PCINT0:
; Remember: global interrupt are disabled here!
	ret ; Do not use reti which will enable interrupt again.
;
ISR_WDT:
; Remember: global interrupt are disabled here!
	ret ; Do not use reti which will enable interrupt again.
;
; ============================================
;     M A I N    P R O G R A M    I N I T
; ============================================
;
Main:
	eor R1, R1 ; Init R1 to zero.
	out SREG, R1 ; Clear status register
	out WDTCR, R1 ; Clear WDT register if runaway pointer enables WDT reset by misstake, see AVR132 chap 2.4.
	
; Init stack
	ldi TR1, LOW(RAMEND) ; Init LSB stack
	out SPL,TR1
; Init ports
	ldi TR1, DDRB_INIT
	out DDRB, TR1
	sbi PORTB, PORTB4 ;Enable pull-up on button.
; Power saving
	ldi TR1, (1<<BODS) | (1<<BODSE)
	out BODCR, TR1 ; Turn of BOD during power down
	sbi ACSR, ACD ; Turn off analog comparator
; Initialize others
	sbi PCMSK, PCINT5 ; Enable interrupt on PCINT5 (button), this does not enable the interrupt!
	sbi	ADCSRA, ADPS1 ; Prescaler for ADC clock is set to 4, which gives a ADC clock of 150 kHz @ 600 kHz MCU clock.
	ldi TR1, (1<<MUX1) | (1<<MUX0)
	out ADMUX, TR1 ; Connect pin to ADC with 10-bit precision
; Read stored closed position
	ldi	TR1, E2_DoorClosedPos
	ldi TR2, 2
	ldi XH, HIGH(DoorClosedPos)
	ldi XL, LOW(DoorClosedPos)
Read_Byte:
	sbic EECR, EEPE 
	rjmp Read_Byte ; EEPROM is currently beeing written to, wait...
	out EEARL, TR1 ; Address
	sbi EECR, EERE ; Start EEPROM read
	in TR3, EEDR
	st X+, TR3
	inc TR1
	dec TR2
	brne Read_Byte
	
Loop:
; Initialize WDT to 16 ms
	wdr
	ldi TR1, (1<<WDTIE) | (1<<WDCE)
	out WDTCR, TR1
; Disable button interrupt and clear interrupt flag.
	out GIMSK, R1
	out GIFR, R1
; Enable global interrupt
	sei
; Update time in button state, take care of glitches.
	in TR1, PINB
	com TR1
	andi TR1, 0x1 ; Holds button state 0 = released, 1 = pressed
	
	lds TR2, ButtonRawOldState
	cpse TR1, TR2 ; Has button value changed?
	rjmp Button_Changed

	lds TR2, ButtonRawTimeInState ; Load time in state
	cpi TR2, 0xFF
	breq Check_Button_Stable
	inc TR2
	sts ButtonRawTimeInState, TR2
	rjmp Check_Button_Stable
Button_Changed:
	sts ButtonRawTimeInState, R1

	sts ButtonRawOldState, TR1 ; Store actual state to old.

Check_Button_Stable:	
	
	
	
	
