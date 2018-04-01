/*
 * lcd.h
*/

#ifndef LDC_H
#define LDC_H

#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

// Connect LCD data pins to PORTB and control pins to PORTC
// RS = PC.0
// RW = PC.1
// EN = PC.2 
#define LCD_CPRT PORTC //LCD COMMANDS PORT 
#define LCD_CDDR DDRC  //LCD COMMANDS DDR 
#define LCD_CPIN PINC  //LCD COMMANDS PIN 
#define LCD_RS  0      //LCD RS   (PC.0)
#define LCD_RW  1      //LCD RW   (PC.1)
#define LCD_EN  2      //LCD EN   (PC.2)

//**************************************************************** 
//Function Declarations 

/*
 * Send a command to the LCD, make pin RS = 1 and R/W = 0;
 * Send a H-to-L pulse to the E pin to enable the internal
 * latch. Place command in command Reg (output AVR: PortB.0...7)
*/
void lcdCommand (unsigned char cmd){
	sendData(cmd);				//send cmd to data port
	LCD_CPRT &= ~(1<<LCD_RS);   //RS = 0 for command
	LCD_CPRT &= ~(1<<LCD_RW);   //RW = 0 for write
	LCD_CPRT |= (1<<LCD_EN);    //EN = 1 for H-to-L pulse
	_delay_us(1);					//wait to make enable wide
	LCD_CPRT &= ~(1<<LCD_EN);   //EN = 0 for H-to_L pulse
	_delay_us(100);				//wait to make enable wide
}

/*
 * Send data to the LCD, make pin RS = 0 and R/W = 0;
 * Send a H-to-L pulse to the E pin to enable the internal
 * latch. Place data in Data Reg (output AVR: PortB.0...7)
*/
void lcdData(unsigned char data){		
		sendData(data);					//send data to data port
		LCD_CPRT |= (1<<LCD_RS);		//RS = 1 for data
		LCD_CPRT &= ~(1<<LCD_RW);   //RW = 0 for write
		LCD_CPRT |= (1<<LCD_EN);		//EN = 1 for H-to-L pulse
		_delay_us(1);					//wait to make enable wide
		LCD_CPRT &= ~(1<<LCD_EN);   //EN = 0 for H-to_L pulse
		_delay_us(100);				//wait to make enable wide
}

/*
 * Initiate the LCD 
*/
void lcd_init()
{
	DDRB = 0xFF;
	LCD_CDDR = 0xFF;			
	LCD_CPRT &=~(1<<LCD_EN);   //LCD_EN = 0
	_delay_us(2000);
	//wait for init
	lcdCommand(0x38);   //initialize LCD 2 line, 5x7
	lcdCommand(0x0E);   //display on, cursor on
	_delay_us(2000);		 //wait
	lcdCommand(0x06);   //shift cursor right
}
/*
 * Print on to the LCD the 8-bit char value 
 * lcdData is used to send the data
*/
void lcd_print(char * str){
	unsigned char i = 0;
	
	while (str[i]!=0)  {
		lcdData(str[i]); i++;
	}
}

/*
 * go to specific LCD locations
*/ 
void lcd_gotoxy(unsigned char x, unsigned char y){
	unsigned char firstCharAdr[] =  {0x80, 0xC0, 0x94, 0xD4};// locations of the first character of each line
	
	lcdCommand(firstCharAdr[y-1] + x-1);
	_delay_us(100);
}

/*
 * Clear the LCD and reset the cursor to the home position 
*/
void lcd_reset(){
	lcdCommand(0x01);   //clear LCD
	lcdCommand(0x02);   // return home: returns the cursor to the home position
	_delay_ms(500);
}

//send data using portb and portc 
void sendData(unsigned char str){

	PORTB = 0X0;
	PORTC &= ~(1<<PC4); PORTC &= ~(1<<PC5);

	if((str & 0x01) == 0)
		PORTB|= (0<<PB0);
	else
		PORTB|= (1<<PB0);

	if((str & 0x02) == 0)
		PORTB|= (0<<PB1);
	else
		PORTB|= (1<<PB1);

	if((str & 0x04) == 0)
		PORTB|= (0<<PB2);
	else
		PORTB|= (1<<PB2);

	if((str & 0x08) == 0)
		PORTB|= (0<<PB3);
	else
		PORTB|= (1<<PB3);

	if((str & 0x10) == 0)
		PORTB|= (0<<PB4);
	else
		PORTB|= (1<<PB4);

	if((str & 0x20) == 0)
		PORTB|= (0<<PB5);
	else
		PORTB|= (1<<PB5);

	if((str & 0x40) == 0)
		PORTC|= (0<<PC4);
	else
		PORTC|= (1<<PD4);

	if((str & 0x80) == 0)
		PORTC|= (0<<PC5);
	else
		PORTC|= (1<<PD5);
}

//Test the LCD Functionality  
int lcd_test(){
	lcd_init();
	lcd_print("Demo code");		// print some sample code on LCD
	_delay_ms(1000);

	// clear the LCD and print some more text
	lcdCommand(0x01);   //clear LCD
	lcdCommand(0x02);   // return home: returns the cursor to the home position
	lcd_print("Here is some more text that overruns a single line...");		// print text that overruns a single line
	_delay_ms(1000);

	// print text at a specific location on LCD (1st line, then 2nd line)
	lcdCommand(0x01);   //clear LCD
	lcdCommand(0x02);   // return home: returns the cursor to the home position
	_delay_ms(1000);
	lcd_gotoxy(1,1);
	lcd_print("Text for line 1.");
	lcd_gotoxy(1,2);
	lcd_print("Text for line 2.");
	while(1);			//stay here forever
	return 0;
}

#endif
