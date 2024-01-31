#include "actions.h"

void UART_INIT() {
    cli(); // Disable interrupts prior to USART initialization

    UCSR0A &= 0;              // Do not need any settings from here
    UCSR0B |= 0b10010000;     // Receive enable, receive interrupt enable, character size 8 bit.
    UCSR0C |= 0b00000110;     // Character size 8 bit, asynchronous UART, 1 stop bit, no parity check

    UBRR0H = BAUD9600 >> 8;
    UBRR0L = BAUD9600;

    UCSR0B |= (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0); // Enable receiver, transmitter, and receive interrupt

    sei(); // Enable interrupts following initialization
}

void USART_TRANSMIT(char data) {
    while (!(UCSR0A & (1 << UDRE0)));    // Wait for empty transmit buffer
    UDR0 = data;                          // Put data into buffer and send
}

void PIN_INIT() {
    DDRB  |= (1<< DDB0 | 1 << DDB1 | 1 << DDB2 | 1 << DDB3);
    PORTB |= (1 << DDB0 | 1 << PORTB1 | 1 << PORTB2 | 1 << PORTB3);
    PORTB &= ~(1<<PORTB0);
}   

void TIMER0_INIT() {
    // TIMSK0 |= (3 << 1); // Enable OCIE0A, OCIE0B interrupts

    TCCR0A |= 0b11110010;    // Set 0C0A, 0C0B on compare match, set wavegen mode to CTC
    TCCR0B |= 0b00000101;    // Wavegen mode to CTC, prescaler to 1024
    OCR0A = TENmS;           // 10mS debounce with 1024 PSC
}

void TIMER1_INIT() {
    TCCR1A |= (1 << COM1A1 | 1 << COM1B1 | 1 << WGM11); // Initialize Timer 1 Fast PWM ICR
    TCCR1B |= (1 << WGM12 | 1 << WGM13 | 1 << CS11);    // Pre-scale to 8
    TIFR1 |= 3 << 1;

    OCR1AH = S1MIN >> 8;
    OCR1AL = S1MIN;

    OCR1BH = S2MIN >> 8;
    OCR1BL = S2MIN;

    ICR1H = ICR1MAX >> 8;
    ICR1L = ICR1MAX;
}

ISR(USART_RX_vect, ISR_BLOCK) {
    cli(); // Disable interrupt nesting
    volatile uint8_t data = UDR0;

    if (data == 'W' || data == 'w') {
        USART_TRANSMIT(data);
        if (OCR1A < (S1MAX - 50))
            OCR1A += 50;
    } else if (data == 'S' || data == 's') {
        USART_TRANSMIT(data);
        if (OCR1A > (S1MIN + 50))
            OCR1A -= 50;
    } else if (data == 'D' || data == 'd') {
        USART_TRANSMIT(data);
        if (OCR1B < (S2MAX - 50))
            OCR1B += 50;
    } else if (data == 'A' || data == 'a') {
        USART_TRANSMIT(data);
        if (OCR1B > (S2MIN + 50))
            OCR1B -= 50;
    } else if (data == 'Q' || data == 'q') {
        USART_TRANSMIT(data);
        PORTB ^= 1 << PORTB0;
    }
    

    sei(); // Re-enable interrupts upon completion of ISR
}
