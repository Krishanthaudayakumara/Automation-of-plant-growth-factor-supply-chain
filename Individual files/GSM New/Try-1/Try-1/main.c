/*
 * Try-1.c
 *
 * Created: 4/20/2022 7:32:38 PM
 * Author : Krishantha
 */ 

#include <avr/io.h>
#include <util/delay.h>

#ifndef F_CPU
#define F_CPU 8000000UL // 1 MHz clock speed
#endif

#include "lcd.c"


int main(void)
{
	lcdint();

	lcd_line_one();
	lcd_string("Hi",8);
 
}

