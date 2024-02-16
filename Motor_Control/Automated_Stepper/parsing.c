/*
  * 15 FEB 2024
  * Zaid Yasin
*/
#define BUFFER_I_MAX 63

#define isAlpha(INPUT) (INPUT >= 'a' && INPUT <= 'd') || (INPUT >= 'A' && INPUT <= 'D')

#include <stdio.h>
#include <string.h>
#include <stdint.h> // don't think this will be necessary for arduino implementation


typedef struct
{
    uint32_t STEPS;      // defining return type of motor action packet
    char     ASSIGN;     // assignment for motor and direction
    char     STATUS;     // describes whether or not the current return value should be
                         /* accepted as the number of steps to take*/
} Motor_Action;

/////                                                                             /////
//IS_VALID(char INPUT) determines whether the input character is integer OR "A,S,D,F"//
/////                                                                             /////

uint32_t pow_ (uint32_t base, uint32_t exp) // custom pow function because i am too stubborn to include math.h on a resource constrained system.
    {
      if(exp < 0) 
        return 0;
        
        uint32_t result = 1;
        while (exp)
        {
            if (exp & 1)
            result *= base;
            exp >>= 1;
            printf("%x\n", exp);
            base *= base;
        }
        return result;
    }


uint8_t IS_VALID(char INPUT) {
    
    if ((INPUT >= 'a' && INPUT <= 'd') || (INPUT >= 'A' && INPUT <= 'D') || (INPUT >= '0' && INPUT <= '9'))
      return 0;
      
    else 
      return 1;     
}

uint32_t sTou32 (char * INPUT, uint8_t SIZE)
{
    uint32_t sum = 0;
    uint32_t iter_j = SIZE;
    
    char* BUFFER = INPUT;
    while (*BUFFER != '>')
    {
	 // for all values in buffer c*10^[MAX_SIZE - {ITER}]
	 // where ITER = MAXSIZE - iterations
	  
	  sum += ((*BUFFER-'0') * pow_ (10,(SIZE - (SIZE - iter_j))));
	  BUFFER ++;
	  iter_j--;
	}
	printf ("SUM IS: %d\n", sum);
	return sum;
};

// this returns a struct of type uint32_t and two chars

Motor_Action string_parse(char INPUT)
{
    
    
    Motor_Action INTERNAL_ACTION;
    
    static char CONV_SPHOR  =  0;  // establish semaphore control for string parse routine
    static char BUFFER [64] = {0}; //initalize buffer, set all chars to zero
    
    static uint8_t iter_i = 0;
    
    INTERNAL_ACTION.STATUS = '0';    
    
    if (CONV_SPHOR == 0 && INPUT == '<')  //starting character 
    {
     CONV_SPHOR = 1;                      //set semaphore busy
     memset (BUFFER, '0', sizeof(BUFFER));  // clear buffer
    
     INTERNAL_ACTION.STATUS = '0';
    
     return INTERNAL_ACTION;
    }
    
    //loading the buffer.
    
    else if (CONV_SPHOR == 1 && IS_VALID(INPUT) == 0 && iter_i <= (BUFFER_I_MAX-1))
    {
      BUFFER[iter_i] = INPUT;
      
      if (isAlpha(INPUT))  
      {
		  BUFFER[iter_i + 1] = '\n';
          // this is your motor & direction sel.
          // A: M1 CW     C: M2 CW
          // B: M1 CCW    D: M2 CCW
          
          INTERNAL_ACTION.ASSIGN = INPUT;
          BUFFER[iter_i] = '>';
          INTERNAL_ACTION.STEPS  = sTou32(BUFFER, (iter_i-1));
          INTERNAL_ACTION.STATUS = '1';
          CONV_SPHOR = 0;                        //free semaphore
          return INTERNAL_ACTION;
      }
     
      iter_i ++; //buffer effective size becomes 62. this is still excessive.
      
      
    }
    INTERNAL_ACTION.STATUS = '0';
    return INTERNAL_ACTION;
}

/*

in an interrupt function

UDR0 value loaded into INPUT

placeholder_name = string_parse(INPUT)

if (placeholder_name.status)
{
    // do motor control stuff based off ASSIGN and STEPS
}

otherwise, exit interrupt

*/
int main()
{
    char SHABL [10] = {'<','1','3','6','5','3','2','A','0','0'};
    
    Motor_Action retval;
    
    for (int i = 0; i < 10; i++)
    {
       retval = string_parse(SHABL[i]);
    }
    
    printf("DIGIT : %u\n", retval.STEPS);
    printf("ASSGN : %c\n", retval.ASSIGN);
    
    return 0;
}