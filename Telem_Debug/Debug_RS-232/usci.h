/*
 * USCI Functions
 */

#ifndef USCI_H_
#define USCI_H_

#include <msp430.h>

/*
 * Initializes all USCI modules using their own respective `usci_xx_init()` functions
 */
void usci_init(void);

void usci_A0_init(void);

void usci_A1_init(void);

/*
 * USCI A2 Initialization [UART Mode]
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
void usci_A2_init(void);

void usci_A2_enable(void);

void usci_A2_disable(void);

void usci_A2_enableInterrupt(void);

void usci_A2_disableInterrupt(void);

void usci_A2_transmitChar(char charToTransmit);

char usci_A2_receiveChar(void);

int usci_A2_transmitString(char* pString);

int usci_A2_receiveString(char* pString);

/*
 * USCI A3 Initialization [UART Mode]
 *
 * We will use a default baud rate of 9600 bps
 * The SMCLK is currently at 10 MHz
 *
 * Using the "MSP430 USCI/EUSCI UART Baud Rate Calculation" tool at:
 * http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html
 * The following values were found:
 *      clock Prescalar: 65
 *      firstModReg: 2
 *      secondModReg: 0
 *      overSampling: 1
 */
void usci_A3_init(void);

void usci_A3_enable(void);

void usci_A3_disable(void);

void usci_A3_enableInterrupt(void);

void usci_A3_disableInterrupt(void);

void usci_A3_transmitChar(char charToTransmit);

char usci_A3_receiveChar(void);

int usci_A3_transmitString(char* pString);

int usci_A3_receiveString(char* pString);

void usci_B0_init(void);

void usci_B1_init(void);

void usci_B2_init(void);

void usci_B3_init(void);

#endif /* USCI_H_ */
