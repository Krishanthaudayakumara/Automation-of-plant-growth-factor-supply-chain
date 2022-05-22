/*
 * ATmega 16/32 interfacing with RTC DS1307
 * http://www.electronicwings.com
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "I2C_Master_H_file.h"
#include "LCD16x2_4Bit.h"
#include <util/delay.h>

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