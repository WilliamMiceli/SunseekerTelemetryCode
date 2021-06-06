#include "SunseekerTelemetry2021.h"
#include "clock_init.h"
#include "interrupts.h"
#include "io_init.h"
#include "timers.h"

volatile unsigned char status_flag = FALSE;

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	                // Stop watchdog timer; `WDTPW` is the "WatchDog Timer PassWord", required for all `WDTCTL` operations
	_DINT();     		    					// Disables interrupts

	clock_init();								// Configure HF and LF clocks
	delay();

	timerB_init();                              // Initialize timer B
	delay();

	io_init();                                  // Initialize all input/output pins
	delay();

	P8OUT &= ~LEDY0;                            // Initially set LEDY0 to Low
	P8OUT |= LEDY1;                             // Initially set LEDY1 to High

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
