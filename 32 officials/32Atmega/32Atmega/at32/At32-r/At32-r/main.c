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
#include "USART_Interrupt.c"
#include "gsm.c"
#include "SPI_slave.c"

#define SREG   _SFR_IO8(0x3F)



int main(void)
{
	uint8_t data, sent_wt = 0, sent_nt = 0;
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
	lcd_clear();
	_delay_ms(2000);
	
	while(1){
		lcd_clear();
		data = SPI_Receive();
		if (data == 2) {
			lcd_string("WATER LOW!",10);
			_delay_ms(100);
			if (sent_wt == 0)
			{
				GSM_Send_Msg("+94763183978","Water low! Please fill the Tank");
				sent_wt = 1;
			}
			
			lcd_clear();
			lcd_string("MESSAGE SENT!",13);
			_delay_ms(500);
			break;

		}
		if (data==3)
		{
			lcd_string("NUTRIENT LOW!",14);
			_delay_ms(100);
			if (sent_nt == 0)
			{
				GSM_Send_Msg("+94763183978","Water low! Please fill the Tank");
				sent_nt = 1;
			}
			
			lcd_clear();
			lcd_string("MESSAGE SENT!",13);
			_delay_ms(500);
			break;
		}
		
		break;
	}
	
	

   
}


