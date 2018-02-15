;
; DA2_2.asm
;
; Created: 2/10/2017 10:33:31 AM
; Author : Luis
;

.CSEG ;Code Segment 

.org 0
	jmp main 
.org 0x20
	jmp TIM0_OVF

.DEF COUNT = R16	;Define Count as R16
.DEF COUNT5 = R17	;Define Count5 as R17
.DEF COUNT10 = R18	;Define Count10 as R18
.EQU FIVE = 0x05	;FIVE = 5
.EQU TEN = 0x0A		;TEN = 10 
.DEF CLK = R19		;Define CLK as R19 
.DEF PC_5 = R20		;PB5 will be used to toggle PC.5
.DEF PC_6 = R21		;PB6 will be used to toggle PC.6
.DEF ZERO = R2		;Define Zero as R2
.DEF i = R24		;Define i as R24

main:
	CLR CLK			;R19 = 0
	CLR ZERO		;R2 = 0 
	CLR R23			;R23 = 0
	LDI i,6			;i = 6 
    LDI R22, 0XFF	;R22 = 0b1111_1111
	OUT DDRD,R22	;PD.0-7 are set as outputs
	LDI R22, 0X31	;R22 = 0b0011_0010
	OUT DDRC, R22	;PC.5 and PC.4 are set as outputs

	FOREVER:
		OUT TCNT0, ZERO				;Load Timer0 = 0
		OUT TCCR0A, ZERO			;Timer0: normal mode, internal clock
		LDI R23,(1<<CS02)|(1<<CS00)	;Timer0: enabled, CLk(i/o)/1024		
		OUT TCCR0B, R23

		;enable overflow interrupt
		LDI R23,(1<<TOV0)
		STS TIMSK0, R23
		SEI 					;enable global interrupts

		LOOP_FOREVER:
			RJMP LOOP_FOREVER
			

;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;FUNCTIONS

CHECK_TOG_5:
	LDI R25,FIVE		;if(!(COUNT5 == 5))	
	CP COUNT5,R25		;	goto NOT_EQ_5
	BRNE NOT_EQ_5		;else
	LDI R25,0x10		;	toggle PC.4
	CLR COUNT5
	EOR PC_5,R25
	NOT_EQ_5:
RET

CHECK_TOG_10:
	LDI R25,TEN			;if(!(COUNT10 == 10))
	CP COUNT10,R25		;	goto NOT_EQ_10
	BRNE NOT_EQ_10		;else
	LDI R25,0x20		;	toggle PC.5
	CLR COUNT10
	EOR PC_6,R25
	NOT_EQ_10:
RET


;THE ITERATIONS TILL TCNTO > 255 and 
;TOV0 is set 
AGAIN_LOOP:
;wait for overlow
	AGAIN:
		IN R25,TIFR0
		SBRS R25,0
		RJMP AGAIN
	LDI R25, (1<<TOV0)
	OUT TIFR0,R25
RET


TIM0_OVF:
		
		LDI R25, 12
		CP R24, R25
		BRNE IF_NEQ

		CLR R24				;i = 0
		
		;Disable Timer
		CLR R25				;R25 = 0
		OUT TCCR0B, R25		;Disable Timer0

		;CLEAR Timer0 Overflow Flag (TOV0)
		LDI R25, 1			;R25 = 0X01
		OUT TIFR0, R25		

		;Change the CLK
		LDI R25, 0XFF		;R25 = 255
		EOR CLK, R25		;toggle the pulse

		;Check if the clock is Posedge/Negedge
		CP CLK,ZERO			
		BREQ NEGEDGE		;Branch if CLK  == 0

		;increment counter
		INC COUNT5
		INC COUNT10	

		;call the function
		CALL CHECK_TOG_5	

		;call the function
		CALL CHECK_TOG_10

		;OUTPUT TO PORTC 
		CLR R25			;CLEAR R25
		;R25 bit 0 is set since CLK did a posedge
		ORI R25, 0x01	;R25 = 0b0000_0001
		OR R25, PC_5	;R25 = 0b0001_0001
		OR R25, PC_6	;R25 = 0b0011_0001
		OUT PORTC, R25	;PORTC = 0
		
		INC COUNT		;COUNT++
		OUT PORTD,COUNT	

		NEGEDGE:
			IN R25, PORTC		
			LDI R23, 0x01	;EXOR PORTC.0 which represents the clock
			EOR R25, R23
			OUT PORTC, R25	;OUTPUT TO PORTC


		LDI R23,(1<<CS02)|(1<<CS00)	;Timer0: enabled, CLk(i/o)/1024		
		OUT TCCR0B, R23

		;enable overflow interrupt
		LDI R23,(1<<TOV0)
		STS TIMSK0, R23

		RJMP END_TIM0_OVF

	IF_NEQ:
		INC R24		; i++

		;CLEAR Timer0 Overflow Flag (TOV0)
		LDI R25, 1			;R25 = 0X01
		OUT TIFR0, R25

	END_TIM0_OVF:	

RETI
		
	
