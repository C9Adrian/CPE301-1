/*
 * DA2_3.c
 *
 * Created: 2/9/2017 5:05:37 PM
 * Author : Luis
 */ 

#include <avr/io.h>

char TOGG_5();
char TOGG_10();

volatile unsigned char count5 = 0;
volatile unsigned char count10 = 0;

int main(void)
{
	unsigned char clk = 0; 
	unsigned char count = 0;

    DDRD = 0xFF;	//PD.0-7 are set as outputs
	DDRC = 0x31;	//PC.5 and PC.4 and PC.0 are set as outputs
	PORTC = 0;		//PC.0-7 OUT LOW
	PORTD = 0;		//PD.0-7 OUT LOW 

    while (1) 
	{
		//reset count 
		if( count == 255)
			count = 0;

		TCCR0A = 0;							//TIMER0: Normal Mode, internal clock
		TCCR0B = (1<<CS02)|(1<<CS00);		//TIMER0: enable, prescalar 1024 - 8MHz/1024

		//Inner loop count from 0 to 255
		//and this is done 7 time to genereate a period of about 0.5s
		for(unsigned char i = 0; i < 7; i++)
		{
			TCNT0 = 0;								//LOAD TIMER0 = 0
			while((TIFR0 & (1<< TOV0)) == 0);		//wait for an overflow
			TIFR0 &= (1<<TOV0);						//clear the overflow(TOV0) 
		}

		//A Few more cycle to generate a more accurate
		//clock period
		TCNT0 = 95;							//LOAD TIMER0 = 95
		while((TIFR0 & (1<<TOV0))== 0 );
		TIFR0 &= (1<<TOV0);					//clear the overflow(TOV0) 
		TCCR0B = 0;							//TIMER0: disable timer 
		
		if(clk == 1)
		{
			clk = 0;						//RESET CLK
			count++;						//Count = Count + 1;
			count10++;						//Count10 = Count10 + 1; 
			count5++;

			PORTC = (TOGG_5()|TOGG_10()| (0x01)); 
			PORTD = count;						//OUTPUT Count
		}

		else if(clk == 0)
		{
			clk = 1;							//SET CLK
			PORTD = count;						//OUTPUT CLOCK
			PORTC ^= 0X01;						//toggle the CLK
		}

	}
}

char TOGG_5()
{
	unsigned char TOG5 = 0;
	TOG5 = 0x10 & PORTC;		//Grab the PC.4 
	if(count5 == 5){			//Check if it's at it 5th rising edge
		count5 = 0;
		TOG5 ^= 0x10;			//toggle and reset counter
	}
								//else do nothing
	return TOG5;
}
char TOGG_10()
{
	unsigned char TOG10 = 0;
	TOG10 = 0x20 & PORTC;	//Grab the PC.5	
	if(count10 == 10){		//Check if it's at it 10th rising edge
		count10 = 0;
		TOG10 ^= 0x20;		//toggle and reset counter
	}
							//else do nothing
	return TOG10;
}
