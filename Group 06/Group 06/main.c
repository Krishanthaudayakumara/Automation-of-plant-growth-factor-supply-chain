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

#include "Header_files/USART_Interrupt.h"				/* include USART Header file */
#include "Header_files/USART_Interrupt.c"

#define SREG   _SFR_IO8(0x3F)


#include <util/delay.h>

#ifndef F_CPU
#define F_CPU 8000000UL // 1 MHz clock speed
#endif

#include "Header_files/lcd.h"
#include "Header_files/keypad.h"
#include "Header_files/ultrasonic.h"
#include "Header_files/dht11.h"
#include "Header_files/ldr.h"
#include "Header_files/gsm.h"

int ldrconfig();
void get_plantcount();
void water_level();
void dht11_output();

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
				get_plantcount();
				break;
				
			case '3':
				water_level();
			break;		
			
			case '4':
				dht11_output();
			break;
				
			case '5':
				ldrconfig();
			break;
			
			case '6':
				buffer_pointer = 0;
				int is_msg_arrived;
				memset(message_received, 0, 60);
				lcdint();
				USART_Init(9600);						    /* initialize USART communication */
				sei();
				lcd_line_one();
				lcd_string("GSM Initializing",16);
				_delay_ms(3000);
				lcd_clear();
				lcd_string("AT",2);
				GSM_Begin();								/* check GSM responses and initialize GSM */
				lcd_clear();
				
				for(int a=0;a<2;a++){
					GSM_Send_Msg("+94773229902","Hi");
					lcd_string("SD",2);
					_delay_ms(2000);
					lcd_clear();
				}
				break;
			
			default:
			break;
			
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


int ldrconfig()
{
	uint16_t adc_result0;
	//int i;
	//int ldr;
	float i, ldr, temp;
	char buffer[10];
	//DDRC = 0x01;           // to connect led to PC0
	
	// initialize adc and lcd
	adc_init();
	lcd_clear();

	
	_delay_ms(5);
	
	while(1)
	{
		adc_result0 = adc_read(0);
		i=(adc_result0*0.01/2.1);
		ldr = (i*10.0/(5.0-i));     // read adc value at PA0
		

		//i=(adc_result0*0.01/2.1);
		//ldr = (i*10/(5-i));
		
		lcd_line_one();
		lcd_string("LDR VAL:",9);
		itoa(ldr,buffer,10);   //display ADC value
		lcd_string(buffer,5);
		
		_delay_ms(10);

		
		// condition for led to glow
		/*if (adc_result0 < LTHRES){
			PORTC = 0x00;
		}
		else{
			PORTC = 0x01;
		}
		*/
	}
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

void water_level(){
	char numberString[4];
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
}

void dht11_output(){
	
	char data[5];
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
			lcd_string("FA ON  ",7);
			
			//PORTB=(0<<PINB0);
			//PORTB=(1<<PINB1);
			//PORTC=(1<<PINC0);
			//DDRB |= (1<<1);
			
		}
		if ((I_RH + D_RH ) <=75)
		{
			
			//lcd_gotoxy(11,1);
			lcd_string("FO ON  ",7);
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