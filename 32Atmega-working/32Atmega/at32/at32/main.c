
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

#include "include/lcd.c"
#include "include/keypad.c"
#include "include/ultrasonic.c"
#include "include/ultrasonic_2.c"
#include "include/dht11.c"
#include "include/ldr.c"
#include "include/spi_master.c"
#include "include/functions.c"
#include "include/I2C_Master_H_file.h"
#include "include/I2C_Master_C_file.c"

int ldrconfig();
void get_plantcount();
void water_level();
void nt_level();
void dht11_output();
void displayTime();


int main(void)

{

	DDRA=0xff;
	DDRB=0x07;
	DDRD=0xf0;

	char buffer[5];
	char key;
	int wtrperPlant, fertilizerPlant, humMin = 0, humMax, temprtMin, temprtMax;
	

	lcdint();
	
	SPImstr_init();
	/*
	lcd_string("HELLO! WELCOME!",14);
	_delay_ms(1200);
	lcd_line_two();
	lcd_string("from ~GROUP 06~",16);
	_delay_ms(1000);
	
	lcd_clear();
	lcd_line_one();
	lcd_string("ENTER 1 TO",10);
	lcd_line_two();
	lcd_string("Configure",12);
	_delay_ms(1200);
	*/
	
	lcd_clear();
	lcd_line_one();
	lcd_string("ENTER 2 TO",10);
	lcd_line_two();
	lcd_string("Activate system",16);
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
				get_plantcount();
				wtrperPlant = waterPerPlant();
				fertilizerPlant = fertilizePerPlant();
				humMin = humidityMin();
				humMax = humidityMax();
				temprtMin = tempMin();
				temprtMax = tempMax();

				break;
			case '2':
				
			while(1){
				if(!humMin == 0) {
					dht11_output(humMin,humMax,temprtMin,temprtMax);

				} else dht11_output(75,85,23,29);
				water_level();
				_delay_ms(500);
				nt_level();
				_delay_ms(1000);
				displayTime();
				_delay_ms(200);

			}
				break;
				
			case '3':
			break;		
			
			case '4':
			break;
				
			case '5':
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
	uint8_t spiSignal = 2;

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
					_delay_ms(500);
					SPI_write(spiSignal);
					break;

					// PORTA=(1<<PINA0);
					
				}
				if(distance>=10)
				{
					lcd_line_two();
					lcd_string("ENOUGH WATER",12);
					break;
					// PORTA=(0<<PINA0);
					
				}
				key = scankey();
				break;
				_delay_ms(1200);
				
			}
			break;
		}
		
		break;
	}
}


void nt_level(){
	char numberString[4];
	uint8_t spiSignal = 3;

	while(1) {
		
		uint16_t r;
		
		_delay_ms(100);

		lcd_clear();
		
		
		HCSR04Init_2();

		while(1)
		{
			HCSR04Trigger_2();
			r=GetPulseWidth_2();
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
				lcd_string("NT LV =",8);
				
				itoa(distance, numberString, 10);
				lcd_string(numberString,3);

				lcd_string("cm",2);
				
				
				previous_distance = distance;
				_delay_ms(30);
				
				if(distance<10)
				{
					lcd_line_two();
					lcd_string("NT LEVEL LOW!!!",15);
					_delay_ms(500);
					SPI_write(spiSignal);
					break;

					// PORTA=(1<<PINA0);
					
				}
				if(distance>=10)
				{
					lcd_line_two();
					lcd_string("ENOUGH NT LEVEL",16);
					break;
					// PORTA=(0<<PINA0);
					
				}
				key = scankey();
				break;
				_delay_ms(1200);
				
			}
			break;
		}
		
		break;
	}
}

void dht11_output(int humMin, int humMax, int temprtMin, int temprtMax){
	
	char data[5];
	DDRC=0xff;
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
		
		if ((I_Temp + D_Temp) <= temprtMin)
		{
			lcd_string("H ON  ",6);
			SPI_write((uint8_t)71);
			//PORTB=(1<<PINB0);
			//PORTB=(0<<PINB1);
			//PORTC=(0<<PINC0);
			
			//DDRB |= (1<<0);
			
			
		} else {
			SPI_write((uint8_t)70);
		}
		
		
		
		if ((I_Temp + D_Temp) >= temprtMax)
		{
			
			
			//lcd_gotoxy(5,1);
			lcd_string("FA ON  ",7);
			SPI_write((uint8_t)4);
			//PORTB=(0<<PINB0);
			//PORTB=(1<<PINB1);
			//PORTC=(1<<PINC0);
			//DDRB |= (1<<1);
			
	}else{
			SPI_write((uint8_t)5);

	}
		if ((I_RH + D_RH ) <= humMax)
		{
			
			//lcd_gotoxy(11,1);
			lcd_string("FO ON ",7);
			_delay_ms(1000);
			SPI_write((uint8_t)61);
			//PORTE=(1<<PINE0);
		}
		
		else 
		{
			
			lcd_string("FO OFF",7);
			_delay_ms(1000);
			SPI_write((uint8_t)60);
			//PORTE=(0<<PINE0);
			
		}
		_delay_ms(2000);
	
		/*
		_delay_ms(100);
		key = scankey();
		if(key == '*') break;
		_delay_ms(1200);
		*/
		break;
		
	}
}

void displayTime(){
	int motor=0;
	char buffer[20];
	char* days[7]= {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
	

	I2C_Init();										/* Initialize I2C */
	lcdint();									/* Initialize LCD16x2 */

	DDRD |= 1 << PIND0;
	
	
	while(1)
	{
		RTC_Read_Clock(0);							/* Read the clock with second address i.e location is 0 */
		if (hour & TimeFormat12)
		{
			sprintf(buffer, "%02x:%02x:%02x  ", (hour & 0b00011111), minute, second);
			if(IsItPM(hour))
			strcat(buffer, "PM");
			else
			strcat(buffer, "AM");
			lcd_string(buffer,2);
		}
		
		else
		{
			sprintf(buffer, "%02x:%02x:%02x  ", (hour & 0b00011111), minute, second); //write value into minutes register. The MINUTES register is composed of 2 parts: The lower nibble (bit 0 through bit 3) and the upper nibble (bit 4 through bit 6). Both nibbles are used together to represent double-digit numbers. For example, the number �13� is composed of a �1� and a �3�. Each digit is stored in a separate nibble. The �1� is stored as a BCD number in the upper nibble while the �3� is stored as a BCD number in the lower nibble. For example, to store the number �13�, the MINUTES register should be set to �001 0011� in binary.The �001� is BCD for �1� and 0011 is BCD for �3�
			
			lcd_line_one();
			lcd_string("Time:",5);
			lcd_string(buffer,9);
			
			if ((hour==0b00010111) && (minute==0b00010010)) {
				//_delay_ms(1000);
				PORTC= PORTC | (1<<6) | (1<<7);
				lcd_line_two();
				lcd_string("Motor ON",8);
				_delay_ms(6000);
				PORTC= PORTC &(~(1<<6)) & (~(1<<7));


				//lcd_print_xy(1,0,buffer);
				
			}
			
			
			else {
				lcd_line_two();
				lcd_string("Motor Off",12);				
				// PORTD &= ~(1 << PIND0);
				
			}
			
		}
		//RTC_Read_Calendar(3);						/* Read the calender with day address i.e location is 3 */
		//sprintf(buffer, "%02x/%02x/%02x %3s ", date, month, year,days[day-1]);
		//lcd_print_xy(1,0,buffer);
		break;
	}
}