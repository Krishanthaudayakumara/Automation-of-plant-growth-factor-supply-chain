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
#include "I2C_Master_H_file.h"
#include "I2C_Master_C_file.c"

#define SREG   _SFR_IO8(0x3F)


int main(void)
{
	uint8_t data, sent_wt = 0, sent_nt = 0;
	
	char buffer[20];
	char* days[7]= {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
	
	
	DDRB=0xff;
	DDRD=0x07;
    /* Replace with your application code */
	buffer_pointer = 0;

	lcdint();
	SPIsl_init();
	
	data = SPI_Receive();
	sprintf(buffer,"%d",data);
	lcd_string(data,8);

	I2C_Init();
	
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
	
	
	while(1){
		lcd_string("Hi",2);
		_delay_ms(500);
		lcd_clear();
		data = SPI_Receive();
		lcd_string("BII",3);
		_delay_ms(500);


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
		else{
			RTC_Read_Clock(0);							/* Read the clock with second address i.e location is 0 */
			if (hour & TimeFormat12)
			{
				sprintf(buffer, "%02x:%02x:%02x  ", (hour & 0b00011111), minute, second);
				if(IsItPM(hour))
				strcat(buffer, "PM");
				else
				strcat(buffer, "AM");
				lcd_string(buffer,12);
			}
			
			else
			{
				sprintf(buffer, "%02x:%02x:%02x  ", (hour & 0b00011111), minute, second); //write value into minutes register. The MINUTES register is composed of 2 parts: The lower nibble (bit 0 through bit 3) and the upper nibble (bit 4 through bit 6). Both nibbles are used together to represent double-digit numbers. For example, the number “13” is composed of a ‘1’ and a ‘3’. Each digit is stored in a separate nibble. The ‘1’ is stored as a BCD number in the upper nibble while the ‘3’ is stored as a BCD number in the lower nibble. For example, to store the number “13”, the MINUTES register should be set to ‘001 0011’ in binary.The ‘001’ is BCD for ‘1’ and 0011 is BCD for ‘3’
				
				lcd_string(buffer,16);
				
				if ((hour==0b00011000) && (minute==0b01010000)) {
					
					_delay_ms(1000);
					//PORTD=(0<<PIND0);
					//sprintf(buffer,"Motor Off");
					//lcd_print_xy(1,0,buffer);
					
				}
				
				
				
			}
		}
		
		break;
	}
	
	

   
}


