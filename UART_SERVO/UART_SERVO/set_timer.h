#ifndef _SET_TIMER_H
#define _SET_TIMER_H

#include <avr/io.h>
/*
	THESE MACROS JUST SETUP THE TIMER/COUNTERx 
	to a desired mode with a desired prescale.OCxR mode 
	of toggling should be set manually. 

	Interrupts should also be dealt manually
*/

/*Timer/Counter 0!!!!!!! */ 
//MODE OF OPERATION TC0

//NORMAL
void TC0_NORM()
{
	TCCR0B &= ~(1<<WGM02); 
	TCCR0A &= ~(1<<WGM01);
	TCCR0A &= ~(1<<WGM00);
}
//PWM,PHASE CORRECT TOP = 0XFF
void TC0_PWM_PC_D_TOP() 
{
	TCCR0B &= ~(1<<WGM02); 
	TCCR0A &= ~(1<<WGM01);
	TCCR0A |= (1<<WGM00);
}
//CTC
void TC0_CTC()
{
	TCCR0B &= ~(1<<WGM02); 
	TCCR0A |= (1<<WGM01); 
	TCCR0A &= ~(1<<WGM00);
}
//FAST PWM TOP = 0XFF
void TC0_FAST_PWM_D_TOP() 	
{
	TCCR0B &= ~(1<<WGM02); 
	TCCR0A |= (1<<WGM01)| (1<<WGM00);
} 
//PWM PHASE CORRECT TOP = OCRA
void TC0_PWM_S_TOP()
{
	TCCR0B |= (1<<WGM02); 
	TCCR0A &= ~(1<<WGM01);
	TCCR0A |= (1<<WGM00);
}
//FAST PWM TOP = OCRA
void TC0_FAST_PWM_S_TOP() 	
{
	TCCR0B |= (1<<WGM02); 
	TCCR0A |= (1<<WGM01)|(1<<WGM00);
} 

// //Prescaling
// //Left out External clock source on T0 pin 
// //falling and rising edge

//CLOCK SELECT BITS 

//NO CLOCK SOURCE
void TC0_STOP() 
{
	TCCR0B &= ~(1<<CS02);
	TCCR0B &= ~(1<<CS01);
	TCCR0B &= ~(1<<CS00);
}

//NO PRESCALE
void TC0_NOPRESCALING()
{
	TCCR0B &= ~(1<<CS02);
	TCCR0B &= ~(1<<CS01);
	TCCR0B |= (1<<CS00);
}

//F_CPU/8
void TC0_PRESCALING_8() 	
{
	TCCR0B &= ~(1<<CS02);
	TCCR0B |= (1<<CS01);
	TCCR0B &= ~(1<<CS00);
}
//F_CPU/64
void TC0_PRESCALING_64() 	
{
	TCCR0B &= ~(1<<CS02);
	TCCR0B |= (1<<CS01);
	TCCR0B |= (1<<CS00);
}
//F_CPU/256
void TC0_PRESCALING_256() 	
{
	TCCR0B |= (1<<CS02);
	TCCR0B &= ~(1<<CS01);
	TCCR0B &= ~(1<<CS00);
}
//F_CPU/1024
void TC0_PRESCALING_1024() 
{
	TCCR0B |= (1<<CS02);
	TCCR0B &= ~(1<<CS01);
	TCCR0B |= (1<<CS00);
}

// /*Timer/Counter 1!!!!!!! */ 
// // I left out the situation where it compares to the output compare reg
// //OCR1A

//MODES OF OPERATIONS

//NORMAL
void TC1_NORM()		   	   
{
	TCCR1B &= ~(1<<WGM13);
	TCCR1B &= ~(1<<WGM12);
	TCCR1A &= ~(1<<WGM11);
	TCCR1A &= ~(1<<WGM10);
}
//PWM, PHASE CORRECT TOP = 0X00FF
void TC1_PWM_PC_D_TOP_8()    
 {
	TCCR1B &= ~(1<<WGM13);
	TCCR1B &= ~(1<<WGM12);
	TCCR1A &= ~(1<<WGM11);
	TCCR1A |= (1<<WGM10);
}
//TOP = 0X01FF
void TC1_PWM_PC_D_TOP_9()     
{
	TCCR1B &= ~(1<<WGM13);
	TCCR1B &= ~(1<<WGM12);
	TCCR1A |= (1<<WGM11); 
	TCCR1A &= ~(1<<WGM10);
}
//TOP = 0X03FFF
void TC1_PWM_PC_D_TOP_10()	 
{ 
	TCCR1B &= ~(1<<WGM13);
	TCCR1B &= ~(1<<WGM12);
	TCCR1A |= (1<<WGM11)|(1<<WGM10);
}
//CTC
void TC1_CTC()				  
{ 
	TCCR1B &= ~(1<<WGM13);
	TCCR1B |= (1<<WGM12);
	TCCR1A &= ~(1<<WGM11);
	TCCR1A &= ~(1<<WGM10);
}
//FAST PWM TOP = 0X00FF
void TC1_FAST_PWM_D_TOP_8b()  
{
	TCCR1B &= ~(1<<WGM13);
	TCCR1B |= (1<<WGM12);
	TCCR1A &= ~(1<<WGM11);
	TCCR1A |= (1<<WGM10);
}
//TOP = 0X01FF
void TC1_FAST_PWM_D_TOP_9b()  
{
	TCCR1B &= ~(1<<WGM13);
	TCCR1B |= (1<<WGM12);
	TCCR1A |= (1<<WGM11);
	TCCR1A &= ~(1<<WGM10);
}
//TOP = 0X03FF
void TC1_FAST_PWM_D_TOP_10b()
{
 	TCCR1B &= ~(1<<WGM13);
 	TCCR1B |= (1<<WGM12);
 	TCCR1A |= (1<<WGM11);
 	TCCR1A |= (1<<WGM10);
}
//PWM,PHASE CORRECT TOP = ICR1
void TC1_PWM_PC_S_TOP()       
{
	TCCR1B |= (1<<WGM13);
	TCCR1B &= ~(1<<WGM12);
	TCCR1A &= ~((1<<WGM11)|(1<<WGM10));
}
//FAST PWM TOP = ICR1
void TC1_FAST_PWM_S_TOP()     
{
	TCCR1B |= (1<<WGM13);
	TCCR1B |= (1<<WGM12);
	TCCR1A |= (1<<WGM11); 
	TCCR1A &= ~(1<<WGM10);
}

// //Prescaling

//STOP TC1
void TC1_STOP()         	 
{
	TCCR1B &= ~((1<<CS12)|(1<<CS11)|(1<<CS10));
}
//NO Prescale
void TC1_NOPRESCALING() 	 
{
	TCCR1B &= ~((1<<CS12)|(1<<CS11));
	TCCR1B |= (1<<CS10);
}
//F_CPU/8
void TC1_PRESCALING_8() 	 
{
	TCCR1B &= ~(1<<CS12);
	TCCR1B |= (1<<CS11);
	TCCR1B &= ~(1<<CS10);
}
//F_CPU/64
void TC1_PRESCALING_64()	 
{
	TCCR1B &= ~(1<<CS12);
	TCCR1B |= (1<<CS11);
	TCCR1B |= (1<<CS10);
}
//F_CPU/256
void TC1_PRESCALING_256() 	
{
	TCCR1B |= (1<<CS12);
	TCCR1B &= ~((1<<CS11)|(1<<CS10));
}
//F_CPU/1024
void TC1_PRESCALING_1024()   
{
	TCCR1B |= (1<<CS12);
	TCCR1B &= ~(1<<CS11);
	TCCR1B |= (1<<CS10);
}

/*
	If a settop is 0 then just set up timer with no top value else
	set the stop value
*/
void initateTimer1()
{																			
	
	  /*Example*/
	  //TOP = (F_cpu/(Focnx*N))-1
	  //Desire Focnx = 50 Hz
	  //ICR1 = (uint_8)((16e6/(50*64))-1);
	  ICR1 = 4999;	

	  //SET Timer 1 to have the top to be ICR1
	  //FAST PWM reading the OCRA in non-inverting mode
	  //A prescalar of 64 => 16MHz / 64

	  //ex: of bits being set
	  //TCCR1A = (1<<COM1A1)|(1<<COM1B1)|(1<<WGM11);
	  //TCCR1B = (1<<WGM13)|(1<<WGM12)|(1<<CS11)|(1<<CS10);

	  //OC1A AND OC1B as outputs for PWM
	  DDRB |= (1<<PB1)|(1<<PB2);

	  TCCR1A = (1<<COM1A1)|(1<<COM1B1);
	  TC1_FAST_PWM_S_TOP();
	  TC1_PRESCALING_64();
 }



#endif 