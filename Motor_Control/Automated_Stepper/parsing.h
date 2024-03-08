#ifndef STEPPER_PARSE_H
#define STEPPER_PARSE_H

#include "auto_stepper_resources.h"

#define IS_VALID(INPUT) (INPUT >= 'a' && INPUT <= 'd') || (INPUT >= 'A' && INPUT <= 'D') || (INPUT >= '0' && INPUT <= '9' )|| (INPUT == '%' || INPUT == '>')
#define isAlpha(INPUT)  (INPUT >= 'a' && INPUT <= 'd') || (INPUT >= 'A' && INPUT <= 'D')

typedef struct
{
    uint16_t STEPS_X;    // defining return type of motor action packet
    uint16_t STEPS_Y;
    
    char     DIR_X;     // assignment for motor and direction
    char     DIR_Y;
    char     STATUS;     // describes whether or not the current return value should be
                         // accepted as the number of steps to take
} Motor_Action;

typedef struct 
{
    uint8_t STRING_SEL;

    char  X [32];
    char  Y [32];

    uint8_t iX;
    uint8_t iY;
} xy;

uint16_t pow_(uint16_t base, uint16_t exp);
void find_steps(char* INPUT, char* DIR, uint16_t* STEPS, uint8_t SIZE);

Motor_Action string_parse (char INPUT);

void TEST();

#endif
