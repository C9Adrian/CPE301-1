/*
 * DA4_DC.c
 *
 * Created: 2/20/2017 2:08:50 PM
 * Author : Luis
 */ 

#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 16000000UL

unsigned short getADC();				//get the value read from ADC0
void initateTimer();					//initialize the Timer and set PWM with a 50Hz
void ADC0init();						//initialize ADC0 as an input
void update_OC1A(unsigned char);		//update the value of the DC

int main(void)
{
	//temporarily hold the value from the analog channel
	unsigned short val;

	//DUTY CYCLE 
	unsigned char DC;
	
	//set the a nibble as output 
	DDRD = 0x0F;

	ADC0init();
	initateTimer();

	while (1)
	{
		//Start conversion
		ADCSRA |= (1 << ADSC);
		//wait conversion to finish
		while((ADCSRA & (1 << ADIF)) == 0);
		//Get value that was converted
		val = ADC;

		if(val > 512)
		{
			//enable power to 1,2EN
			//P.D0 and PD.1 for 1A HIGH and 2A LOW
			//FOWARD
			PORTD = 0x03;

			//GET THE DUTY CYCLE BASED ON VOLTAGE IN
			//Then update OC1A to put DC
			DC = (unsigned char)((100.0*val)/1023);
			update_OC1A(DC);

		}
		else
		{
			//enable power to 1,2EN
			//P.D0 and PD.1 for 1A LOW and 2A HIGH
			//BACKWARD
			PORTD =0x05;

			//GET THE DUTY CYCLE BASED ON VOLTAGE IN
			//Then update OC1A to put DC
			DC = (uint8_t)((100.0*(1023-val))/1024);
			update_OC1A(DC);
		}
		_delay_ms(700);
	}

	return 0;
}

void initateTimer()
{
	//Set PORTB1 pin as output
	// make OC1A as output.
	DDRB |= (1<<DDB1);	
	// Output compare mode on OC1A. Fast PWM with top = ICR1.
	// Clear OC1A on Compare match and set at bottom.
	TCCR1A |= (1<<COM1A1)|(0<<COM1A0)|(0<<COM1B1)|(0<<COM1B0)|(0<<FOC1A)|(0<<FOC1B)|(1<<WGM11)|(0<<WGM10);

	// Start timer with pre-scaler 64
	TCCR1B |= (0<<ICNC1)|(0<<ICES1)|(1<<WGM13)|(1<<WGM12)|(0<<CS12)|(1<<CS11)|(1<<CS10);
	
	//TOP =  (F_CPU / (N * F_pwm)) - 1, where N is the prescaler = 64, and F_pwm is the desired 50Hz frequency.
	ICR1 = 4999;  
}
void ADC0init()
{
	DDRC	&= ~(0<<DDC0);	// SET PC.0 as an input  
	ADCSRA	= 0x87;			// Enable ADC and CLK/128
	ADMUX	= (1<<REFS0);	// VCC reference, ADC0 single ended input
}
void update_OC1A(unsigned char DC)
{
	//SET OC1A to create desire Duty Cycle based on the value passed in
	OCR1A = (unsigned short)((DC * 4999.0)/100.0);
}
