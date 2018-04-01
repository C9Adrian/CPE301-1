/*
 * DA4_Ser.c
 *
 * Created: 2/20/2017 12:32:11 PM
 * Author : Luis
 */ 

#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 16000000L
#define SERV_MIN 97
#define SER_MAX 535

void initateTimer();				//initialize the Timer and set PWM with a 50Hz 
void ADC0init();					//initialize ADC0 as an input 
float position(unsigned short);		//get the servo position 

int main(void)
{
	//temporarily hold the value from the analog channel
	unsigned short val;
	//get the value of Vin
	float servo;

	ADC0init();
	initateTimer();

    while (1) 
    {
		//Start conversion 
		ADCSRA |= (1 << ADSC);
		//wait conversion to finish
		while((ADCSRA & (1 << ADIF)) == 0);

		if(!(val == (ADCH<<1))){
				//take the value from the upper bits of the ADC
				val = ADCH<<1;
		}

		//Get the Vin being read 
		servo = position(val);
		//this will determine the position 
		servo = (servo*438)+97;
		
		//The OCR1A value is based on what value ADC0 is reading
		//then develop a PWM based on ADC0 reading 
		if(servo < SERV_MIN)
			OCR1A = SERV_MIN;
		else
			OCR1A = (int) servo;
    }

	return 0;
}

void initateTimer(){

//TOP = ((Focnx*N)/F_cpu)-1
//Desire Focnx = 50 Hz
ICR1 = 4999;
//ICR1 = (int)((16e6/(50*64))-1);

//SET Timer 1 to have the top to be ICR1
//FAST PWM reading the OCRA in non-inverting mode
//A prescalar of 64 => 16MHz / 64
TCCR1A = (1<<COM1A1)|(1<<WGM11);
TCCR1B = (1<<WGM13)|(1<<WGM12)|(1<<CS11)|(1<<CS10);
}
void ADC0init(){

	//SET ADC0 as an input 
	DDRC |= (0<<PC0);

	//PWM pin (OC1A)
	DDRB |= (1<<PB1);
	
	//SET AVcc with external capacitor at AREF
	//and ADC0 as an input MUX[3:0] = 0b0000
	ADMUX = 0x60;
	
	//Turn on the ADC for conversion 	
	//CLKadc/128 = ADPS[2:0] = 0b111
	ADCSRA = 0x87;
	//= (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	//Run in Free mode
	ADCSRB = 0x0;	
}
float position(unsigned short val){
	//ADC = Vin*1024.0/Vref
	//Vin = ADC * 5(Vref)/1024.0 
 return ((val*5)/1024.0);
}

