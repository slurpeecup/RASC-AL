/*
 * Gimbal PWM.c
 *
 * Created: 11/09/2023 9:36:10 PM
 * Author : RASC-GULL
 */ 

//Objective is to control LED's and 360 Servos with pulse width modulation

#include <avr/io.h>
#include <time.h>

void init()
{
	DDRD  |= (1<<DDD5 | 1<<DDD6); //Enable pin outputs D5 and D6
	PORTD |= (1<<PORTD5 | 1<<PORTD6);
}

void timer_0_init()
{
	TCCR0A |= (1<<WGM00 | 1<<WGM01 | 1<<COM0A1 | 1<<COM0B1); //Enable Fast PWM for Timer0 LED
	TCCR0B |= (1<<CS00 | 1<<CS02); //Pre-Scale to 1024
	OCR0A = 77; //30% duty cycle of pre-scale
	OCR0B = 179; //70% duty cycle of pre-scale
}

int main(void)
{	
	init();
	timer_0_init();
	
    	while(1){}
}

