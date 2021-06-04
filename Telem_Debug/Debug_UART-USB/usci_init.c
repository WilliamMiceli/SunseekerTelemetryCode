#include "usci_init.h"

void usci_init(void){
    /******************************** USCI A0 ********************************/

    /******************************** USCI A1 ********************************/

    /******************************** USCI A2 ********************************/

    /*
     * UART Mode
     *
     * The FTDI FT230XS-R that was selected can handle up to 3 Mbaud, so we'll target 1 Mbaud
     * The SMCLK is currently at 10 MHz
     *
     * Using the "MSP430 USCI/EUSCI UART Baud Rate Calculation" tool at:
     * http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html
     * The following values were found:
     *      clock Prescalar: 10
     *      firstModReg: 0
     *      secondModReg: 0
     *      overSampling: 0
     */
//    USCI_A_UART_initParam USCI_A_UART_initParam_A2 = {
//        .selectClockSource = USCI_A_UART_CLOCKSOURCE_SMCLK,
//        .clockPrescalar = 10,
//        .firstModReg = 0,
//        .secondModReg = 0,
//        .parity = USCI_A_UART_NO_PARITY,
//        .msborLsbFirst = USCI_A_UART_LSB_FIRST,
//        .numberofStopBits = USCI_A_UART_ONE_STOP_BIT,
//        .uartMode = USCI_A_UART_MODE,
//        .overSampling = USCI_A_UART_LOW_FREQUENCY_BAUDRATE_GENERATION
//    };

    //USCI_A_UART_init(USCI_A2_BASE, &USCI_A_UART_initParam_A2);                                                  // Initialize the UART block
    //USCI_A_UART_enable(USCI_A2_BASE);                                                                           // Enables operation of the UART block

    // Initialization
    UCA2CTL1 |= UCSWRST;                                                        // Sets the Software Reset to high, disabling the USCI module; Register options can only be set when the UCSWRST bit = 1
    UCA2CTL1 |= UCSSEL__SMCLK;                                                  // USCI module will use the SMCLK as it's clock source
    UCA2CTL1 &= UCRXEIE;                                                        // Disabling Erroneous Character Reception Interrupt
    UCA2CTL1 &= UCBRKIE;                                                        // Disabling Break Character Reception Interrupt
    UCA2CTL1 &= UCDORM;                                                         // Disabling USCI module Dormant Mode
    UCA2CTL1 &= UCTXADDR;                                                       // Next frame transmitted is an address
    UCA2CTL1 &= UCTXBRK;                                                        // Do not transmit a Break/Synch as the next frame

    /******************************** USCI A3 ********************************/

    /******************************** USCI B0 ********************************/

    /******************************** USCI B1 ********************************/

    /******************************** USCI B2 ********************************/

    /******************************** USCI B3 ********************************/

}
