/*
 * Group 06.c
 *
 * Created: 12/17/2021 10:15:11 AM
 * Author : Krishantha
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include<avr/io.h>

#include<util/delay.h>

#ifndef F_CPU
#define F_CPU 1000000UL // 1 MHz clock speed
#endif

#include "Header_files/lcd.h"
#include "Header_files/keypad.h"
#include "Header_files/ultrasonic.h"
#include "Header_files/dht11.h"



int main(void)

{

	DDRA=0xff;

	DDRB=0x07;

	DDRD=0xf0;

	char key;
	char numberString[4];

	lcdint();
	
	lcd_string("HELLO! WELCOME!",14);
	_delay_ms(1200);
	lcd_line_two();
	lcd_string("from ~GROUP 06~",16);
	_delay_ms(1000);
	
	lcd_clear();
	lcd_line_one();
	lcd_string("ENTER 1 TO",10);
	lcd_line_two();
	lcd_string("INSTRUCTIONS",12);
	_delay_ms(1200);

	
	while(1)
	{
		
		lcd_clear();
		lcd_string("ENTER COMMAND:",15);
		key = scankey();
		char data[5];
		
		switch(key)
		{
			case '1':
				lcd_clear();
				lcd_string("INSTRUCTIONS",12);
				break;
			case '2':
				lcd_clear();
				lcd_line_one();
				lcd_string("ENTER PLANT",11);
				lcd_line_two();
				lcd_string("COUNT: ",7);
				key = scankey();
				if(key == '*') break;
				int plantCount = displayKey();
				
				char text[10];
				sprintf(text, "%d", plantCount);
				
				lcdcmd(0x01);
				lcd_string("PLANT COUNT IS",15);
				lcdcmd(0xC0);
				lcd_string(text,10);
				_delay_ms(1000);
				break;
				
			case '3':
				while(1) {
					
					uint16_t r;
					
					_delay_ms(100);

					lcd_clear();
					
					
					HCSR04Init();

					while(1)
					{	
						HCSR04Trigger();              
						r=GetPulseWidth();
						if(r==US_ERROR) 
						{
							lcd_clear();
							lcd_string("E!",2);
							_delay_ms(1000);
							
						}
						else
						{
							
							distance=50-(r*0.034/2.0);	
							
							
							
							if (distance != previous_distance)    
							{
								lcd_clear();
							}
							
	
							
							lcd_line_one();
							lcd_string("WATER LV =",11);
							
							itoa(distance, numberString, 10);   
							lcd_string(numberString,3);

							lcd_string("cm",2);
							
							
							previous_distance = distance;
							_delay_ms(30);
							
							if(distance<10)
							{
								lcd_line_two();
								lcd_string("WATER LOW!!!",13);
								// PORTA=(1<<PINA0);
								
							}
							if(distance>=10)
							{
								lcd_line_two();
								lcd_string("ENOUGH WATER",12);
								// PORTA=(0<<PINA0);
								
							}
							key = scankey();
							if(key == '*') break;
							_delay_ms(1200);
							
						}
					}
					
				if(key == '*') break;
				}
			break;		
			
			case '4':
				
				DDRE=0xff;
				
				
				
	
				while(1)
				{
					lcd_clear();				/* clear LCD */
					lcd_line_one();		/* enter column and row position */
					Request();				/* send start pulse */
					Response();				/* receive response */
					I_RH=Receive_data();	/* store first eight bit in I_RH */
					D_RH=Receive_data();	/* store next eight bit in D_RH */
					I_Temp=Receive_data();	/* store next eight bit in I_Temp */
					D_Temp=Receive_data();	/* store next eight bit in D_Temp */
					CheckSum=Receive_data();/* store next eight bit in CheckSum */
		
					if ((I_RH + D_RH + I_Temp + D_Temp) != CheckSum)
					{
						//lcd_gotoxy(0,0);
						lcd_string("Error",5);
					}
		
					else
					{
						itoa(I_RH,data,10);
						//lcd_gotoxy(2,0);
						lcd_string("H=",2);
						lcd_string(data,2);
						lcd_string(".",1);
			
						itoa(D_RH,data,10);
						
						lcd_string(data,2);
						lcd_string("%",1);

						
						itoa(I_Temp,data,10);
						//lcd_gotoxy(10,0);
						lcd_string("T=",2);
						lcd_string(data,2);
						lcd_string(".",1);
			
						itoa(D_Temp,data,10);
						lcd_string(data,2);
						lcddata(0xDF);
						lcd_string("C ",2);
						
						lcd_line_two();
			
						/*itoa(CheckSum,data,10);
						lcd_print(data);
						lcd_print(" ");*/
					}
		
					_delay_ms(500);
		
					if ((I_Temp + D_Temp) <= 24)
					{
						lcd_string("H ON  ",6);
			
						//PORTB=(1<<PINB0);
						//PORTB=(0<<PINB1);
						//PORTC=(0<<PINC0);
			
						//DDRB |= (1<<0);
			
			
					}
					if ((I_Temp + D_Temp) >= 30)
					{
			
			
						//lcd_gotoxy(5,1);
						lcd_string("FA ON",5);
			
						//PORTB=(0<<PINB0);
						//PORTB=(1<<PINB1);
						//PORTC=(1<<PINC0);
						//DDRB |= (1<<1);
			
					}
					if ((I_RH + D_RH ) <=75)
					{
			
						//lcd_gotoxy(11,1);
						lcd_string("FO ON",5);
						//PORTE=(1<<PINE0);
					}
		
					/*else
					{
						lcd_gotoxy(0,0);
						lcd_print("fogger off");
						PORTE=(0<<PINE0);

			
					}*/
					_delay_ms(100);
					key = scankey();
					if(key == '*') break;
					_delay_ms(1200);
		
				}	
			}
			
			
							
		}
		
}
	
	




int displayKey()
{
	int tmpNum;
	int data[10];
	int i = 0;
	int num = 0;
	while(1)
	{
		key=scankey();
		lcddata(key);
		if(key == '#') break;
		tmpNum = charToInt(key);
		data[i] = tmpNum;
		i++;
		_delay_ms(300);
	}
	int k = i-1;
	
	for(int j=0;i>j;j++){
		num += data[j]*pow(10,k);
		k--;
	}
	
	lcdcmd(0x01);
	
	return num;
}

