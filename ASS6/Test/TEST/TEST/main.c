// ---------------------------------------------------------------------------
// CpE 301
// March 6, 2017
// S. Harris
// DA6_UARTexample_withInterrupts.c
// ---------------------------------------------------------------------------
//
// This code displays the character typed by the user on the serial terminal.
//
// ---------------------------------------------------------------------------

#include <avr/io.h>
#define F_CPU 16000000UL	// Clock Speed
#include <avr/interrupt.h>

#define BAUD  9600

void initUART();

int main( void )
{
	initUART();
	while (1)									// Wait for interrupt
		;

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
	UCSR0B |= (1 << RXCIE0);						// Enable receiver interrupt
	sei();										// Enable global interrupts
}

// UART receiver interrupt handler
ISR (USART_RX_vect)
{
	unsigned char receivedChar;

	receivedChar = UDR0;							// Read data from the RX buffer
	UDR0 = receivedChar;							// Write the data to the TX buffer
}
