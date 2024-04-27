/*
 * MCV2.c
 *
 * Created: 4/10/2024 4:16:15 AM
 * Author : z434
 */ 
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define MOTOR_X_PULSE_PIN 3
#define MOTOR_Y_PULSE_PIN 5
#define MOTOR_X_DIR_PIN 4
#define MOTOR_Y_DIR_PIN 6
#define MOTOR_ENA_PIN 2

//* STRUCT DEFINITIONS *//

typedef struct instruction {
	char MOTOR;
	char DIR;
	uint8_t LOWval;
	uint8_t HIGHval;
} instruction;

typedef struct input_frame_buffer {
	instruction INST;
	uint8_t temp_ins_index;
} input_frame_buffer;

typedef struct instruction_buffer {
	volatile instruction INST_LIST[32];
	uint8_t INST_INDEX;
	uint8_t num_insts;
} instruction_buffer;

typedef struct ACTIVE_INSTRUCTIONS {
	
	volatile uint8_t MOTOR_X_FLAG;
	volatile uint8_t MOTOR_X_DIRECTION;
	
	volatile uint8_t MOTOR_Y_FLAG;
	volatile uint8_t MOTOR_Y_DIRECTION;
	
	volatile uint16_t MOTOR_X_NSTEPS;
	volatile uint16_t MOTOR_Y_NSTEPS;
	
} ACTIVE_INSTRUCTIONS;

//* END STRUCT DEFINITIONS *//


//* GLOBAL VARIABLES *//

//########## GLOBAL DEFINITIONS ###############
volatile input_frame_buffer TFB0 = {
	{0, 0, 0, 0}, // INST
	0             // temp_ins_index
};

volatile instruction_buffer MINSB_X0 = {
	{{0}}, // INST_LIST
	0,   // INST_INDEX
	0    // num_insts
};

volatile instruction_buffer MINSB_Y0 = {
	{{0}}, // INST_LIST
	0,   // INST_INDEX
	0    // num_insts
};


volatile ACTIVE_INSTRUCTIONS ACTIVE_INST = {
	0, 0, 0,
	0, 0, 0
};

//* END GLOBAL VARIABLES *//


void TIMER1_INITIALIZATION()
{
  TCCR1A |= (1<<WGM11) | (1<<WGM10);             // mode 15
  TCCR1B |= (1<<WGM13) | (1<<WGM12) | (1<<CS10); // mode 15 no psc
  TIMSK1 |= (1<<OCIE1A) | (1<<OCIE1B);           // compare match A,B interrupt
  
  OCR1A = 16000;
  OCR1B = OCR1A >> 2;
}

void PINS_INITIALIZATION()
{
  DDRD |= (1<<PORTD2) | (1<<PORTD3) | (1<<PORTD4)
                      | (1<<PORTD5) | (1<<PORTD6);
  PORTD = 0;
}

void UART_INIT()
{
	cli();
	UBRR0 = 25; //76.8kb/s with U2X0 = 0; if this no work try UBRR0 = 25, 38.4 kb/s
	UCSR0A &= ~(1<<U2X0);    // no division
	UCSR0B |= 0b10011000;     // Receive enable, receive interrupt enable, character size 8 bit.
	UCSR0C |= 0b00000110;     // Character size 8 bit, asynchronous UART, 1 stop bit, no parity check
	UCSR0B |= (1 << RXEN0) | (1 << RXCIE0); // Receive enable, receive interrupt enable
	sei();
}

void TIMER1_INTERRUPTS_ON()
{
  TIMSK1 |=  (1<<OCIE1A) | (1<<OCIE1B);
}

void TIMER1_INTERRUPTS_OFF()
{
  TIMSK1 &= ~ (1<<OCIE1A) & ~(1<<OCIE1B);
}


//* Serial Debug Functions *//

void USART_TRANSMIT(char data)
{
	while (!(UCSR0A & (1 << UDRE0)));    // Wait for empty transmit buffer
	UDR0 = data;                          // Put data into buffer and send
}

uint8_t stringsize_from_ptr(char* inString)
{
	uint8_t retval = 0;
	while (*(inString +  retval) != '\0')
	{
		retval++;
	}
	return retval;
}

void uiToS(int num, char* buffer) {
	int i = 0;

	// Extract digits in reverse order
	do {
		buffer[i++] = num % 10 + '0';
		num /= 10;
	} while (num > 0);

	// Reverse the string
	for (int j = 0; j < i / 2; j++) {
		char temp = buffer[j];
		buffer[j] = buffer[i - j - 1];
		buffer[i - j - 1] = temp;
	}

	// Null-terminate the string
	buffer[i] = '\0';
}


void dumpString (char * inString)
{
	uint8_t strlength = stringsize_from_ptr(inString);
	for (int i = 0; i < strlength; i++)
	{
		USART_TRANSMIT (*(inString + i));
	}
}

//* END SERIAL DEBUG *//

//* BUFFER INTERACTIONS *//

void motor_X_Buffer_Push()
{
	//dumpString("XPUSH\r\n");
	MINSB_X0.INST_LIST[MINSB_X0.INST_INDEX] = TFB0.INST;
	MINSB_X0.num_insts++;
	MINSB_X0.INST_INDEX++;
}

void motor_X_Buffer_Pop()
{
	//dumpString("XPOP\r\n");
	if (MINSB_X0.num_insts > 0)
	{
		size_t rest_of_buffer = sizeof(instruction) * (MINSB_X0.num_insts - 1); // size of the rest of the elements, minus one
		for (size_t i = 0; i < rest_of_buffer; ++i) {
			MINSB_X0.INST_LIST[i] = MINSB_X0.INST_LIST[i + 1];
		}
		MINSB_X0.INST_LIST[31] = (instruction){0};
		MINSB_X0.num_insts--;
		if (MINSB_X0.INST_INDEX > 0) {
			MINSB_X0.INST_INDEX--;
		}
	}
}

void motor_Y_Buffer_Push()
{
	//dumpString("YPUSH\r\n");
	MINSB_Y0.INST_LIST[MINSB_Y0.INST_INDEX] = TFB0.INST; // straight copy the instruction into the smallest available index
	MINSB_Y0.INST_INDEX++;  // move to the next index
	MINSB_Y0.num_insts++;
	// so at this point, the trace from USART input to an additive buffer operation ends here.
	// subtractive operations should occur in the INSTRUCTION_HANDLER function
}

void motor_Y_Buffer_Pop()
{
	//dumpString("YPOP\r\n");
	if (MINSB_Y0.num_insts > 0)
	{
		size_t rest_of_buffer = sizeof(instruction) * (MINSB_Y0.num_insts - 1); // size of the rest of the elements, minus one
		for (size_t i = 0; i < rest_of_buffer; ++i) {
			MINSB_Y0.INST_LIST[i] = MINSB_Y0.INST_LIST[i + 1];
		}
		MINSB_Y0.INST_LIST[31] = (instruction){0};
		MINSB_Y0.num_insts--;
		if (MINSB_Y0.INST_INDEX > 0) {
			MINSB_Y0.INST_INDEX--;
		}
	}
}

void WIPE_FRAME_BUFFER()
{
	//dumpString("WIPE_FRAME_BUFFER \r\n");
	TFB0.temp_ins_index = 0;
	TFB0.INST = (instruction){0}; //slam that bit field to 0.
}

void BUFFER_LOAD(char UDR0_data)
{
	//take that temp frame and chunk the incoming 8 bytes into the appropriate position on-frame
	*(&TFB0.INST.MOTOR + TFB0.temp_ins_index) = UDR0_data;
	TFB0.temp_ins_index++;
	//
	// TODO: implement some kind of guardrails mandating that index 0, 1 contain X,Y,n,c or drop packet
	//
	if (TFB0.temp_ins_index >= 4)     //completed frame force increments to four
	{
		if (TFB0.INST.MOTOR == 'X')
		{
			motor_X_Buffer_Push();
		}
		else if (TFB0.INST.MOTOR == 'Y')
		{
			motor_Y_Buffer_Push();
		}
		
		WIPE_FRAME_BUFFER();
	}
}

//* END BUFFER INTERACTIONS *//

//* PIN CONTROL *//

void SET_DIR(char DIR_INPUT, uint8_t MOTOR)
{
	//dumpString("Set Dir\r\n");
	switch (DIR_INPUT)
	{

		case ('C'):
		case ('c'):
		PORTD |= 1 << MOTOR;
		break;
		
		case ('N'):
		case ('n'):
		PORTD &= ~(1 << MOTOR);
		break;
		
		default:
	    // do nothing
		break;
	}
}

//* END PIN CONTROL *//

//* SYSTEM STATE*//


void LOAD_NEXT_ACTIVE_INSTRUCTIONS()
{
	
  ACTIVE_INST = (ACTIVE_INSTRUCTIONS) {0};
  if (MINSB_X0.num_insts >= 1)
  {
    ACTIVE_INST.MOTOR_X_FLAG = 1;
	ACTIVE_INST.MOTOR_X_DIRECTION =  MINSB_X0.INST_LIST[0].DIR;
	ACTIVE_INST.MOTOR_X_NSTEPS   |= (MINSB_X0.INST_LIST[0].HIGHval << 8) |
	                                (MINSB_X0.INST_LIST[0].LOWval);
    SET_DIR(ACTIVE_INST.MOTOR_X_DIRECTION, MOTOR_X_DIR_PIN);
	motor_X_Buffer_Pop();
	
  }
  if (MINSB_Y0.num_insts >= 1)
  {
	  ACTIVE_INST.MOTOR_Y_FLAG = 1;
	  ACTIVE_INST.MOTOR_Y_DIRECTION =  MINSB_Y0.INST_LIST[0].DIR;
	  ACTIVE_INST.MOTOR_Y_NSTEPS   |= (MINSB_Y0.INST_LIST[0].HIGHval << 8) |
	                                  (MINSB_Y0.INST_LIST[0].LOWval);
	  SET_DIR(ACTIVE_INST.MOTOR_Y_DIRECTION, MOTOR_Y_DIR_PIN);
	  motor_Y_Buffer_Pop(); 
  }
}


void SYSTEM_STATE_CONTROLLER()
{
	static uint8_t STATE = 0;
    char debugbuffer [20];
	switch (STATE)
	{
		case (0):
		while (!(MINSB_X0.num_insts | MINSB_Y0.num_insts));
		STATE = 1; 
		break;

		case (1):
		LOAD_NEXT_ACTIVE_INSTRUCTIONS();
		
		STATE = 2;
		break;
		
		case (2):
		TIMER1_INTERRUPTS_ON();
		while ((ACTIVE_INST.MOTOR_X_NSTEPS | ACTIVE_INST.MOTOR_Y_NSTEPS) != 0)
		{
			;
		}
		TIMER1_INTERRUPTS_OFF();
		STATE = 0;
		break;
	}
}

int main(void)
{
	TIMER1_INITIALIZATION();
	PINS_INITIALIZATION();
	UART_INIT();
	TIMER1_INTERRUPTS_OFF();
    while (1) 
    {
		SYSTEM_STATE_CONTROLLER();
    }
}

ISR (TIMER1_COMPA_vect)
{
	//USART_TRANSMIT('A');
	if (ACTIVE_INST.MOTOR_X_FLAG && (ACTIVE_INST.MOTOR_X_NSTEPS!=0))
	{
		PORTD &= ~(1 << MOTOR_X_PULSE_PIN); // pin 2 goes hi
	}
	if (ACTIVE_INST.MOTOR_Y_FLAG && (ACTIVE_INST.MOTOR_Y_NSTEPS!=0))
	{
		PORTD &= ~(1 << MOTOR_Y_PULSE_PIN); // pin 2 goes hi
	}
}

ISR(TIMER1_COMPB_vect)
{
	//USART_TRANSMIT('B');
	if (ACTIVE_INST.MOTOR_X_FLAG && (ACTIVE_INST.MOTOR_X_NSTEPS!=0))
	{
		PORTD |= (1 << MOTOR_X_PULSE_PIN); // pin 2 goes hi
		ACTIVE_INST.MOTOR_X_NSTEPS --;
	}
	if (ACTIVE_INST.MOTOR_Y_FLAG && (ACTIVE_INST.MOTOR_Y_NSTEPS!=0))
	{
		PORTD |= (1 << MOTOR_Y_PULSE_PIN); // pin 2 goes hi
		ACTIVE_INST.MOTOR_Y_NSTEPS --;
	}
    
}

ISR(USART_RX_vect)
{
	volatile uint8_t data = UDR0;
	USART_TRANSMIT(data);  //wasting cycles, this is just for debug
	BUFFER_LOAD(data); //this should be fairly self documenting
}