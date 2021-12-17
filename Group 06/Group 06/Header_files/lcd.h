#include <stdio.h>
#include <math.h>

#include<avr/io.h>

#define lcdport PORTA

#define signal PORTB

#define en PB2

#define rw PB1

#define rs PB0

char key;

void lcdcmd(unsigned char cmd);//lcd commands

void lcdint();//initialize lcd

void lcddata(unsigned char data);

void lcd_string(const unsigned char *str, unsigned char length);//display in lcd

int charToInt(char c);//character to integer

int displayKey();//save input

void lcd_clear();//lcd clear

void lcd_line_one();//lcd line one

void lcd_line_two();//lcd line two



void lcdint()
{
	lcdcmd(0x38);

	_delay_ms(1);

	lcdcmd(0x01);

	_delay_ms(1);

	lcdcmd(0x0E);

	_delay_ms(1);
}


void lcdcmd(unsigned char x)
{
	lcdport=x;

	signal=(0<<rs)|(0<<rw)|(1<<en);

	_delay_ms(1);

	signal=(0<<rs)|(0<<rw)|(0<<en);

	_delay_ms(50);
}

void lcddata(unsigned char data)
{
	lcdport= data;

	signal= (1<<rs)|(0<<rw)|(1<<en);

	_delay_ms(1);

	signal= (1<<rs)|(0<<rw)|(0<<en);

	_delay_ms(50);
}


void lcd_string(const unsigned char *str, unsigned char length)
{
	for(int i=0; i<length; i++)
	{
		lcddata(str[i]);
	}
}

int charToInt(char c){
	int num = 0;
	num = c - '0';

	return num;
}