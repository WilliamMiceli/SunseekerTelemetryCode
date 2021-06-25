#include "SunseekerTelemetry2021.h"
#include "clock_init.h"
#include "interrupts.h"
#include "io_init.h"
#include "timers.h"
#include "usci.h"

long delayCounter = 0;
volatile unsigned char status_flag = FALSE;
char USBTransmitString[256];
char RS232TransmitString[] = "[DEBUG] Test Transmission String";
char RS232RelayString[256];
char* pUSBReceiveString;
char* pRS232ReceiveString;

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;                   // Stop watchdog timer; `WDTPW` is the "WatchDog Timer PassWord", required for all `WDTCTL` operations
    _DINT();                                    // Disable interrupts

    clock_init();                               // Configure HF and LF clocks
    delay();

    timerB_init();                              // Initialize timer B
    delay();

    io_init();                                  // Initialize all input/output pins
    delay();

    usci_init();                                // Initialize USCI modules
    delay();

    _EINT();                                    // Enable global interrupts

    P8OUT |= LEDY0;                             // Initially set LEDY0 to High

    sprintf(USBTransmitString, "Transmitted over RS-232: \"%s\"", RS232TransmitString);
    pUSBReceiveString = calloc(1, 256 * sizeof(*pUSBReceiveString) + 1);
    pRS232ReceiveString = calloc(1, 256 * sizeof(*pUSBReceiveString) + 1);

    usci_A2_enable();                           // Enable the USB interface
    usci_A2_enableInterrupt();                  // Enable interrupts for the USB interface
    usci_A3_enable();                           // Enable the RS-232 interface
    usci_A3_enableInterrupt();                  // Enable interrupts for the RS-232 interface

    while(1)
    {
        // Repeating debug transmission string
        if(delayCounter == 0){
            usci_A3_transmitString(&RS232TransmitString[0]);
            usci_A2_transmitString(&USBTransmitString[0]);
        }else{
            ++delayCounter;
            delayCounter %= 10000;
        }

        // String received, to be transmitted back
        if(pRS232ReceiveString[0] != '\0'){
            sprintf(RS232RelayString, "Received and Re-Transmitted over RS-232: \"%s\"", pRS232ReceiveString);
            usci_A2_transmitString(&RS232RelayString[0]);
            usci_A3_transmitString(pRS232ReceiveString);
            pRS232ReceiveString[0] = '\0';
        }
    }
}
