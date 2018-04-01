/*
* UART_SERVO.c
*
* Created: 3/28/2017 8:56:14 PM
* Author : Luis
*/ 



#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#include "ioe.h"
#include "set_timer.h"
/*	
	TC1 Clk = 16e6/64 = 250000UL (4us period)
	3001HB: SERVO_MIN 97(388us/4us), SERVO MAX 535 (2140us/4us)
	TGY-R5180MG: MAX RANGE 595(2380us) MIN RANGE 177 (708us)
		Values Used: SERVO_MIN will be our 0 deg and SERVO MAX will be our 180 deg 
 */
 #define SERV_MIN 245	
 #define SERV_MAX 505



 volatile int count = 0;
 volatile int digit = 0;
 unsigned char servo;
 unsigned char on[3] = {0};
 unsigned char position[6] = {0};

void setLedPorts();
int getServoposition(int);
void resetBuff();
void changeLedState(int servoNum);
int getAngle(int val);
int getInt(unsigned char *position);

int main(void)
{	
    //initate Timer0 
	initateTimer1();	
	//intial value 		
	OCR1A = SERV_MIN;
	OCR1B = SERV_MIN;	

	//set ports used to output high or low
	setLedPorts();

	//initate UART0		
	usart0_init_();				

	//enable RCX interrupt
	recInterrupt();				
	//enable global interrupts
	sei();						

	while(1);
	return 0;
}

 //read input 
 ISR(USART_RX_vect)
{
	unsigned char receivedChar;
	receivedChar = UDR0;							// Read data from the RX buffer
	putChar_(receivedChar);
	_delay_ms(2);

	if(count == 0){
		unsigned char temp = receivedChar - 0x30;
		switch(temp){
			case 1:servo = 0;++count;break;
			case 2:servo = 1;++count;break;
			case 3:servo = 2;++count;break;
			default:break;printm("\r\nPlease enter a valid servo");break;
		}
		//printm(":Servo  ");
	}
	else if(count == 1){
		unsigned char temp = receivedChar - 0x30;
		if(temp == 0){
			switch(servo){
				case 0:	on[servo] = 0;changeLedState(servo);break;
				case 1: on[servo] = 0;changeLedState(servo);break;
				case 2: on[servo] = 0;changeLedState(servo);break;
				default: printm("NO CHANGE");break;
			}
		}
		else{
				switch(servo){
				case 0:	on[servo] = 1;changeLedState(servo);break;
				case 1: on[servo] = 1;changeLedState(servo);break;
				case 2: on[servo] = 1;changeLedState(servo);break;
				default: printm("NO CHANGE");break;
			}
		}
		++count;
	}
	else if(count == 2){
	
		if(digit >= 3){
			
			printm ("Value Read: ");
			printm (position);

			int pwmVal;
			pwmVal = getServoposition((getInt(position)));

			switch(servo){
				case 0:{
					if(pwmVal < SERV_MIN)
						OCR1A = SERV_MIN;
					else if(pwmVal > SERV_MAX)
						OCR1A = SERV_MAX;
					else
						OCR1A = pwmVal;
					break;
				}
				case 1:{
					if(pwmVal < SERV_MIN)
						OCR1B = SERV_MIN;
					else if(pwmVal > SERV_MAX)
						OCR1B = SERV_MAX;
					else
						OCR1B = pwmVal;
					break;
				}

				//currently only have 2 servos
				case 2: {
					if(pwmVal < SERV_MIN){
						OCR1B = SERV_MIN;
						OCR1A = SERV_MIN;
					}
					else if(pwmVal > SERV_MAX){
						OCR1B = SERV_MAX;
						OCR1A = SERV_MAX;
					}
					else{
						OCR1B = pwmVal;
						OCR1A = pwmVal;
					}
					break;
				}
			}
			
			
			printm("\r\n");
			resetBuff(position);
			count = 0;
			digit = 0;
	 	}
	 	else {
			position[digit] = receivedChar ;
	 		++digit;
		}
	}

}

int getServoposition(int val){
	int servo = 0;
	servo = (int) ((((float)val)/180.0)*260.0)+245.0;
	return servo;
}

void resetBuff(unsigned char *buff){
	for(int i = 0; i < 6; ++i)
		buff[i] = 0;
}

void setLedPorts(){
	DDRB |= (1<<PB4)|(1<<PB5);
	for(int i = 0; i < 10; ++i ){
		PORTB ^= (1<<PB4)|(1<<PB5);
	}

	PORTB &= ~(1<<PB4)|~(1<<PB5); 
}
void changeLedState(int servoNum){

	switch(servoNum){
		case 0: 
			if(on[servoNum])
				PORTB |= (1<<PB4);
			else 
				PORTB &= ~(1<<PB4); 
		break;

		case 1: 
			if(on[servoNum])
				PORTB |= (1<<PB5);
			else
				PORTB &= ~(1<<PB5);
		break;

		case 2:	
			if(on[servoNum])
				PORTB |= (1<<PB4)|(1<<PB5);
			else
				PORTB &= ~((1<<PB4)|(1<<PB5));  
		default: break; 
	}
}
int getInt(unsigned char *position)
{
	int sum = 0;
	sum =  (position[0]-0x30)*100;
	sum += (position[1]-0x31)*10;
	sum += (position[2]-0x30);
	return sum;
}
int getAngle(int val)
{
	int num = 0;
	num = 65 + ((float)val/5.8);
	return num;
}
