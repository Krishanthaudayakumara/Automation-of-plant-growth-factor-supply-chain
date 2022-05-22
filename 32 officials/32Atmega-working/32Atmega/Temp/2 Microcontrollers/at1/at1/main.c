/*
 * at1.c
 *
 * Created: 5/4/2022 7:15:20 PM
 * Author : Krishantha
 */ 
#define F_CPU 1000000UL						/* define Clock Frequency */
#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
    /* Replace with your application code */
    while (1) 
    {
		
	
		PORTD |= 1 << PIND0; //1
		_delay_ms(1000);
		PORTD &= ~(1 << PIND0); //0
		_delay_ms(1000);
    }
}

