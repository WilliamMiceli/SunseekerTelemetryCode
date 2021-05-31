#include "SunseekerTelemetry2021.h"

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	                // Stop watchdog timer; `WDTPW` is the "WatchDog Timer PassWord", required for all `WDTCTL` operations
	_DINT();     		    					// Disables interrupts

	clock_init();								// Configure HF and LF clocks
	delay();

    P8DIR |= LEDG | LEDR | LEDY0 | LEDY1;       // Set all four main LEDs as output
    P8OUT |= LEDG;                              // Turn on permanently the green LED
    P8OUT &= ~(LEDY0 | LEDY1);                  // Initially set both yellow LEDs to Low

	_EINT(); 	                                // Enable global interrupts

	volatile int i;
    while(1)
    {
    	P8OUT ^= LEDY0 | LEDY1;                 // Toggle both yellow LEDs
    	for(i = 1000; i > 0; i--){
    		delay();
    	}
    }
}
