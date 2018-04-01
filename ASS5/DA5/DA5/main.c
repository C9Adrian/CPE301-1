/*
 * DA5.c
 *
 * Created: 3/1/2017 10:19:00 PM
 * Author : Luis
 */ 

#include "lcd.h"
#include <avr/io.h>

//KEYPAD PINS	 
#define KEYPORT PORTD
#define KEYDDR DDRD
#define KEYPIN PIND

//SET OUTUPT signals

/* Had to map it out differently based on the
// the data sheet

//How it looks on the keypad interface
{
	{'1','2','3','A'},
	{'4','5','6','B'},
	{'7','8','9','C'},
	{'*','0','#','D'}
}
*/

unsigned char keypad[4][4] ={
							{'1','4','7','*'},
							{'2','5','8','0'},
							{'3','6','9','#'},
							{'A','B','C','D'}
							};

int main(void)
{	
		lcd_init();
		lcd_reset();
		lcd_print("Input Pin");

		unsigned char col,row = 0;
		unsigned char count = 0; 
		unsigned char pin[4] = {0};

		while(1)
		{
			start:
				if(count == 0)
				{

					lcd_init();
					lcd_reset();
					lcd_print("Input Pin");
					lcd_gotoxy(1,2);
					col = row = 0;
					pin[4] = pin[3] = pin[2]= pin[1]= 0;
				}

				//Establish the Inputs and outputs
				//to the keypad
				KEYDDR =  0xF0;
				KEYPORT = 0xFF;


			/*
			 * Go in at least once,
			 * Then PORT7...4 are set low and wait until the pad pressed
			 * released. 

			*/
			do 
			{
				KEYPORT &= 0x0F;
				col = (KEYPIN & 0x0F); 

			}while(col != 0x0F);


			/*
			 * De-bouncing 
			*/
			 do
			 {
				 do
				 {
					 _delay_ms(20);					//call delay
					 col = (KEYPIN & 0x0F);			//see if any key is pressed
				 } while (col == 0x0F);				//keep checking for key press

				 _delay_ms(20);						//call delay for de-bounce
				 col = (KEYPIN & 0x0F);				//read columns
			 } while (col == 0x0F);					//wait for key press

			while(1)
			{
				 KEYPORT = 0xEF;		 //ground row 0
				 _delay_ms(20);
				 col = (KEYPIN & 0x0F);	 //read the columns
				 if (col != 0x0F)		 //column detected
				 {
					 row = 0;			 //save row location
					 break;				 //exit while loop
				 }
				 KEYPORT = 0xDF;		 //ground row 1
				 _delay_ms(20);
				 col = (KEYPIN & 0x0F);  //read the columns
				 if(col != 0x0F)		 //column detected
				 {
					 row = 1;			 //save row location
					 break;				 //exit while loop
				 }
				 KEYPORT = 0xBF;		//ground row 2
				 _delay_ms(20);
				 col = (KEYPIN & 0x0F); //read the columns
				 if(col != 0x0F)		//column detected
				 {
					 row = 2;		//save row location
					 break;			//exit while loop
				 }

				 KEYPORT = 0x7F;		//ground row 3
				 _delay_ms(20);
				 col = (KEYPIN & 0x0F); //read the columns
				if(col!= 0x0F)
				 {
					row = 3;				//save row location
					break;					//exit while loop
				}

			}

			if(count >= 3)
			{
				for(int i = 0; i < 3; i = i +1)
				{
					switch(i)
					{
					 case 0: if(pin[i] != '5'){
								lcdData(pin);
								_delay_ms(1000);
								printError(pin);
								count = 0;
								goto start;
							}
							 break;
					 case 1:if(pin[i] != '2'){
								 lcdData(pin);
								 _delay_ms(1000);
								printError(pin);
								count = 0;
								goto start;
							}
							break;
						
					 case 2:if(pin[i] != '7'){
								lcdData(pin);
								_delay_ms(1000);
								printError(pin);
								count = 0;
								goto start;
							}
							else
								Correct(pin);
							break;
					default:count = 0;goto start;break;
								
					}
				}

				count = 0;
				goto start;
			}
			

			 //check column and send result to Port D
			 if(col == 0x0E)
				 pin[count] = (keypad[row][0]);
			 else if(col == 0x0D)
				pin[count] = (keypad[row][1]);
			 else if(col == 0x0B)
				 pin[count] = (keypad[row][2]);
			 else
				pin[count] = (keypad[row][3]);

			if(count < 3)
			{
				lcdData(pin[count]);
				_delay_ms(500);
			}	
			 ++count;						
		}
		return 0;
}

void printError(char *str)
{
	lcd_init();
	lcd_reset();
	lcd_print(str);
	lcd_gotoxy(1,2);
	lcd_print("Wrong Pin!!");
	_delay_ms(2500);
}

void Correct(char *str){

	lcd_init();
	lcd_reset();
	lcd_print(str);
	lcd_gotoxy(1,2);
	//“System unlocked”
	lcd_print("Access Granted!!");
	//_delay_ms(2500);
	while(1);
}