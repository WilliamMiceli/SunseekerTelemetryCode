/*
 *  Originally Written for TELEMETRY 2021 PCB Development
 * 
 *  UART to Modem UCA3 Interface
 *
 *  Clock and microcontroller Initialization RS232
 */

// Include files
#include<msp430x54xa.h>
#include "RS232.h"
#include "SunseekerTelemetry2021.h"

/*********************************************************************************/
// UART to Modem UCA3 Interface (voltage isolated)
/*********************************************************************************/
void UART_init(void)
{
    UCA3CTL1 |= UCSWRST | UCSSEL_2;	//put state machine in reset & SMCLK
    UCA3CTL0 |= UCMODE_0;
    UCA3BRW = (int)(SMCLK_RATE/MODEM_BR1);	//10MHZ/9600
    UCA3MCTL |= UCBRF_0 + (char)MODEM_UCBRS1; //modulation UCBRSx=computed from baud rate UCBRFx=0
	
	UCA3IFG &= ~UCTXIFG;			//Clear Xmit and Rec interrupt flags
	UCA3IFG &= ~UCRXIFG;
	UCA3CTL1 &= ~UCSWRST;			// initalize state machine
	UCA3ABCTL |= UCABDEN;			// automatic baud rate

//  UCA3IE |= UCRXIE|UCTXIE;		//enable TX & RX interrupt
}

void UART_putchar(char data)
{
    while((UCA3IFG & UCTXIFG) == 0);
    UCA3TXBUF = data; 
}

unsigned char UART_getchar(void)
{
	char i = 100;

    while((UCA3IFG & UCRXIFG) == 0 && i != 0)
    {
    	i--;
    }
    return(UCA3RXBUF);
}

int UART_puts(char *str)
{
	int i;
    char ch;
    i = 0;

    while((ch=*str)!= '\0')
    {
    	UART_putchar(ch);
    	str++;
    	i++;
    }
    UART_putchar(0x0A);
    UART_putchar(0x0D);
    
    return(i);
}

int UART_gets(char *ptr)
{
    int i;
    i = 0;
    while (1) {
          *ptr = UART_getchar();
          if (*ptr == 0x0D){
             *ptr = 0;
             return(i);
          }
          else 
          {
          	ptr++;
          	i++;
          }
     }
}

void UART_put_int(void)
{
    extern char *putPC_ptr;
    extern char put_status_PC;
    char ch;
    
    ch = *putPC_ptr++;

	if (ch == '\0')
	{
		UCA3IE &= ~UCTXIE;
		put_status_PC = FALSE;
	}
	else
	{
		UCA3TXBUF = ch;
		UCA3IE |= UCTXIE;
		put_status_PC = TRUE;
	}
}

