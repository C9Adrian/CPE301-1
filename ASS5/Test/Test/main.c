/*
 * Test.c
 *
 * Created: 3/2/2017 11:04:12 AM
 * Author : Luis
 */ 
#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

// Connect LCD data pins to PORTB and control pins to PORTC
// RS = PC.0
// RW = PC.1
// EN = PC.2
#define LCD_DPRT PORTD //LCD DATA PORT 
#define LCD_DDDR DDRD  //LCD DATA DDR 
#define LCD_DPIN PIND  //LCD DATA PIN  
#define LCD_CPRT PORTC //LCD COMMANDS PORT 
#define LCD_CDDR DDRC  //LCD COMMANDS DDR 
#define LCD_CPIN PINC  //LCD COMMANDS PIN 
#define LCD_RS  0      //LCD RS   (PC.0)
#define LCD_RW  1      //LCD RW   (PC.1)
#define LCD_EN  2      //LCD EN   (PC.2)


//**************************************************************** 
void lcdCommand (unsigned char cmd) {  
	LCD_DPRT = cmd;				//send cmd to data port  
	LCD_CPRT &= ~(1<<LCD_RS);   //RS = 0 for command  
	LCD_CPRT &= ~(1<<LCD_RW);   //RW = 0 for write   
	LCD_CPRT |= (1<<LCD_EN);    //EN = 1 for H-to-L pulse  
	_delay_us(1);					//wait to make enable wide  
	LCD_CPRT &= ~(1<<LCD_EN);   //EN = 0 for H-to_L pulse  
	_delay_us(100);				//wait to make enable wide 
} 
 
void lcdData(unsigned char data) {  
	LCD_DPRT = data;				//send data to data port  
	LCD_CPRT |= (1<<LCD_RS);		//RS = 1 for data  
	LCD_CPRT &= ~(1<<LCD_RW);   //RW = 0 for write   
	LCD_CPRT |= (1<<LCD_EN);		//EN = 1 for H-to-L pulse  
	_delay_us(1);					//wait to make enable wide  
	LCD_CPRT &= ~(1<<LCD_EN);   //EN = 0 for H-to_L pulse  
	_delay_us(100);				//wait to make enable wide  
} 

void lcd_init() {  
	LCD_DDDR = 0xFF;  
	LCD_CDDR = 0xFF;    
	LCD_CPRT &=~(1<<LCD_EN);   //LCD_EN = 0  
	_delay_us(2000);    
	//wait for init  
	lcdCommand(0x38);   //initialize LCD 2 line, 5x7  
	lcdCommand(0x0E);   //display on, cursor on  
	lcdCommand(0x01);   //clear LCD  
	_delay_us(2000);		 //wait  
	lcdCommand(0x06);   //shift cursor right 
} 
 
void lcd_print(char * str) {  
	unsigned char i = 0;  
	
	while (str[i]!=0)  {   
		lcdData(str[i]); i++;  
	}
} 

// go to specific LCD locations
void lcd_gotoxy(unsigned char x, unsigned char y) {  
	unsigned char firstCharAdr[] = {0x80, 0xC0};   // locations of the first character of each line
	
	lcdCommand(firstCharAdr[y-1] + x-1);  
	_delay_us(100); 
} 


int main(void) {  
	// initialize LCD
	lcd_init();			  
	lcd_print("Demo code");		// print some sample code on LCD
	_delay_ms(500);

	// clear the LCD and print some more text
	lcdCommand(0x01);   //clear LCD
	lcdCommand(0x02);   // return home: returns the cursor to the home position
	lcd_print("Here is some more text that overruns a single line...");		// print text that overruns a single line
	_delay_ms(500);

	// print text at a specific location on LCD (1st line, then 2nd line)
	lcd_gotoxy(1,1);  
	lcd_print("Text for line 1.");  
	//lcd_gotoxy(1,2);  
	//lcd_print("Text for line 2.");   
	
	while(1);    //stay here forever  
	return 0; 
}   

