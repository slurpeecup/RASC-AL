/*
 * LED PWM Phase 2.c
 *
 * Created: 11/12/2023 3:28:10 PM
 * Author : RASC-GULL
 */ 

//Objective is to control LED's with varying pulse width modulation through button control.

#define F_CPU 16000000UL

#include <avr/io.h>
#include <stdlib.h>
#include <time.h>

void pin_init()
{
	DDRC  &= ~(1<<DDC0 | 1<<DDC1 | 1<<DDC2 | 1<<DDC3 | 1<<DDC4); //Enable pin inputs C0, C1, C2, and C4 for buttons 	 
        //PC0 controls increment, PC1 controls decrement, PC2 controls toggle, reads +5V or 0V													         
	PORTC |= (31<<0);		          //Pull-up resistance for buttons
	DDRD  |= (1<<DDD5 | 1<<DDD6);             //Enable pin outputs D5 and D6 for LEDs
	PORTD |= (1<<PORTD5 | 1<<PORTD6);
	
	DDRB |= 1<<DDB3;
	PORTB |= 1<<PORTB3;
}

void timer0_init()   //TODO: REVISIT W/ TIMER1 once this works... lmfao
{
	TCCR0A |= (1<<COM0A1 | 1<<COM0B1 | 1<<WGM00 | 1<<WGM01);	//Enable Fast PWM for Timer0 LED
	TCCR0B |= (1<<CS00 | 1<<CS02);					//Pre-Scale to 1024
	OCR0A = 255U;
	OCR0B = 0U;
}

void timer2_init()
{
	TCCR2A |= 163<<WGM20; //COM2A1, COM2B1, WGM20,WGM21, TCCR2A = 10xx--x11
	TCCR2B |= 5<<CS20; //111, PSC = 1024, TCCR2B = XX--XXXX
	OCR2A = 0;
}

void FFmSDELAY()
{
	for(volatile uint8_t a = 0; a < 0xFF; ++a)
	{
		for(volatile uint8_t b = 0; b < 100; ++b);
	}
}

void set_led0_bright()
{
	static uint8_t ON_FLAG = 0U; // Ensure suser is not holding the button 
	// TODO, this function should also dim LED1 symmetrically as it steps LED0, and vice versa. 
	// For example, LED0 brightness = 240, LED1 brightness = 15. LED0 brightness = 235, LED1 brightness is 20, LED0 brightness is 100, LED1 brightness is 155... and so on.
	if (OCR0A > 245) OCR0A = 255;
	if (OCR0A < 10)  OCR0A = 0;     //Set upper and lower bounds

	if(!(PINC & (1<<0)) && ON_FLAG == 0) //When button is pressed, increment the duty var till equal to 255. This brightens the LED.
	{
		FFmSDELAY();
		if(OCR0A <= 245 && OCR0A >= 0)
		{
			OCR0A+=10;	//Discrete step of roughly 2%
			ON_FLAG++;
		}
	}
	else ON_FLAG--;
}

void set_led0_dim() // This function should do the same as LED0, but in the opposite direction.
{
  	static uint8_t ON_FLAG = 0U; //Ensures user is not holding the button
  	if (OCR0A > 245) OCR0A = 255;
  	if (OCR0A < 10)  OCR0A = 0;   
	
	if(!(PINC & (1<<1)) && ON_FLAG == 0)
	{
		FFmSDELAY();
	  	if(OCR0A <= 255 && OCR0A >= 10)
	  	{
	    		OCR0A-=10;
	    		ON_FLAG++;
	  	}
	}
  	else ON_FLAG--;
}

void set_led1_bright() //Same as LED 0 Brightness
{
	static uint8_t ON_FLAG = 0U;
  	if (OCR0B > 245) OCR0B = 255;
  	if (OCR0B < 10)  OCR0B = 0;

  	if(!(PINC & (1<<1)) && ON_FLAG == 0)
	{
		FFmSDELAY();
		if(OCR0B <= 245 && OCR0B >= 0)
	 	{
	    		OCR0B+=10;
	    		ON_FLAG++;
	  	}
	} 
  	else ON_FLAG--;
}

void set_led1_dim() //Same as LED 0 Dimness
{
	static uint8_t ON_FLAG = 0U;
	if (OCR0B > 245) OCR0B = 255;
	if (OCR0B < 10)  OCR0B = 0;

	if(!(PINC & (1<<0)) && ON_FLAG == 0)
	{
		FFmSDELAY();
		if(OCR0B <= 255 && OCR0B >= 10)
	 	{
	    		OCR0B-=10;
	    		ON_FLAG++;
		}
	} 
	else ON_FLAG--;
}

void set_led2() //Same as previous LEDs, but this one is combined increment/decrement.
{
	static uint8_t ON_FLAG = 0U;
	if (OCR2A > 245) OCR2A = 255;
	if (OCR2A < 10)  OCR2A = 0;
	
	if ((!(PINC & (1<<0)) && ON_FLAG == 0))
  	{
   		FFmSDELAY();
   		if(OCR2A <= 245 && OCR2A >= 0)
   		{
     			OCR2A+=10;
     			ON_FLAG++;
   		}
  	}
	
  if(!(PINC & (1<<1)) && ON_FLAG == 0)
  {
	FFmSDELAY();
	if(OCR2A <= 255U && OCR2A >= 10U)
    	{
      		OCR2A-=10U;
      		ON_FLAG++;
    	}
  }
	else ON_FLAG--;
}

void PAN_LED_CTL()
{
	set_led2();
}

void TILT_LED_CTL()
{
	set_led0_bright();
  	set_led0_dim();		
  	set_led1_bright();
  	set_led1_dim();
}

int main(void)
{
  	pin_init();	//Call initializations
	timer0_init();
  	timer2_init();
		
  while(1)
  {	 

    	if(!(PINC & (1<<2)))        //If toggle low     
   	{
      		if(!(PINC & (1<<2)))      //Toggle is really low
      		{
			TILT_LED_CTL();
      		} 
    	}
 	else PAN_LED_CTL();
  }
}
