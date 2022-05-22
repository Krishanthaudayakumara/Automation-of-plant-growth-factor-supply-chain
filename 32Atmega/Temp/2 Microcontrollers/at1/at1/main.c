/*
 * at1.c
 *
 * Created: 5/4/2022 7:15:20 PM
 * Author : Krishantha
 */ 
#define F_CPU 1000000UL						/* define Clock Frequency */
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>

#include "lcd.c"
#include "SPImaster.c"


int main(void)
{
		DDRA = DDRA | (1<<5) | (1<<6) | (1<<7);

		DDRC=0xf0;

		uint8_t count;

		char buffer[5];
		char key;
		int wtrperPlant, fertilizerPlant, humMin = 0, humMax, temprtMin, temprtMax;
		

		lcdint();
		
		SPImstr_init();
    /* Replace with your application code */
    while (1) 
    {
		SPI_write(count);
		sprintf(buffer, "%d   ", count);
		lcd_clear();
		lcd_string(buffer,2);
		
		count++;
		_delay_ms(500);
    }
}

