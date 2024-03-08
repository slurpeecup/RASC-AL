#include "auto_stepper_resources.h"

void UART_INIT()
{
  cli();


  UBRR0 = 12; //76.8kb/s with U2X0 = 0; if this no work try UBRR0 = 25, 38.4 kb/s
  
  UCSR0A &= ~ (1<<U2X0);    // no division
  UCSR0B |= 0b10011000;     // Receive enable, receive interrupt enable, character size 8 bit.
  UCSR0C |= 0b00000110;     // Character size 8 bit, asynchronous UART, 1 stop bit, no parity check

  sei();  

}

void USART_TRANSMIT(char data) 
{
    while (!(UCSR0A & (1 << UDRE0)));    // Wait for empty transmit buffer
    UDR0 = data;                          // Put data into buffer and send
}

// what do?
/*
every 500uS or 0.5mS, stepper takes one step, this corresponds to a
frequency of 2kHz. at an step angle of 0.9deg, the motor revolves at
1800 deg/sec at a microstep ratio of 1:1. targeting a microstep ratio
of 1:16, the motor traverses a much more modest (read: slow) traversal
of 112.5 deg/sec. this is pretty acceptable honestly, so until evidence
suggests this is a bad course of action, I will adhere to a step duration 
of 500uS
*/

void TIMER1_INIT()
{
    

}
