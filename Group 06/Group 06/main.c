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



int main(void)

{

	DDRA=0xff;

	DDRB=0x07;

	DDRD=0xf0;

	char key;

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
				
		}
		
		
		if (key == '3')
		{
			char numberString[4];   // we re defining an array of character. It will be utilized later to store integer to be displayed on the LCD screen

			while(1) {
				
				uint16_t r;
				
				_delay_ms(100);

				lcdcmd(0x01);
				
				//Set io port direction of sensor
				HCSR04Init();

				while(1)
				{
					
					//Send a trigger pulse
					HCSR04Trigger();               // calling the ultrasonic sound wave generator function
					
					//Measure the width of pulse
					r=GetPulseWidth();             // getting the duration of the ultrasound took to echo back after hitting the object
					
					//Handle Errors
					if(r==US_ERROR)                // if microcontroller doesn't get any pulse then it will set the US_ERROR variable to -1
					// the following code will check if there is error then it will be displayed on the LCD screen
					{
						lcdcmd(0x01);
						lcdcmd(0x80);
						lcd_string("E!",2);
						_delay_ms(1000);
						lcdcmd(0x01);
					}
					else
					{
						
						distance=50-(r*0.034/2.0);	// This will give the distance in centimeters
						
						
						
						if (distance != previous_distance)    // the LCD screen only need to be cleared if the distance is changed otherwise it is not required
						{
							lcdcmd(0x01);
						}
						
						lcdcmd(0x01);
						
						lcdcmd(0x80);
						lcd_string("WATER LV =",11);
						
						itoa(distance, numberString, 10);    // distance is an integer number, we can not display integer directly on the LCD. this line converts integer into array of character
						lcd_string(numberString,3);

						lcd_string("cm",2);
						
						
						
						
						previous_distance = distance;
						_delay_ms(30);
						
						if(distance<10)
						{
							lcdcmd(0xC0);
							lcd_string("WATER LOW!!!",13);
							// PORTA=(1<<PINA0);
							
						}
						if(distance>=10)
						{
							lcdcmd(0xC0);
							lcd_string("ENOUGH WATER",12);
							// PORTA=(0<<PINA0);
							
						}
						
						_delay_ms(1200);
						
					}
				}
				
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

