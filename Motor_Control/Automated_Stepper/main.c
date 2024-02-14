/*
 * Automated Stepper.c
 * Created: 02/14/2024 5:10:52 PM
 * Author : RASCGULL
 */

//Objective is to control two stepper motors for gimbal.

/*I am currently writing generic parsing code for the horizontal stepper only. Once that works, I will add vertical functioning. 
The interrupt needs updating too. The current idea is to integrate the parse_scan function into the rotating step functions.*/

#define F_CPU 16000000UL
#define BAUD 115200
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)
#define BAUD9600 103u
#define directionPin1 (1<<PORTB0)
#define pulsePin1 (1<<PORTB1)
#define directionPin2 (1<<PORTD3)
#define pulsePin2 (1<<PORTD4)

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void uart_init() //Initialize transmit/receive setting for primary/secondary comms
{
    cli(); //Disable interrupts prior to USART initialization

    UCSR0A &= 0; //Do not need any settings from here
	UCSR0B |= (1 << RXEN0 | 1 << TXEN0 | 1 << RXCIE0); //Enable receiver, transmitter, and receive interrupt.
    UCSR0B |= 0b10010000; //Receive enable, receive interrupt enable, character size 8 bit.
	UCSR0C |= 0b00000110; //Character size 8 bit, asynchronous UART, 1 stop bit, no parity check
    UBRR0H = BAUD9600>>8;
    UBRR0L = BAUD9600;

    sei(); //Enable interrupts following initialization
}

void uart_transmit(unsigned char data) //Transmit buffer
{
	while(!(UCSR0A & (1<<UDRE0)));  //Wait for empty transmit buffer
	UDR0 = data;                    //Put data into buffer and send
}

void step_hcw() //Rotate horizontal clockwise
{
	unsigned char count_cw;
  
	PORTB &= ~directionPin1; //Set direction pin LOW to rotate clockwise
	for(count_cw = 0; count_cw < 200; count_cw++) //There are 200 steps in this stepper motor. Each step is 1.8� x 200 = 360�.
	{
		PORTB |= pulsePin1;  //Pulse on and off clockwise
		_delay_ms(1);
		PORTB &= ~pulsePin1;
		_delay_ms(1);
	}
}

void step_hccw() //Rotate horizontal counterclockwise
{
	unsigned char count_ccw;

	PORTB |= directionPin1;  //Set direction pin HIGH to rotate counterclockwise
	for(count_ccw = 0; count_ccw < 200; count_ccw++)
	{
		PORTB |= pulsePin1; //Pulse on and off counterclockwise
		_delay_ms(1);
		PORTB &= ~pulsePin1;
		_delay_ms(1);
	}
}

void step_vcw() //Rotate vertical clockwise
{
	unsigned char count_vcw;
	
	PORTD &= ~directionPin2;
	for(count_vcw = 0; count_vcw < 200; count_vcw++)
	{
		PORTD |= pulsePin2;  //Pulse on and off clockwise
		_delay_ms(1);
		PORTD &= ~pulsePin2;
		_delay_ms(1);
	}
}

void step_vccw() //Rotate vertical counterclockwise
{
	unsigned char count_vccw;
	
	PORTD |= directionPin2;
	for(count_vccw = 0; count_vccw < 200; count_vccw++)
	{
		PORTD |= pulsePin2;  //Pulse on and off counterclockwise
		_delay_ms(1);
		PORTD &= ~pulsePin2;
		_delay_ms(1);
	}
}

void parse_scan(const char *command, int *step, char *direction) //Parse string information. As of now this is all for one stepper motor. Will likely dismantle this function.
{
	sscanf(command, "%d, %s", step, direction); //Search through the string
	
	if(*step > 100) //Limit step to no more than 100. The gimbal doesn't need to swivel around.
	{
		*step = 100;
		printf("Warning! Nothing more than 100 steps are needed.\n");
	}
}

void parse_instruct() //Instructs stepper motor(s) how many steps to take after scan
{
	char inputString[20];
	printf("Please enter number of steps and direction desired:/n");
	fgets(inputString, sizeof(inputString), stdin); //Get ready to read from address of inputString. "sizeof" command prevents string overflow.
	
	int step;		 //Steps up to 100.
	char direction;  //Direction can either be CW or CCW. High is CCW whereas low is CW.
	
	parse_scan(inputString, &step, direction); //Do the actual parsing.
	
	printf("Output for horizontal stepper:/n"); //Displays user input.
	printf("Steps: %d/n", step);
	printf("Direction: %s/n", direction);
}

ISR(USART_RX_vect, ISR_BLOCK) //OUTDATED
{
	cli(); //Disable interrupt nesting
	volatile uint8_t data = UDR0;

	if(data == 'A' || data == 'a') //If key 'A' or 'a' is detected, step horizontally in the counterclockwise direction.
	{
		uart_transmit(data);
		step_hccw();
	} 
  
	else if(data == 'S' || data == 's') 
	{
		uart_transmit(data);
		step_hcw();
	}

	else if(data == 'W' || data == 'w')
	{
		uart_transmit(data);
		step_vcw();
	}
  
	else if(data == 'Z' || data == 'z')
	{
		uart_transmit(data);
		step_vccw();
	}

	sei(); //Enable global interrupt
}

int main() //Call all child functions
{
	DDRB |= (1<<DDB0 | 1<<DDB1); //Enable 2 random pins for DIRECTION- and PULSE- for Stepper Motor 1
	DDRD |= (1<<DDD3 | 1<<DDD4); //Enable 2 random pins for DIRECTION- and PULSE- for Stepper Motor 2

	uart_init();
	sei();
	while (1){}
}