
#ifndef AUTO_STEPPER_RESOURCES
#define AUTO_STEPPER_RESOURCES

#include <stdio.h>
#include <string.h>
#include <stdint.h> // don't think this will be necessary for arduino implementation
#include <avr/io.h>
#include <avr/interrupt.h>

#define BUFFER_I_MAX 63

#define IS_VALID(INPUT) (INPUT >= 'a' && INPUT <= 'd') || (INPUT >= 'A' && INPUT <= 'D') || (INPUT >= '0' && INPUT <= '9')
#define isAlpha(INPUT)  (INPUT >= 'a' && INPUT <= 'd') || (INPUT >= 'A' && INPUT <= 'D')

#endif