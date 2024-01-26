/*
 * Stepper Motor Comms.c
 * Created: 1/23/2024 5:10:52 PM
 * Author : RASCGULLS
 */ 

#define F_CPU 16000000UL
#define BAUD 9600
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)
#define FORWARD 0
#define REVERSE 1

#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>

void PIN_INIT() 
{
	DDRB  |= (1<<DDB0 | 1<<DDB1);
	PORTB |= (1<<PORTB0 | 1<<PORTB1);
}

void USART_INIT()
{
	cli(); //Disable interrupts prior to USART initialization

	UCSR0A &= 0;		   //Do not need any settings from here
	UCSR0B |= 0b10010000;  //Receive enable, receive interrupt enable, character size 8 bit.
	UCSR0C |= 0b00000110;  //Character size 8 bit, asynchronous UART, 1 stop bit, no parity check

	UBRR0H = (BAUDRATE>>8);
	UBRR0L = BAUDRATE;

	UCSR0B |= (1<<RXEN0 | 1<<TXEN0 | 1<<RXCIE0); // Enable transmitter, receiver, and receive interrupt

	sei(); //Enable all interrupts following initialization
}

void USART_TRANSMIT(unsigned char data)
{
	while(!(UCSR0A & (1<<UDRE0)));		//Wait for empty transmit buffer
	UDR0 = data;                        //Put data into buffer and send
}

ISR(USART_RX_vect, ISR_BLOCK)
{
	cli();								//Disable interrupt nesting
	volatile uint8_t data = UDR0;
	
	if (data == 'W' || data == 'w')
		{
			USART_TRANSMIT(data);
			// switch control
		}
	if (data == 'A' || data == 'a')
		{
			USART_TRANSMIT(data);
			// will likely need a while loop "direction++"
			if(int i = FORWARD)
			{
				PORTB |= (1<<PORTB0);
			}
		}
	else if (data == 'S' || data == 's')
		{
			USART_TRANSMIT(data);
			// while loop in other direction "direction--"
			for(int i = REVERSE)
			{
				PORTB |= (1<<PORTB1);
			}
		}
		
		sei();
}

int main(void)
{
	PIN_INIT();
	USART_INIT();
	
	sei();
	
    while (1){}
}

