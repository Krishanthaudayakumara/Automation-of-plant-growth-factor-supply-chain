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
#include "I2C_Master_H_file.h"

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
	int ldr = ldrconfig();
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
		ldr = (i*10.0/(5.0-i))-1;     // read adc value at PA0
		

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
		break;
	}

	return ldr;
}
#define Device_Write_address	0xD0				/* Define RTC DS1307 slave address for write operation */
#define Device_Read_address		0xD1				/* Make LSB bit high of slave address for read operation */
#define TimeFormat12			0x40				/* Define 12 hour format */
#define AMPM					0x20


int second,minute,hour,day,date,month,year;

bool IsItPM(char hour_)
{
	if(hour_ & (AMPM))
	return 1;
	else
	return 0;
}

void RTC_Read_Clock(char read_clock_address)
{
	I2C_Start(Device_Write_address);				/* Start I2C communication with RTC */
	I2C_Write(read_clock_address);					/* Write address to read */
	I2C_Repeated_Start(Device_Read_address);		/* Repeated start with device read address */

	second = I2C_Read_Ack();						/* Read second */
	minute = I2C_Read_Ack();						/* Read minute */
	hour = I2C_Read_Nack();							/* Read hour with Nack */
	I2C_Stop();										/* Stop i2C communication */
}

void RTC_Read_Calendar(char read_calendar_address)
{
	I2C_Start(Device_Write_address);
	I2C_Write(read_calendar_address);
	I2C_Repeated_Start(Device_Read_address);

	day = I2C_Read_Ack();							/* Read day */
	date = I2C_Read_Ack();							/* Read date */
	month = I2C_Read_Ack();							/* Read month */
	year = I2C_Read_Nack();							/* Read the year with Nack */
	I2C_Stop();										/* Stop i2C communication */
}


int main(void)
{
	int motor=0;
	char buffer[20];
	char* days[7]= {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
	

	I2C_Init();										/* Initialize I2C */
	lcdinit();										/* Initialize LCD16x2 */

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
			lcd_print_xy(0,0,buffer);
		}
		
		else
		{
			sprintf(buffer, "%02x:%02x:%02x  ", (hour & 0b00011111), minute, second); //write value into minutes register. The MINUTES register is composed of 2 parts: The lower nibble (bit 0 through bit 3) and the upper nibble (bit 4 through bit 6). Both nibbles are used together to represent double-digit numbers. For example, the number “13” is composed of a ‘1’ and a ‘3’. Each digit is stored in a separate nibble. The ‘1’ is stored as a BCD number in the upper nibble while the ‘3’ is stored as a BCD number in the lower nibble. For example, to store the number “13”, the MINUTES register should be set to ‘001 0011’ in binary.The ‘001’ is BCD for ‘1’ and 0011 is BCD for ‘3’
			
			lcd_print_xy(0,0,buffer);
			
			if ((hour==0b00011000) && (minute==0b01010000)) {
				
				
				motor=1;
				//_delay_ms(1000);
				//PORTD=(0<<PIND0);
				//sprintf(buffer,"Motor Off");
				//lcd_print_xy(1,0,buffer);
				
			}
			
			
			else {
				sprintf(buffer,"Motor Off");
				lcd_print_xy(1,0,buffer);
				
				PORTD &= ~(1 << PIND0);
				
			}
			
			if(motor==1){
				
				sprintf(buffer,"Motor On");
				lcd_print_xy(1,0,buffer);
				
				PORTD |= 1 << PIND0;
				_delay_ms(6000);
				motor=0;
			}
		}
		//RTC_Read_Calendar(3);						/* Read the calender with day address i.e location is 3 */
		//sprintf(buffer, "%02x/%02x/%02x %3s ", date, month, year,days[day-1]);
		//lcd_print_xy(1,0,buffer);
	}
}