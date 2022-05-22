/*
 * at2.c
 *
 * Created: 5/4/2022 7:31:22 PM
 * Author : Krishantha
 */ 
#define F_CPU 1000000UL						/* define Clock Frequency */
#include <avr/io.h>
#include <util/delay.h>

#include "lcd.c"
#include "SPIslave.c"

int main(void)
{
	uint8_t data, sent_wt = 0, sent_nt = 0;
		uint8_t count;

	char buffer[20];
	char* days[7]= {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
	
	
	DDRB = DDRB | (1<<0) | (1<<1) | (1<<2);

	/* Replace with your application code */

	lcdint();
	SPIsl_init();
    /* Replace with your application code */
    while (1) 
    {
		count = SPI_Receive();
		lcd_string("BIF",3);
		sprintf(buffer, "%d   ", count);
		lcd_string(buffer,2);
		_delay_ms(100);
		lcd_clear();

    }
}

