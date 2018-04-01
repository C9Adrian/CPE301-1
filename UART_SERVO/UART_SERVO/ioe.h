#ifndef IOE_H
#define IOE_H

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#ifndef F_CPU
#define F_CPU 16000000UL
#endif 
/*SETTINGS*/
#ifndef BAUD
#define BAUD  9600							//BAUDRATE (9600 is default)
#endif
#define BUFF	 256

#define ASYNCH_NORM	((F_CPU/16)/BAUD - 1)
#define ASYNCH_DUB	((F_CPU/8)/BAUD - 1)
#define SYNC_MASTER ((F_CPU/2)/BAUD - 1)

/*MACROS USEFUL TO DISABLE AND ENABLE*/
#define TX_START()		UCSR0B |= _BV(TXEN0)	// Enable TX
#define TX_STOP()		UCSR0B &= ~_BV(TXEN0)	// Disable TX
#define RX_START()		UCSR0B |= _BV(RXEN0)	// Enable RX
#define RX_STOP()		UCSR0B &= ~_BV(RXEN0)	// Disable RX
#define COMM_START()	TX_START(); RX_START()	// Enable communicationsF
#define COMM_STOP()		TX_STOP();RX_STOP()		// Disable Communication

//Frame Size to be Transmitted 
#define CHAR6()			UCSR0C |= _BV(UCSZ00) 
#define CHAR7()			UCSR0C |= _BV(UCSZ01) 
#define CHAR8()			UCSR0C |= _BV(UCSZ01)|_BV(UCSZ00) 
#define CHAR9()			UCSR0B |= _BV(UCSZ02);UCSR0C |= _BV(UCSZ01)|_BV(UCSZ00)

/* Interrupt macros; Remember to set the GIE bit in SREG before using (see datasheet) */
#define RX_INTEN()		UCSR0B |= _BV(RXCIE0)	// Enable interrupt on RX complete
#define RX_INTDIS()		UCSR0B &= ~_BV(RXCIE0)	// Disable RX interrupt
#define TX_INTEN()		UCSR0B |= _BV(TXCIE0)	// Enable interrupt on TX complete
#define TX_INTDIS()		UCSR0B &= ~_BV(TXCIE0)	// Disable TX interrupt

/*Stop Bit*/
#define STOPBIT_1()		UCSR0C &= (0<<USBS0)
#define STOPBIT_2()		UCSR0C |= (1<<USBS0)

/*Parity Mode*/
#define DisParity() 	UCSR0C &= ~(1<<UPM01);UCSR0C &= ~(1<<UPM00)
#define EvenParity()	UCSR0C |= (1<<UPM01)
#define OddParity()		UCSR0C |= (1<<UPM01)|(1<<UPM00)

/*MODE*/
#define ASYNCH_MODE()	UCSR0C &= ~(1<<UMSEL01);UCSR0C &= ~(1<<UMSEL00)
#define SYNCH_MODE()	UCSR0C |= (1<<UMSEL00)
#define MASTER_MODE()	UCSR0C |= (1<<UMSEL01)|(1<<UMSEL00)

/*Enable Interrupts*/
#define recInterrupt()	UCSR0B |= (1 << RXCIE0)

/*FUNCTION DECLERATION*/

/*
 * Procedure to initialize USART0 asynchronous with enabled RX/TX, 8 bit data,
 * no parity, and 1 stop bit. 
*/
void usart0_init_ ()
{
	// To set baud rate
	unsigned int baudrate = ASYNCH_NORM;
	UBRR0H = (unsigned char) ((baudrate) >> 8);	//top nibble
	UBRR0L = (unsigned char) ((baudrate)) ;		//lower byte		

	COMM_START();									// enable transmit/receive
	
	// asynchronous, 8N1, disable parity, 1 stop bit
	ASYNCH_MODE();
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
	if(data == '\n')
		putChar_('\r');
			
	while(!(UCSR0A & _BV(UDRE0)));
	UDR0 = data;
}


/*A string print called printm that uses a 
char array and your putchar clone to transmit
strings*/

void printm(char *str)
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
void scanm(uint8_t* buffer)
{
	int i = 0;
	while(buffer[i] != '\n'){
		buffer[i] = getChar_();
		if (buffer[i]=='\r')
			break;
		putChar_(buffer[i]);
		i++;
	}
	buffer[i] = '\0';
}
void print_int(int data)
{
	char datastring[30] = {0};
	sprintf(datastring, "%d", data);
	printm(datastring);
}
void print_double(double data)
{
	char datastring[30] = {0};
	sprintf(datastring, "%f", data);
	printm(datastring);
}

#endif /*IOE_H*/