
//////////////////////////////////////////////////////////////////
// This code exists as boilerplate for a serial based control   // 
// of a simple two-axis gimbal.'W' and 'A' keys control Axis 1, //
// Axis 2 is controlled by the 'S' and D keys.'Q' toggles laser //
//////////////////////////////////////////////////////////////////

// (≥^◕ᄌ◕^≤) meowwwwwwww

#include "actions.h"

int main(void)
{
    UART_INIT();
    PIN_INIT();
    TIMER1_INIT();
    
	sei();

    while (1){};
}