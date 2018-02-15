;
; DA0.asm
;
; Created: 1/24/2017 2:45:44 PM
; Author : Luis Ruiz
;


; Replace with your application code
start:
    LDI r16,0x0F			;r16 = 0b0000_1111
	OUT DDRB, r16		;configure PortB bits 3:0 as outputs, bits 7:4 as inputs
	LDI r16,0x04			;r16 = 0b0000_0100	
	OUT PORTB,r16		;turn PortB2 on
	rjmp start			;repat
    
