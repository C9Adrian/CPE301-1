// -------------------------------------------------------------------
// CpE 301
// Spring 2017
// S. Harris
// DA6_UARTexample.c
// -------------------------------------------------------------------
//
// This code repeatedly writes the alphabet to the serial terminal
//
// --------------------------------------------------------------------

#include <avr/io.h>
#define F_CPU 8000000	// Clock Speed
#include <util/delay.h>

#define BAUD  9600

void initUART();
void writeChar(unsigned char c);


int main( void )
{
	unsigned char ch = 'A';

	initUART();
	while (1) {
	  writeChar(ch);
	  ch++;
	  if (ch > 'Z')
	    ch = 'A';
	}

	return 0;	
}

void initUART() {
	unsigned int baudrate;

	// Set baud rate:  UBRR = [F_CPU/(16*BAUD)] -1
	baudrate = ((F_CPU/16)/BAUD) - 1;
	UBRR0H = (unsigned char) (baudrate >> 8);
	UBRR0L = (unsigned char) baudrate;

	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);		// Enable receiver and transmitter
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);		// Set data frame: 8 data bits, 1 stop bit, no parity
}

void writeChar(unsigned char c) {
	UDR0 = c;									// Display character on serial (i.e., PuTTY) terminal
	_delay_ms(200);
}
