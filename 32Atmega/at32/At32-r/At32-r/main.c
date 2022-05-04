/*
 * At32-r.c
 *
 * Created: 4/30/2022 4:41:39 PM
 * Author : Krishantha
 */ 

#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdbool.h>
#include <util/delay.h>

#ifndef F_CPU
#define F_CPU 8000000UL // 1 MHz clock speed
#endif
#include "lcd.c"
#include "ldr.c"
#include "USART_Interrupt.c"
#include "gsm.c"
#include "SPI_slave.c"

#define SREG   _SFR_IO8(0x3F)


int ldrconfig();

int main(void)
{
	uint8_t data;
	char buffer[5];
	DDRB=0xff;
	DDRD=0x07;
    /* Replace with your application code */
	buffer_pointer = 0;

	lcdint();
	SPIsl_init();
	
	USART_Init(9600);						    /* initialize USART communication */
	sei();
	lcd_line_one();
	lcd_string("GSM Initializing",16);
	_delay_ms(3000);
	lcd_clear();
	lcd_line_one();
	lcd_string("AT",2);
	GSM_Begin();	
	GSM_Send_Msg("+94763183978","Hi bro");							/* check GSM responses and initialize GSM */
	lcd_clear();
	
	while(1){
		data = SPI_Receive();
		sprintf(buffer, "%d   ", data);
		lcd_string(buffer,10);
	}
	
	ldrconfig();
	_delay_ms(2000);

   
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
