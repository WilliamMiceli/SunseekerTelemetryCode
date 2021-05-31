/*
 * USCI Initialization
 */

#ifndef USCI_INIT_H_
#define USCI_INIT_H_

#include <msp430x54xa.h>
#include "usci_a_uart.h"

bool USCI_A_UART_init(uint16_t baseAddress, USCI_A_UART_initParam *param);
void USCI_A_UART_enable(uint16_t baseAddress);

void usci_init(void);

#endif /* USCI_INIT_H_ */
