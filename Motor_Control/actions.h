#ifndef GIMBAL_ACTIONS
#define GIMBAL_ACTIONS

#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define S1MAX    5000U
#define S1MIN     500U
#define S2MAX    5000U
#define S2MIN     500U
#define TENmS     156U
#define BAUD9600  103U
#define ICR1MAX 50000U

#define F_CPU 16000000UL

void TIMER0_INIT();
void TIMER1_INIT();
void PIN_INIT();
void UART_INIT();
void USART_TRANSMIT(char data);

#endif
