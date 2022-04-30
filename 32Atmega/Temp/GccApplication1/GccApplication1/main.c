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
#include <avr/interrupt.h>
#include <string.h>
#include <stdbool.h>



#define SREG   _SFR_IO8(0x3F)

#include<avr/io.h>

#include<util/delay.h>

#ifndef F_CPU
#define F_CPU 8000000UL // 1 MHz clock speed
#endif

#include "lcd.c"
#include "keypad.c"


int ldrconfig();
void get_plantcount();
void water_level();
void dht11_output();
int displayKey();

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
		displayKey();
		get_plantcount();
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


void get_plantcount(){
	lcd_clear();
	lcd_line_one();
	lcd_string("ENTER PLANT",11);
	lcd_line_two();
	lcd_string("COUNT: ",7);
	key = scankey();
	//if(key == '*') break;
	int plantCount = displayKey();
	
	char text[10];
	sprintf(text, "%d", plantCount);
	
	lcdcmd(0x01);
	lcd_string("PLANT COUNT IS",15);
	lcdcmd(0xC0);
	lcd_string(text,10);
	_delay_ms(1000);
}
