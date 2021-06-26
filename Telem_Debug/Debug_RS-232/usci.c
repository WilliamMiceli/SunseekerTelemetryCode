#include "usci.h"

void usci_init(void){
    usci_A0_init();
    usci_A1_init();
    usci_A2_init();
    usci_A3_init();
    usci_B0_init();
    usci_B1_init();
    usci_B2_init();
    usci_B3_init();
}
    /*************************************************************************/
    /******************************** USCI A0 ********************************/
    /*************************************************************************/

void usci_A0_init(void){

}

    /*************************************************************************/
    /******************************** USCI A1 ********************************/
    /*************************************************************************/

void usci_A1_init(void){

}

    /*************************************************************************/
    /******************************** USCI A2 ********************************/
    /*************************************************************************/

void usci_A2_init(void){
    UCA2CTL1 |= UCSWRST;                                                        // Software Reset Enable - Set high, disabling the USCI module; Changes to USCI configuration registers can only be made when the UCSWRST bit = 1
    UCA2CTL0 &= ~UCPEN;                                                         // Parity Enable - Disabled
    UCA2CTL0 &= ~UCMSB;                                                         // MSB First Select - Set low; Least Significant Bit will be sent first
    UCA2CTL0 &= ~UC7BIT;                                                        // Character Length - 8-bit data selected
    UCA2CTL0 &= ~UCSPB;                                                         // Stop Bit Select - One stop bit selected
    UCA2CTL0 &= ~UCMODE1;                                                       // USCI Mode [2 Bits Required] - UART mode selected
    UCA2CTL0 &= ~UCMODE0;                                                       // USCI Mode [2 Bits Required] - UART mode selected
    UCA2CTL0 &= ~UCSYNC;                                                        // Synchronous Mode Enable - Asynchronous mode selected
    UCA2CTL1 &= ~0b11000000;                                                    // Reset both UCSSELx bits so the USCI Clock Source Select bits are more predictable
    UCA2CTL1 |= UCSSEL__SMCLK;                                                  // USCI Clock Source Select - SMCLK
    UCA2CTL1 &= ~UCRXEIE;                                                       // Receive Erroneous-Character Interrupt Enable - Disabled
    UCA2CTL1 &= ~UCBRKIE;                                                       // Receive Break Character Interrupt Enable - Disabled
    UCA2CTL1 &= ~UCDORM;                                                        // Dormant - Disabled; USCI module will not be put into sleep mode
    UCA2CTL1 &= ~UCTXADDR;                                                      // Transmit Address - Next frame transmitted is data
    UCA2CTL1 &= ~UCTXBRK;                                                       // Transmit Break - Do not transmit a Break/Synch as the next frame
    UCA2BRW = 10;                                                               // Clock Prescalar
    UCA2MCTL &= ~0b11111111;                                                    // Reset entire register
    UCA2MCTL |= 0x0 << 4;                                                       // First Modulation Stage Select
    UCA2MCTL |= 0x0 << 1;                                                       // Second Modulation Stage Select
    UCA2MCTL |= 0x0;                                                            // Oversampling Mode Enable - Disabled
}

void usci_A2_enable(void){
    UCA2CTL1 &= ~UCSWRST;                                                       // Software Reset Enable - Set low, enabling the USCI module
}

void usci_A2_disable(void){
    UCA2CTL1 |= UCSWRST;                                                        // Software Reset Enable - Set high, disabling the USCI module
}

void usci_A2_enableInterrupt(void){
    UCA2IE |= (UCTXIE | UCRXIE);                                                // Enable transmit and receive interrupts
}

void usci_A2_disableInterrupt(void){
    UCA2IE &= ~(UCTXIE | UCRXIE);                                               // Disable transmit and receive interrupts
}

void usci_A2_transmitChar(char charToTransmit){
    while((UCA2IFG & UCTXIFG) == 0){                                            // If the USCI module is not yet ready for the next value to transmit, wait indefinitely
        _NOP();                                                                 // Processor will do nothing until loop exits
    }
    UCA2TXBUF = charToTransmit;                                                 // When USCI module signals that it is ready, the next character value is moved into the transmit buffer
}

char usci_A2_receiveChar(void){
    short i = 100;                                                              // Initialize a countdown

    while (((UCA2IFG & UCRXIFG) == 0) && (i > 0)){                              // Wait until either the end of the countdown or when the UCA2IFG register signals that the receive buffer is ready
        --i;
    }
    return(UCA2RXBUF);                                                          // Return the receive buffer regardless of whether it is ready or not
}

int usci_A2_transmitString(char* pString){
    short i = 0;                                                                // Initialize a counter of how many characters were transmitted
    char nextChar = *pString;                                                   // Load the first character in the string array from the pointer

    while(nextChar != '\0'){                                                    // Unless the next character in the string array is a null terminator, continue sending more characters
        usci_A2_transmitChar(nextChar);                                         // Transmit the next character, as it is not a null terminator
        ++i;                                                                    // Increment the counter
        ++pString;                                                              // Move the pointer to the next memory location, which is storing the next character in the string array
        nextChar = *pString;                                                    // Load the next character from memory
    }

    usci_A2_transmitChar('\n');                                                 // Transmit a "New Line" character
    usci_A2_transmitChar('\r');                                                 // Transmit a "Carriage Return" character

    return(i);                                                                  // Return the counter of how many characters were transmitted, not including the "New Line" and "Carriage Return" characters
}

int usci_A2_receiveString(char* pString){
    short i = 0;                                                                // Initialize a counter of how many characters were received

    while(1){                                                                   // Continue looping until a "Carriage Return" character is received
        *pString = usci_A2_receiveChar();                                       // Store the received character in the designated memory location

        if(*pString == '\r'){                                                   // If a "Carriage Return" character is found:
            *pString = '\0';                                                    // Overwrite it with a null terminator
            return(i);                                                          // End receiving new characters and return the count of characters received
        }else{                                                                  // Else:
            ++pString;                                                          // Increment the pointer to the next memory location for the next character
            ++i;                                                                // Increment the counter of received characters
        }
    }
}

    /*************************************************************************/
    /******************************** USCI A3 ********************************/
    /*************************************************************************/

void usci_A3_init(void){
    UCA3CTL1 |= UCSWRST;                                                        // Software Reset Enable - Set high, disabling the USCI module; Changes to USCI configuration registers can only be made when the UCSWRST bit = 1
    UCA3CTL1 &= ~0b11000000;                                                    // Reset both UCSSELx bits so the USCI Clock Source Select bits are more predictable
    UCA3CTL1 |= UCSSEL__SMCLK;                                                  // USCI Clock Source Select - SMCLK
    UCA3CTL1 &= ~UCRXEIE;                                                       // Receive Erroneous-Character Interrupt Enable - Disabled
    UCA3CTL1 &= ~UCBRKIE;                                                       // Receive Break Character Interrupt Enable - Disabled
    UCA3CTL1 &= ~UCDORM;                                                        // Dormant - Disabled; USCI module will not be put into sleep mode
    UCA3CTL1 &= ~UCTXADDR;                                                      // Transmit Address - Next frame transmitted is data
    UCA3CTL1 &= ~UCTXBRK;                                                       // Transmit Break - Do not transmit a Break/Synch as the next frame
    UCA3CTL0 &= ~UCPEN;                                                         // Parity Enable - Disabled
    UCA3CTL0 &= ~UCMSB;                                                         // MSB First Select - Set low; Least Significant Bit will be sent first
    UCA3CTL0 &= ~UC7BIT;                                                        // Character Length - 8-bit data selected
    UCA3CTL0 &= ~UCSPB;                                                         // Stop Bit Select - One stop bit selected
    UCA3CTL0 &= ~UCMODE1;                                                       // USCI Mode [2 Bits Required] - UART mode selected
    UCA3CTL0 &= ~UCMODE0;                                                       // USCI Mode [2 Bits Required] - UART mode selected
    UCA3CTL0 &= ~UCSYNC;                                                        // Synchronous Mode Enable - Asynchronous mode selected
    UCA3BRW = 65;                                                               // Clock Prescalar
    UCA3MCTL &= ~0b11111111;                                                    // Reset entire register
    UCA3MCTL |= 0x2 << 4;                                                       // First Modulation Stage Select
    UCA3MCTL |= 0x0 << 1;                                                       // Second Modulation Stage Select
    UCA3MCTL |= 0x1;                                                            // Oversampling Mode Enable - Enabled
}

void usci_A3_enable(void){
    UCA3CTL1 &= ~UCSWRST;                                                       // Software Reset Enable - Set low, enabling the USCI module
}

void usci_A3_disable(void){
    UCA3CTL1 |= UCSWRST;                                                        // Software Reset Enable - Set high, disabling the USCI module
}

void usci_A3_enableInterrupt(void){
    UCA3IE |= (UCTXIE | UCRXIE);                                                // Enable transmit and receive interrupts
}

void usci_A3_disableInterrupt(void){
    UCA3IE &= ~(UCTXIE | UCRXIE);                                               // Disable transmit and receive interrupts
}

void usci_A3_transmitChar(char charToTransmit){
    while((UCA3IFG & UCTXIFG) == 0){                                            // If the USCI module is not yet ready for the next value to transmit, wait indefinitely
        _NOP();                                                                 // Processor will do nothing until loop exits
    }
    UCA3TXBUF = charToTransmit;                                                 // When USCI module signals that it is ready, the next character value is moved into the transmit buffer
}

char usci_A3_receiveChar(void){
    short i = 100;                                                              // Initialize a countdown

    while (((UCA3IFG & UCRXIFG) == 0) && (i > 0)){                              // Wait until either the end of the countdown or when the UCA3IFG register signals that the receive buffer is ready
        --i;
    }
    return(UCA3RXBUF);                                                          // Return the receive buffer regardless of whether it is ready or not
}

int usci_A3_transmitString(char* pString){
    short i = 0;                                                                // Initialize a counter of how many characters were transmitted
    char nextChar = *pString;                                                   // Load the first character in the string array from the pointer

    while(nextChar != '\0'){                                                    // Unless the next character in the string array is a null terminator, continue sending more characters
        usci_A3_transmitChar(nextChar);                                         // Transmit the next character, as it is not a null terminator
        ++i;                                                                    // Increment the counter
        ++pString;                                                              // Move the pointer to the next memory location, which is storing the next character in the string array
        nextChar = *pString;                                                    // Load the next character from memory
    }

    usci_A3_transmitChar('\n');                                                 // Transmit a "New Line" character
    usci_A3_transmitChar('\r');                                                 // Transmit a "Carriage Return" character

    return(i);                                                                  // Return the counter of how many characters were transmitted, not including the "New Line" and "Carriage Return" characters
}

int usci_A3_receiveString(char* pString){
    short i = 0;                                                                // Initialize a counter of how many characters were received

    while(1){                                                                   // Continue looping until a "Carriage Return" character is received
        *pString = usci_A3_receiveChar();                                       // Store the received character in the designated memory location

        if(*pString == '\r'){                                                   // If a "Carriage Return" character is found:
            *pString = '\0';                                                    // Overwrite it with a null terminator
            return(i);                                                          // End receiving new characters and return the count of characters received
        }else{                                                                  // Else:
            ++pString;                                                          // Increment the pointer to the next memory location for the next character
            ++i;                                                                // Increment the counter of received characters
        }
    }
}

    /*************************************************************************/
    /******************************** USCI B0 ********************************/
    /*************************************************************************/

void usci_B0_init(void){

}

    /*************************************************************************/
    /******************************** USCI B1 ********************************/
    /*************************************************************************/

void usci_B1_init(void){

}

    /*************************************************************************/
    /******************************** USCI B2 ********************************/
    /*************************************************************************/

void usci_B2_init(void){

}

    /*************************************************************************/
    /******************************** USCI B3 ********************************/
    /*************************************************************************/

void usci_B3_init(void){

}
