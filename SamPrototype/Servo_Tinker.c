/*
 * Servo Tinker.c
 *
 * Created: 11/22/2023 8:01:36 PM
 * Author : sam14
 */ 

//Objective is to control LED's with varying pulse width modulation through button control.

#define F_CPU 16000000UL

#include <stdlib.h>
#include <time.h>
#include <avr/io.h>
#include <util/delay.h>

#define T1MAX 5000U
#define T1MIN 500U
#define ICR1MAX 50000U

void FFmSDELAY()
{
	for(volatile uint8_t a = 0; a < 0xFF; ++a){
		for(volatile uint8_t b = 0; b < 100; ++b);
	}
}
void OmSDELAY()
{
	for(volatile uint8_t a = 0; a < 0xFF; ++a){
		for(volatile uint8_t b = 0; b < 2; ++b);
	}
}
void pin_init()
{
	DDRC  &= ~(1<<DDC0 | 1<<DDC1 | 1<<DDC2 | 1<<DDC3);					//Enable pin inputs C0, C1, and C2 for buttons 
																		//PC0 controls increment up, PC1 controls decrement, PC2 controls toggle, reads +5 or 0v													         
	PORTC |= (31<<0);						                            //Pull-up resistance for buttons
	DDRD  |= (1<<DDD5 | 1<<DDD6);										//Enable pin outputs D5 and D6 for LEDs
	PORTD |= (1<<PORTD5 | 1<<PORTD6);

	DDRB  |= (1<<DDB1 | 1 << DDB2 | 1<<DDB3);
	PORTB |= (1<<PORTB1 | 1 << PORTB2 | 1<<PORTB3);
}

void timer1_init()
{
	TCCR1A |= (1<<COM1A1 | 1<<COM1B1 | 1<<WGM11); //Initialize Timer 1 Fast PWM ICR
	TCCR1B |=  (1<<WGM12 | 1<<WGM13 | 1<<CS11); //Pre-scale to 8
	TIFR1 |= 3<<1;
	
	OCR1AH = T1MIN>>8;
	OCR1AL = T1MIN;
	
	OCR1BH = T1MIN>>8;
	OCR1BL = T1MIN;
	
	ICR1H = ICR1MAX>>8;
	ICR1L = ICR1MAX && 0x00FF;
}

void SERVO_PAN()
{
	uint8_t ON_FLAG = 0U;
		static uint16_t OCR1B_TOTAL = T1MIN;

		// TODO, this function should also dim LED1 symmetrically as it steps LED0, and vice versa.
		// For example, LED0 brightness = 240, LED1 brightness = 15. LED0 brightness = 235, LED1 brightness is 20, LED0 brightness is 100, LED1 brightness is 155... and so on.
		
		
		
		if (OCR1B_TOTAL > T1MAX)  OCR1B_TOTAL = T1MAX;
		if (OCR1B_TOTAL < T1MIN)  OCR1B_TOTAL = T1MIN;     //Set upper and lower bounds
		
		OCR1BH = OCR1B_TOTAL >> 8;
		OCR1BL = OCR1B_TOTAL;

		if ((!(PINC & (1<<0)) && ON_FLAG == 0)) //When button is pressed, decrement the duty var till equal to T1MIN.
		{
		  OmSDELAY();
		  OCR1B_TOTAL += 1;	
		}
		else OCR1B_TOTAL -= 1;
}

int main(void)
{
	pin_init();						//Call initializations
	timer1_init();
		
	while (1)
	{	 
		if(!(PINC & (1<<2)))        //If toggle low     
		{
//			_delay_ms(25);			//Debounce time
			if(!(PINC & (1<<2)))    //Toggle is really low
			{
		//		TILT_LED_CTL();
				SERVO_PAN();
			} 
		}
	//	else //PAN_LED_CTL();
	}
}
