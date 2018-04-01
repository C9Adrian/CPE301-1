/*
 * DA3.c
 *
 * Created: 2/15/2017 5:08:39 PM
 * Author : Luis
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <math.h>

#define F_CPU 16000000L
//UPDATE According to delay
#define DELAY 500

char PWM_Desired(unsigned char);
int main(void)
{
	//TIMER 0 A&B
	//Fast PWM Mode
	//WITH NO PRESCALING
	//CHECK OCF0A and OCF0B
	//TOP OCR0A[7:0] and TOP OCR0B[7:0]
	TCCR0A = _BV(COM0A1)|_BV(COM0B1)|_BV(WGM01)|_BV(WGM00);
	TCCR0B = _BV(CS00);
	
	//TIMER 2 
	//Fast PWM Mode
	//WITH NO PRESCALING
	//TOP 0CR2A[7:0]
	//TIFR2 TO CHECK OCFA
	TCCR2A = _BV(COM2A1)|_BV(WGM21)|_BV(WGM20);
	TCCR2B = _BV(CS20);
	
	//PD.6 is OC0A PD.5 is OC0B
	DDRD = 0x60;
	//PB.3 is OC2A
	DDRB = 0x08;

    while (1) 
    {	
		//DUTY CYLCE FROM 10% to 90%
		//OCR0A = BLUE
		//OCR0B = RED
		//OCR2A = GREEN
		for(unsigned char i = 0; i <= 90; i += 10)
		{	
			unsigned char val = PWM_Desired(i);	

			//SET the desired counter based on duty cycle
			//and after have .5s delay
			OCR0A = val;
			_delay_ms(DELAY);
			OCR0B = val;
			_delay_ms(DELAY);
			OCR2A = val;
			_delay_ms(DELAY);
		}	

		//DUTY CYLCE FROM 90% to 10%
		for(unsigned char i = 90; i >= 0; i -= 10)
		{
			unsigned char val = PWM_Desired(i);
			//SET the desired counter based on duty cycle
			//and after have .5s delay
			OCR0A = val;
			_delay_ms(DELAY);
			OCR0B = val;
			_delay_ms(DELAY);
			OCR2A = val;
			_delay_ms(DELAY);
		}
    }
}

//Calculate our OCRxx value based on the passed in duty cycle
//OCRxx = ((256*Duty Cycle)/100-1
char PWM_Desired(unsigned char DC){
	unsigned char temp;
	temp = ((256*DC)/100)-1;
	return temp;
}