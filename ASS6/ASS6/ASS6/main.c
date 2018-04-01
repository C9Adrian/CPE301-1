/*
 * Author : Luis
 */ 

#include <avr/io.h>
#include "ioe.h"
#include <util/delay.h>
#include <stdio.h>

#define F_CPU 16000000UL

/*
 * Procedure to initialize USART0 asynchronous with enabled RX/TX, 8 bit data,
 * no parity, and 1 stop bit. 
*/
void usart0_init_ ()
{
	// To set baud rate
	UBRR0H = ((ASYNCH_NORM) >> 8);				//top nibble
	UBRR0L = (uint8_t) ((ASYNCH_NORM)) ;		//lower byte		

	COMM_START();									// enable transmit/receive
	
	// asynchronous, 8N1, disable parity, 1 stop bit
	ASYNCH_MODE();
	DisParity();
	STOPBIT_1();
	CHAR8();
}

// Return a char from the serial buffer
/*  Use this function if the RX interrupt is not enabled.
* 	Returns 0 on empty buffer
*/

unsigned char getChar_(void)
{
	//Check if something was received and then 
	//return the item 
	while(!(UCSR0A & _BV(RXC0)));
	return (unsigned char) UDR0;
}

//Transmits a byte
/*
 * 	Use this function if the TX interrupt is not enabled.
 * 	Blocks the serial port while TX completes
 */
void putChar_(unsigned char data)
{
	//What until the buffer is empty 	
	while(!(UCSR0A & _BV(UDRE0)));
	UDR0 = (unsigned char)data;
}

/*A string print called printm that uses a 
char array and your putchar clone to transmit
strings*/

void printm(unsigned char *str)
{
	//While it's not NULL
	while(*str != '\0')
	{
		putChar_(*str);
		++str;
	}
}

/*uses an uninitialized char array and your getchar clone to
construct a string for your ATmega328P */
const unsigned char* scanm(void)
{
	//Allocate buffer of size 256
	//and a ptr to it
	unsigned char buff[BUFF] = {0};
	unsigned char* ptr;
	ptr = buff;

	while((*ptr = getChar_())){
		if(*ptr == '\n'|| *ptr == '\0' )break;
		putChar_(ptr);
		++ptr;
	}

	return buff;
}
void ADC0init()
{
	DDRC	&= ~(0<<DDC0);	// SET PC.0 as an input
	ADCSRA	= 0x87;			// Enable ADC and CLK/128
	ADMUX	= (1<<REFS0);	// VCC reference, ADC0 single ended input
}

double Convert(double val)
{
	return (((val*5.0)/1024.0));
}
void main()
{
	char str[30];
	double val;
	unsigned int prev;
	usart0_init_();
	ADC0init();
	while(1)
	{

		//Start conversion
		ADCSRA |= (1 << ADSC);
		//wait conversion to finish
		while((ADCSRA & (1 << ADIF)) == 0);

		//Get value that was converted
		if(!((unsigned int)ADC == prev)){

			prev = ADC;
			val = Convert(prev);
			dtostrf(val,1,3,str);

			printm("Voltage Read: ");
			printm(str);
			putChar_('\n');
			putChar_('\r');
			_delay_ms(1000);
		}
	}

}

