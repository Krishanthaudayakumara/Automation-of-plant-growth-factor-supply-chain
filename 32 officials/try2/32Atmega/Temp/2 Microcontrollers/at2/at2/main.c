/*
 * at2.c
 *
 * Created: 5/4/2022 7:31:22 PM
 * Author : Krishantha
 */ 
#define F_CPU 1000000UL						/* define Clock Frequency */
#include <avr/io.h>


int main(void)
{
    /* Replace with your application code */
    while (1) 
    {

		PORTA &= ~(1 << PINA0); //0


    }
}

