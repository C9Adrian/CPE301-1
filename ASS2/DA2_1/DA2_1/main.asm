;
; DA2_1.asm
;
; Created: 2/7/2017 5:36:18 PM
; Author : Luis
;


.CSEG ;Code Segment 

;DEFINE REG'S AND SOME CONST
.DEF COUNT = R16	;Define Count as R16
.DEF COUNT5 = R17	;Define Count5 as R17
.DEF COUNT10 = R18	;Define Count10 as R18
.EQU FIVE = 0x05	;FIVE = 5
.EQU TEN = 0x0A		;TEN = 10 
.DEF CLK = R19		;Define CLK as R19 
.DEF PC_5 = R20		;PB5 will be used to toggle PC.5
.DEF PC_6 = R21		;PB6 will be used to toggle PC.6
.DEF ZERO = R2		;Define Zero as R2

main:
	CLR CLK			;R19 = 0
	CLR ZERO		;R2 = 0 
	CLR R23			;R23 = 0
    LDI R22, 0XFF	;R22 = 0b1111_1111
	OUT DDRD,R22	;PD.0-7 are set as outputs
	LDI R22, 0X31	;R22 = 0b0011_0001
	OUT DDRC, R22	;PC.5 and PC.4 are set as outputs

	FOREVER:
		OUT TCNT0, ZERO		;Load Timer0 = 0
		OUT TCCR0A, ZERO	;Timer0: normal mode, internal clock
		
		;Timer0: enabled, CLk(i/o)/1024
		LDI R23,(1<<CS00)	
		ORI R23, 0X04 		
		OUT TCCR0B, R23
		
		;Create nested loop with the outer loop being similar to 
		;for(int i = 0; i < 7; i++)
		;the inner is a counts up till 255 till exiting
		CLR R26					;R26 = 0
		LDI R24, 7				;R24 = 7
		DELAY:					;do{
			OUT TCNT0,ZERO		;Load Timer0 = 0
			CALL AGAIN_LOOP		;Loop used to count for overflow
			INC R26				;R26 = R26 + 1
			CP R26, R24			
			BRLT DELAY			;}while(R26<7);

		;Another small delay to aquire a 0.25sec delay 
		;more accuratley 
		LDI R26,95				;set the TCNTO to 95
		OUT TCNT0,R26 
		DELAY2:					;do{
			IN R25,TIFR0
			SBRS R25,0			;check if TOV0 is set 
			RJMP DELAY2			;while(TCNT0 < 255)
			
		;Disable Timer and Clear the TOV0 flag
		LDI R25, (1<<TOV0)
		OUT TIFR0,R25
		CLR R25				;R25 = 0
		OUT TCCR0B, R25		;Disable Timer0

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
		OUT PORTC, R25	;OUTPUT TO PORTC
		
		INC COUNT		;COUNT++
		OUT PORTD,COUNT	

		NEGEDGE:
			IN R25, PORTC		
			LDI R26, 0x01	;EXOR PORTC.0 which represents the clock
			EOR R25, R26
			OUT PORTC, R25	;OUTPUT TO PORTC

		RJMP FOREVER

	end:
    rjmp end
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
		
