#ifndef F_CPU
#define F_CPU 1000000UL // 1 MHz clock speed
#endif
#include <avr/io.h>
#include <util/delay.h>
#include "LCD/lcd.h"
#define LTHRES 500

// initialize adc
void adc_init()
{
	// AREF = AVcc
	ADMUX = (1<<REFS0);
	
	// ADC Enable and prescaler of 128
	// 16000000/128 = 125000
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}

// read adc value
uint16_t adc_read(uint8_t ch)
{
	// select the corresponding channel 0~7
	// ANDing with '7' will always keep the value
	// of 'ch' between 0 and 7
	ch &= 0b00000111;  // AND operation with 7
	ADMUX = (ADMUX & 0xF8)|ch;     // clears the bottom 3 bits before ORing
	
	// start single conversion
	// write '1' to ADSC
	ADCSRA |= (1<<ADSC);
	
	// wait for conversion to complete
	// ADSC becomes '0' again
	// till then, run loop continuously
	while(ADCSRA & (1<<ADSC));
	
	return (ADC);
}

int main()
{
	uint16_t adc_result0;
	//int i;
	//int ldr;
	char buffer[10];
	DDRC = 0x01;           // to connect led to PC0
	
	// initialize adc and lcd
	adc_init();
	lcd_init(LCD_DISP_ON_CURSOR); //CURSOR

	
	_delay_ms(5);
	
	while(1)
	{
		adc_result0 = adc_read(0);      // read adc value at PA0
		//i=(adc_result0*0.01/2.1);
		//ldr = (i*10/(5-i));
		
		
		lcd_gotoxy(0,0);
		lcd_puts("CIRCUIT DIGEST");
		
		lcd_gotoxy(0,1);
		lcd_puts("LDE res=");
		itoa(adc_result0,buffer,10);   //display ADC value
		lcd_puts(buffer);
		
		_delay_ms(10);

		
		// condition for led to glow
		if (adc_result0 < LTHRES){
		PORTC = 0x00;
		}
		else{
		PORTC = 0x01;
		}
		
		
		
		
	}
}