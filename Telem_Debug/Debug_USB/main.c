#include "SunseekerTelemetry2021.h"
#include "clock_init.h"
#include "interrupts.h"
#include "io_init.h"
#include "timers.h"
#include "usci.h"

long delayCounter = 0;
volatile unsigned char status_flag = FALSE;
char USBTransmitString[] = "[DEBUG] Test Transmission String";
char* pUSBReceiveString;

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

    pUSBReceiveString = calloc(1, 256 * sizeof(*pUSBReceiveString) + 1);

    usci_A2_enable();                           // Enable the USB interface
    usci_A2_enableInterrupt();                  // Enable interrupts for the USB interface

    while(1)
    {
        // Repeating debug transmission string
        if(delayCounter == 0){
            usci_A2_transmitString(&USBTransmitString[0]);
        }else{
            ++delayCounter;
            delayCounter %= 10000;
        }

        // String received, to be transmitted back
        if(pUSBReceiveString[0] != '\0'){
            usci_A2_transmitString(pUSBReceiveString);
            pUSBReceiveString[0] = '\0';
        }
    }
}
