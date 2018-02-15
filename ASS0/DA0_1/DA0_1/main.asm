;
; DA0_1.asm
;
; Created: 1/24/2017 3:22:31 PM
; Author : Luis
;


; Replace with your application code
main:

    LDI r16,55		;r16 = 55
	LDI r17,45		;r17 = 45
	ADD r16,r17		;r16 += r17 (r16 = 45 + 55)
	brvs Overflw	;if Overflow flag is set goto Overflw
	LDI r17, 50		;r17 = 50
	ADD r16,r17		;r16 += r17 (r16 = 100 + 50)

	;flag should set here and goto Overflw
	brvs Overflw	;if Overflow flag is set goto Overflw

;I intentionally add numbers that add
; and create a sum less than 127 
back:

    LDI r16,31		;r16 = 31
	LDI r17,32		;r17 = 32
	ADD r16,r17		;r16 += r17 (r16 = 31 + 32)
	LDI r17, 33		;r17 = 33
	ADD r16,r17		;r16 += r17 (r16 = 63 + 33)

	;the overlow flag shoul be zero and go to noOverflw
	brvc noOverflw	;if Overflow flag is not set goto NoOverflw
	rjmp end

Overflw:
	LDI r17, 0x10	;r16 = 0b0000_0100
	OUT DDRB, r17	;configure Port B bits input: 7:3 & 1:0, output: 2 
	OUT PORTB, r17	;turn PortB on
	rjmp back

noOverflw:
	LDI r16, 0x10	;r16 = 0b0000_0100
	OUT DDRB, r16	;configure Port B bits input: 7:3 & 1:0, output: 2
	LDI r16, 0x00	;r16 = 0b0000_0000
	OUT PORTB, r16	

end:nop