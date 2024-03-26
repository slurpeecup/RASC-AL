#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define BUFFER_I_MAX 63
#define BUFFER_SIZE 20

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
    instruction INST_LIST[32];
    uint8_t INST_INDEX;
    uint8_t num_insts;
} instruction_buffer;

typedef struct motor_mux {
    instruction *X_INSTRUCTION;
    instruction *Y_INSTRUCTION;
    uint16_t X_STEPS;
    uint16_t Y_STEPS;
    uint8_t MOTOR_SEL;
} motor_mux;

//function prototypes
void stepper_ENA();
void stepper_DA();
void UART_INIT();
void PINS_INIT();
void TIMER1_INIT();
void INTERRUPT_INIT();
void INSTRUCTION_HANDLER ();
void USART_TRANSMIT(char data);
void dumpString (char * inString);
#define isMotor(input) (input == 'X') || (input == 'x') || (input == 'Y') || (input == 'y')
#define isDIR(input)   (input == 'C') || (input == 'c') || (input == 'N') || (input == 'n')

#define steppers_on  0b00000110
#define steppers_off 0

#define MOTOR_X_PULSE_PIN 3
#define MOTOR_Y_PULSE_PIN 5

#define MOTOR_X_DIR_PIN 4
#define MOTOR_Y_DIR_PIN 6

#define MOTOR_ENA_PIN 2

#define DEREF_INST_BUFFER_X_and_INC (MINSB_Y0.INST_LIST + (MINSB_Y0.INST_INDEX++))
#define DEREF_INST_BUFFER_Y_and_INC (MINSB_X0.INST_LIST + (MINSB_X0.INST_INDEX++))

//########## GLOBAL DEFINITIONS ###############
input_frame_buffer TFB0 = {
  {0, 0, 0, 0}, // INST
  0             // temp_ins_index
};

instruction_buffer MINSB_X0 = {
  {0}, // INST_LIST
  0,   // INST_INDEX
  0    // num_insts
};

instruction_buffer MINSB_Y0 = {
  {0}, // INST_LIST
  0,   // INST_INDEX
  0    // num_insts
};

motor_mux mux_handler = {
  &MINSB_X0.INST_LIST[0], // X_INSTRUCTION
  &MINSB_Y0.INST_LIST[0], // Y_INSTRUCTION
  0,                      // X_STEPS
  0,                      // Y_STEPS
  0                       // MOTOR_SEL
};

//########## INITIALIZATION FUNCS ###############
void stepper_ENA(){TIMSK1 &= steppers_on;}
void stepper_DA(){TIMSK1 &= steppers_off;}

void UART_INIT()
{
  cli();
  UBRR0 = 12; //76.8kb/s with U2X0 = 0; if this no work try UBRR0 = 25, 38.4 kb/s
 
  UCSR0A &= ~(1<<U2X0);    // no division
  UCSR0B |= 0b10011000;     // Receive enable, receive interrupt enable, character size 8 bit.
  UCSR0C |= 0b00000110;     // Character size 8 bit, asynchronous UART, 1 stop bit, no parity check

  sei();  
}

void PINS_INIT()
{
  DDRB |= (1 << MOTOR_ENA_PIN)   | (1<<MOTOR_X_PULSE_PIN)   | 
          (1 << MOTOR_X_DIR_PIN) | (1 << MOTOR_X_PULSE_PIN) |
          (1 << MOTOR_Y_DIR_PIN);

  PORTB = 0;
}

void TIMER1_INIT()
{
  OCR1A = 8000;
  OCR1B = 4000;

  TCCR1A |= 0b00000010; //disconnect COMS, mode of operation 14
  TCCR1B |= 0b00011001; // mode 14, 1/1 prescale
  TIMSK1 |= 0b00000110; // COMPARE MATCH A,B interrupts;
  ICR1L  |= 8000;
  ICR1H  |= 8000 >> 8;
}

//################################################ END INITIALIZATIONS

void motor_X_Buffer_Push()
{
  MINSB_X0.INST_LIST[MINSB_X0.INST_INDEX] = TFB0.INST;
  MINSB_X0.num_insts++;
  MINSB_X0.INST_INDEX++;
}

void motor_X_Buffer_Pop()
{
  if (MINSB_X0.num_insts > 0) 
  {
    size_t rest_of_buffer = sizeof(instruction) * (MINSB_X0.num_insts - 1);
    memmove(MINSB_X0.INST_LIST, MINSB_X0.INST_LIST + 1, rest_of_buffer);
    memset(&MINSB_X0.INST_LIST[31], 0, sizeof(instruction));
    MINSB_X0.num_insts--;
    if (MINSB_X0.INST_INDEX > 0) {
        MINSB_X0.INST_INDEX--;
    }
  }
}

void motor_Y_Buffer_Push()
{
  MINSB_Y0.INST_LIST[MINSB_Y0.INST_INDEX] = TFB0.INST;
  MINSB_Y0.INST_INDEX++;
  MINSB_Y0.num_insts++;    
}

void motor_Y_Buffer_Pop()
{
  if (MINSB_X0.num_insts > 0) 
  {
    size_t rest_of_buffer = sizeof(instruction) * (MINSB_Y0.num_insts - 1);
    memmove(MINSB_Y0.INST_LIST, MINSB_Y0.INST_LIST + 1, rest_of_buffer);
    memset(&MINSB_Y0.INST_LIST[31], 0, sizeof(instruction));
    MINSB_Y0.num_insts--;
    if (MINSB_Y0.INST_INDEX > 0) {
        MINSB_Y0.INST_INDEX--;
    }
  }
}

void debug_Prints()
{
    char buffer[20]; // Buffer for converting integers to strings

    dumpString("MINSB_X0: MOTOR: ");
    USART_TRANSMIT(MINSB_X0.INST_LIST[0].MOTOR);
    dumpString(", INST_INDEX: ");
    itoa(MINSB_X0.INST_INDEX, buffer, 10);
    dumpString(buffer);
    dumpString(", num_insts: ");
    itoa(MINSB_X0.num_insts, buffer, 10);
    dumpString(buffer);
    dumpString("\n");

    for (int i = 0; i < MINSB_X0.num_insts; ++i) {
        dumpString("MINSB_X0 INST[");
        itoa(i, buffer, 10);
        dumpString(buffer);
        dumpString("]: MOTOR: ");
        USART_TRANSMIT(MINSB_X0.INST_LIST[i].MOTOR);
        dumpString(", DIR: ");
        USART_TRANSMIT(MINSB_X0.INST_LIST[i].DIR);
        dumpString(", LOWval: ");
        itoa(MINSB_X0.INST_LIST[i].LOWval, buffer, 10);
        dumpString(buffer);
        dumpString(", HIGHval: ");
        itoa(MINSB_X0.INST_LIST[i].HIGHval, buffer, 10);
        dumpString(buffer);
        dumpString("\n");
    }

    dumpString("MINSB_Y0: MOTOR: ");
    USART_TRANSMIT(MINSB_Y0.INST_LIST[0].MOTOR);
    dumpString(", INST_INDEX: ");
    itoa(MINSB_Y0.INST_INDEX, buffer, 10);
    dumpString(buffer);
    dumpString(", num_insts: ");
    itoa(MINSB_Y0.num_insts, buffer, 10);
    dumpString(buffer);
    dumpString("\n");

    for (int i = 0; i < MINSB_Y0.num_insts; i++) {
        dumpString("MINSB_Y0 INST[");
        itoa(i, buffer, 10);
        dumpString(buffer);
        dumpString("]: MOTOR: ");
        USART_TRANSMIT(MINSB_Y0.INST_LIST[i].MOTOR);
        dumpString(", DIR: ");
        USART_TRANSMIT(MINSB_Y0.INST_LIST[i].DIR);
        dumpString(", LOWval: ");
        itoa(MINSB_Y0.INST_LIST[i].LOWval, buffer, 10);
        dumpString(buffer);
        dumpString(", HIGHval: ");
        itoa(MINSB_Y0.INST_LIST[i].HIGHval, buffer, 10);
        dumpString(buffer);
        dumpString("\n");
    }

    dumpString("NEXT\n");
}


void BUFFER_LOAD(char UDR0_data) {
    memcpy(&TFB0.INST.MOTOR + TFB0.temp_ins_index, &UDR0_data, sizeof(char));
    TFB0.temp_ins_index++;

    if (TFB0.temp_ins_index >= 4) {
        if (TFB0.INST.DIR == 'X') {
            TFB0.INST.MOTOR = 'X';
        } else if (TFB0.INST.DIR == 'Y') {
            TFB0.INST.MOTOR = 'Y';
        }
        
        printf("MOTOR: %c\n", TFB0.INST.MOTOR);
        
        if (TFB0.INST.MOTOR == 'X') {
            motor_X_Buffer_Push();
            debug_Prints();
            
        } else if (TFB0.INST.MOTOR == 'Y') {
          motor_Y_Buffer_Push();
          debug_Prints();
        }
        TFB0.temp_ins_index = 0; 
        memset(&TFB0.INST, 0, sizeof(instruction));
    }
}

void BUFFER_UNLOAD(char BUFFER_SEL)
{
  
  if (BUFFER_SEL  == 'X')
  {
    motor_X_Buffer_Pop();
  }
  
  else if (BUFFER_SEL == 'Y')
  {
    motor_Y_Buffer_Pop();  
  }
  
}

//########## UTILITY FUNCS ###############
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

void dumpString (char * inString)
{
    uint8_t strlength = stringsize_from_ptr(inString);
    for (int i = 0; i < strlength; i++)
    {
        USART_TRANSMIT(*(inString + i));
    }
}

//###################################### END UTILITY FUNCS

//########## MOTOR HANDLING ###############

void SET_DIR(char DIR_INPUT, uint8_t MOTOR)
{
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
    break;
  }
}

void INSTRUCTION_HANDLER ()
{
  uint16_t stepsX; 
  uint16_t stepsY; 
  char X_DIR;
  char Y_DIR;

  uint8_t active_INSTS = 0;

  if (MINSB_X0.num_insts > 0)
  {
    mux_handler.X_STEPS = (MINSB_X0.INST_LIST[0].HIGHval << 8) | (MINSB_X0.INST_LIST[0].LOWval );
    SET_DIR(MINSB_X0.INST_LIST[0].DIR, MOTOR_X_DIR_PIN);
    stepper_ENA();
    active_INSTS += 4;
  }

  if (MINSB_Y0.num_insts > 0)
  {
    mux_handler.Y_STEPS = (MINSB_Y0.INST_LIST[0].HIGHval << 8) | (MINSB_Y0.INST_LIST[0].LOWval );
    SET_DIR(MINSB_Y0.INST_LIST[0].DIR, MOTOR_Y_DIR_PIN);
    stepper_ENA();
    active_INSTS += 3;
  }

  while ((stepsX > 0) && (stepsY > 0))
  {
    //interrupts on!
    //within the interrupt, muxes will mux
  }
  stepper_DA();

  switch (active_INSTS)
  {
    case (3):
      motor_Y_Buffer_Pop();
    break;
    case (4):
      motor_X_Buffer_Pop();
    break;
    case (7):
      motor_X_Buffer_Pop();
      motor_Y_Buffer_Pop();
    break;
  }
}

ISR (TIMER1_COMPA_vect)
{
  switch (mux_handler.X_INSTRUCTION[0].MOTOR)
  {
    case ('X'):
      PORTB &= ~(1 << MOTOR_X_PULSE_PIN); // pin 1 goes low
      if (mux_handler.X_STEPS > 0)
      {
        mux_handler.X_STEPS --;
      }
      if (mux_handler.Y_STEPS > 0)
        mux_handler.MOTOR_SEL = 1;
    break;
    case ('Y'):
      PORTB &= ~(1 << MOTOR_Y_PULSE_PIN); // pin 2 goes low
      if (mux_handler.Y_STEPS > 0)
      {
        mux_handler.Y_STEPS --;
      }
      if (mux_handler.X_STEPS > 0)
        mux_handler.MOTOR_SEL = 0;
    break;
    default:
    break;
  }
  if ((!mux_handler.X_STEPS && !mux_handler.X_STEPS))
  {
    stepper_DA();
  }
}

ISR(TIMER1_COMPB_vect)
{
  switch (mux_handler.MOTOR_SEL)
  {
    case (0):
      PORTB |= (1 << MOTOR_X_PULSE_PIN); // pin 2 goes hi
    break;
    case (1):
      PORTB |= ~(1 << MOTOR_Y_PULSE_PIN); // pin 2 goes hi
    break;
    default:
    break;
  }
}

ISR(USART_RX_vect)
{
  volatile uint8_t data = UDR0;
  BUFFER_LOAD(data);
}

char* startup_string = "I'm awake!\n";

int main()
{
    UART_INIT();
    PINS_INIT();
    TIMER1_INIT();
    
    dumpString(startup_string);
    
    while (1)
    {
      INSTRUCTION_HANDLER();
    }
    return 0;
}

