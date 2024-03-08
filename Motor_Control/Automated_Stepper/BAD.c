
#define IS_VALID(INPUT) (INPUT >= 'a' && INPUT <= 'b') || (INPUT >= 'A' && INPUT <= 'B') || (INPUT >= '0' && INPUT <= '9' )|| (INPUT == '%' || INPUT == '>')
#define isAlpha(INPUT)  (INPUT >= 'a' && INPUT <= 'b') || (INPUT >= 'A' && INPUT <= 'B')


#include <stdio.h>
#include <string.h>
#include <stdint.h>

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

// custom pow function because I am too stubborn to include math.h on a resource-constrained system.
uint16_t pow_(uint16_t base, uint16_t exp) 
{
    if (exp < 0) 
        return 0;
        
    uint16_t result = 1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        base *= base;
    }
    return result;
}

void find_steps(char* INPUT, char* DIR, uint16_t* STEPS, uint8_t SIZE)
{
    uint16_t sum    = 0;
    uint16_t iter_j = 1;
    
    printf("\nInitial_Size: %d\n", SIZE);    
    printf("BUFFER_IN: %s \n",INPUT);
    
    *DIR   = INPUT[(SIZE)]; // extract direction character from EOS
    INPUT[(SIZE)] = '>';    // replace direction char with terminator
    

    char *BUFFER = INPUT;
    while (*BUFFER != '>')
    {
        // for all values in buffer c*10^[MAX_ORDER - {ITERATIONS}]
        sum += (((*BUFFER) - 48) * pow_(10, (SIZE - iter_j)));
        BUFFER++;
        iter_j++;
    }
    printf("SUM IS: %d\n", sum);

    *STEPS = sum;
    ;
}

// this returns a struct of type uint16_t and two chars

Motor_Action string_parse(char INPUT)
{
    Motor_Action INTERNAL;
    
    static char CONV_SPHOR  =  0;  // establish semaphore control for string parse routine
    static char BUFFER[64] = {0}; // initialize buffer, set all chars to zero
    
    static uint8_t iter_i = 0;
    
    static xy XY;


    INTERNAL.STATUS = '0';    
    
    static char XY_SEL = 'X';

    if (CONV_SPHOR == 0 && INPUT == '<')  // starting character 
    {
        CONV_SPHOR = 1;                      // set semaphore busy
        memset(BUFFER, '0', sizeof(BUFFER));  // clear buffer
        
        INTERNAL.STATUS = '0';

        iter_i = 0;
        XY.iX  = 0;
        XY.iY  = 0;
        
        return INTERNAL;
    }
    
    else if (CONV_SPHOR == 1 && (INPUT == '%'))
    {
      XY_SEL = 'Y';
      INTERNAL.DIR_X = XY.X[XY.iX-1];
      XY.X[XY.iX] = '>';
      iter_i++;
      INTERNAL.STATUS = '0';
      return INTERNAL;
    }

    // loading the buffer.
    else if (CONV_SPHOR == 1 && IS_VALID(INPUT))
    {
        //this line is funny because of how unreadable it is, so i'm keeping it.

        (XY_SEL == 'X') ? (XY.X[XY.iX++] = INPUT )//- '0') 
                        : (XY.Y[XY.iY++] = INPUT );//- '0');

        if (INPUT == '>')  
        {
            // this is your motor & direction sel.
            // A: CW     B:CCW
            
            INTERNAL.DIR_X = INPUT;

           // BUFFER[iter_i] = '>';
            
            find_steps(XY.X, &(INTERNAL.DIR_X), &(INTERNAL.STEPS_X), (XY.iX - 1));
            find_steps(XY.Y, &(INTERNAL.DIR_Y), &(INTERNAL.STEPS_Y), (XY.iY - 2));
            
            INTERNAL.STATUS = '1';
            CONV_SPHOR = 0;       // free semaphore
            XY_SEL = 'X';
            return INTERNAL;
        }
        
        iter_i++;
    }

    return INTERNAL;
}

int main()
{
    char demo[14] = {'<', '5', '6', '5', '3', '2', 'A','%', '4', '5','6','8','b','>'};
    
    Motor_Action retval;
    
    for (int i = 0; i < 14; i++)
    {
        retval = string_parse(demo[i]);
        
        if (retval.STATUS == '1')
        {
          printf("\n\n\nInitial string: %s\n\n", demo);
          printf("X : %u\n", retval.STEPS_X);
          printf("Y : %u\n", retval.STEPS_Y);
          printf("X_DIR : %c\n", retval.DIR_X);
          printf("Y_DIR : %c\n", retval.DIR_Y);
          printf("STATUS : %c\n\n", retval.STATUS);
        }
            
    }
    return 0;
}
