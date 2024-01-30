/*
 * Stepper Motor Comms.c
 * Created: 1/23/2024 5:10:52 PM
 * Author : RASCGULLS
 */

//36kHz appears to be upper echelon of motor. 22 Volts will pull up to 5kHz

#define F_CPU 16000000UL
#define BAUD 115200
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)
#define CCW 0
#define CW 1

#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <string.h>

unsigned char count = 0;
static const char *string [];

void PIN_INIT() 
{
	DDRB  |= (1<<DDB0 | 1<<DDB1);    //Not sure which pins to use just yet. I think any of them can do.
	PORTB |= (1<<PORTB0 | 1<<PORTB1);
}

void USART_INIT()
{
	UCSR0A &= 0; //Do not need any settings from here
	UCSR0B |= ( 1<<TXEN0 | 1<<RXEN0); // Enable transmitter and receiver
	UCSR0C |= 0b00000110;  //Character size 8 bit, asynchronous UART, 1 stop bit, no parity check

	UBRR0H = (BAUDRATE>>8);
	UBRR0L = BAUDRATE;
}

void USART_TRANSMIT(unsigned char data)
{
	while(!(UCSR0A & (1<<UDRE0)));		//Wait for empty transmit buffer
	UDR0 = data;                        //Put data into buffer and send
}

void step_up(unsigned char direction)
{
	volatile uint8_t data = UDR0;
	
	if(data == 'A' || data == 'a')
		{
			USART_TRANSMIT(data);
			if(direction == CW)
			{
				count++;
				if(count > 7) count = 0;
				}
				
			else if(direction == CCW)
			{
				count--;	
			}
			
			else
			{
				count = 7;
			}
		}
		
void switch_step()
{		
	if(data == 'S' || data == 'S')
		{
			USART_TRANSMIT(data);
			case 0: value = 0x1; break;
			case 1: value = 0x1; break;
			case 2: value = 0x1; break;
			case 3: value = 0x1; break;
			case 4: value = 0x1; break;
			case 5: value = 0x1; break;
			case 6: value = 0x1; break;
			case 7: value = 0x1; break;
		}
}

int main(void)
{
	PIN_INIT();
	USART_INIT();
	step_up();
	switch_step;
	
    while (1){}
}