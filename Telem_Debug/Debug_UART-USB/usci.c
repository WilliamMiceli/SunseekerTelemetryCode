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
    UCA2CTL1 |= UCSWRST;                                                        // Software Reset Enable - Set high, disabling the USCI module; USCI configuration changes can only be made when the UCSWRST bit = 1
    UCA2CTL1 &= ~0b11000000;                                                    // Reset both UCSSELx bits so the USCI Clock Source Select bits are more predictable
    UCA2CTL1 |= UCSSEL__SMCLK;                                                  // USCI Clock Source Select - SMCLK
    UCA2CTL1 &= ~UCRXEIE;                                                       // Receive Erroneous-Character Interrupt Enable - Disabled
    UCA2CTL1 &= ~UCBRKIE;                                                       // Receive Break Character Interrupt Enable - Disabled
    UCA2CTL1 &= ~UCDORM;                                                        // Dormant - Disabled; USCI module will not be put into sleep mode
    UCA2CTL1 &= ~UCTXADDR;                                                      // Transmit Address - Next frame transmitted is data
    UCA2CTL1 &= ~UCTXBRK;                                                       // Transmit Break - Do not transmit a Break/Synch as the next frame
    UCA2CTL0 &= ~UCPEN;                                                         // Parity Enable - Disabled
    UCA2CTL0 &= ~UCMSB;                                                         // MSB First Select - Set low; Least Significant Bit will be sent first
    UCA2CTL0 &= ~UC7BIT;                                                        // Character Length - 8-bit data selected
    UCA2CTL0 &= ~UCSPB;                                                         // Stop Bit Select - One stop bit selected
    UCA2CTL0 &= ~UCMODE1;                                                       // USCI Mode [2 Bits Required] - UART mode selected
    UCA2CTL0 &= ~UCMODE0;                                                       // USCI Mode [2 Bits Required] - UART mode selected
    UCA2CTL0 &= ~UCSYNC;                                                        // Synchronous Mode Enable - Asynchronous mode selected
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



    /*************************************************************************/
    /******************************** USCI A3 ********************************/
    /*************************************************************************/

void usci_A3_init(void){

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
