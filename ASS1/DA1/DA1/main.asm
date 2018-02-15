;
; DA1.asm
;
; Created: 2/1/2017 6:02:46 PM
; Author : Luis
;


.CSEG						;CODE SEGMENT

main:
	.DEF COUNT = R16		;Define COUNT as R16
	.DEF NUM1 = R19			;Define NUM as R19
	.DEF ZERO = R0			;Define ZERO  as R0
	.DEF SUM7H = R21
	.DEF SUM7L = R20
	.DEF SUM3H = R24
	.DEF SUM3L = R23	

	CLR ZERO				;R0 = 0
    LDI COUNT,0				;R16 = 0

	;Z contains RAMEND 
	LDI ZH, high(RAMEND)	;ZH = RAMEND[15:8]
	LDI ZL, low(RAMEND)		;ZL = RAMEND[7:0]
	;Z contains RAMEND/2 
	LSR ZH					;{0,[15:9]} C = [8]
	ROR ZL					;{C,[7:1]} C = [0]
	
	MOVW X,Z				;X = Z get a copy of Z 
	LDI r18, 25				;R18 = 25 

	store_loop:				;loop which will store the numbers in the array
		cp COUNT, r18		;compare r16 to 25 
		BRGE exit_store		;check if R16 >= 25

		mov R17, XL			;R17 = XL
		ST X+,R17			;mem[X] = XL ; X = X + 1 	

		INC COUNT			;r16 = r16 + 1
		RJMP store_loop		
	exit_store:
	
	CLR COUNT				;r16 = 0

	sum_loop:				;parse through the array 
							;do{
		LD NUM1,Z+			;R19 = MEM[X] 

		CALL DIV_7			;go to function DIV_7			
		CALL DIV_3			;go to function DIV_3

		INC COUNT			;R16 = R16 + 1
		CP COUNT,R18		
		BRLT sum_loop		;}while(COUNT < 25);
	
	LDI R17, 0x18
	OUT DDRB,R17
	CP R20,ZERO
	BRLO DONT_OUT_4	
	BREQ DONT_OUT_4
	LDI R17,0x10
	OUT PORTB,R17
	DONT_OUT_4:

	CP R23,ZERO
	BRLO DONT_OUT_3
	ORI R17, 0x08	
	OUT PORTB,R17
	DONT_OUT_3:

end:
	nop 
	rjmp end 

;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
; FUNCTION 
DIV_7:						;Function to divide by 7
	CLR R26					;R26 = 0
	SUBI R26,-7				;R26 = 7
	MOV R27, R26			;R27 = 7
	div_loop_7:
		CP NUM1,R26			;CHECK IF R26%R27
		BREQ POSTIVE_7		;If R26%27 = 0 then it is divisible
		BRLO NEGATIVE_7		;If R27 < R26 then its not divisble by 7
		ADD R26,R27			;R26 = R26 + 7 
		BRCS NEGATIVE_7		;If the Addition operation above creates a carry
							;then the number being divided isnt divisble by 7
		RJMP div_loop_7	

	POSTIVE_7:
		ADD SUM7L,NUM1		;ADD SUM7 = SUM7 + MEM[X]
		ADC SUM7H,ZERO	

	NEGATIVE_7:
			RET

DIV_3:							;Function to divide by 3
	CLR R26						;R26 = 0
	SUBI R26,-3					;R26 = 3
	MOV R27,R26					;R27 = 3
	div_loop_3:
		CP NUM1,R26				;CHECK IF R26%R27
		BREQ POSTIVE_3			;IF R26%27 = 0 then it is divisible
		BRLO NEGATIVE_3			;IF R27 < R26 then its not divisble by 3
		ADD R26,R27				;R26 = R26 + 3
		BRCS NEGATIVE_3			;If the Addition operation above creates a carry
								;then the number being divided isnt divisble by 3
		RJMP div_loop_3	

	POSTIVE_3:
		ADD SUM3L,NUM1			;ADD SUM3 = SUM3 + MEM[X]
		ADC SUM3H,ZERO	

	NEGATIVE_3:		
			RET