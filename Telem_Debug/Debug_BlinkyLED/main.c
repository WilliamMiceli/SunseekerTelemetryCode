/*
 * main.c
 */

#include "SunseekerTelemetry2021.h"

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	                // Stop watchdog timer
	_DINT();     		    					// Disables interrupts

	clock_init();								// Configure HF and LF clocks
	delay();

	P8DIR = LEDG | LEDR | LEDY0 | LEDY1;        // Set all four main LEDs as output
	P8OUT |= LEDG;                              // Turn on permanently the green LED

	_EINT(); 	                                //enable global interrupts

    while(1)
    {
    	P8OUT ^= LEDY0 | LEDY1;                 // Toggle both yellow LEDs
    	delay();
    }
}
