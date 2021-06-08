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

	_EINT(); 	                                // Enable global interrupts

    P8OUT |= LEDY0;                             // Initially set LEDY0 to High

    usci_A2_enable();
    usci_A2_enableInterrupt();

    char UARTTransmitString[] = "[DEBUG] Test Transmission String";
    char* pUARTReceiveString = calloc(1, 256 * sizeof(*pUARTReceiveString) + 1);

    while(1)
    {
        usci_A2_transmitString(&UARTTransmitString[0]);
        delayMultiplied(5000);
    }
}
