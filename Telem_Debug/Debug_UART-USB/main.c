#include "SunseekerTelemetry2021.h"
#include "clock_init.h"
#include "interrupts.h"
#include "io_init.h"
#include "timers.h"
#include "usci.h"

volatile unsigned char status_flag = FALSE;

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	                // Stop watchdog timer; `WDTPW` is the "WatchDog Timer PassWord", required for all `WDTCTL` operations
	_DINT();     		    					// Disable interrupts

	clock_init();								// Configure HF and LF clocks
	delay();

	timerB_init();                              // Initialize timer B
	delay();

	io_init();                                  // Initialize all input/output pins
	delay();

	usci_init();                                // Initialize USCI modules
	delay();

	P8OUT &= ~LEDY0;                            // Initially set LEDY0 to High
	P8OUT |= LEDY1;                             // Initially set LEDY1 to Low

	_EINT(); 	                                // Enable global interrupts

    while(1)
    {

    }
}
