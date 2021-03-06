//
// Telemetry MODEM Code
//
#include "SunseekerTelemetry2021.h"

int synccount = 0;

//Initialize the MODEM (RS1) Interface
//
//
void MODEM_init()
{
	UART3CTL1 |= UCSWRST | UCSSEL_2;	//put state machine in reset & SMCLK
	UART3CTL0 |= UCMODE_0;

	UART3BRW = (int)(SMCLK_RATE/MODEM_BR1);	//10MHZ/115200
	UART3MCTL |= UCBRF_0 + (char)MODEM_UCBRS1; //modulation UCBRSx=computed from baud rate UCBRFx=0

	UART3IFG &= ~UCTXIFG;			//Clear Xmit and Rec interrupt flags
	UART3IFG &= ~UCRXIFG;
	UART3CTL1 &= ~UCSWRST;			//initalize state machine
	UART3ABCTL |= UCABDEN;

	//UART3IE |= UCRXIE|UCTXIE;		//enable TX & RX interrupt
}

//  The Initialization/Synchronization sequence for the modem
//  
void MODEM_command(char data[])
{
  char respRX[4];
  int ii;
  UART1_TXN(RFCommand,3);
//  UART1_RXN(respRX,3);  // Hangs if no powered MODEM present
  for (ii=0;ii<0x100;ii++) delay();
  //for(int i = 0;i<0xffff;i++);
  synccount++;
  UART1_TX(data);
  // Note: after the completion of the data send
  // an "OK" is sent of the command is executed
  // an "ERROR" is sent of there is an error
}

//  The MODEM transmission with sync
//  
int MODEM_TX(char data[], int size)
{
    MODEM_command(RFModemS);
    UART1_TXN(data,size); 
    return 1;
}

int MODEM_TX_PCK(char data[])
{
    UART1_TX(data); 
    return 1;
}
//
//#pragma vector=USCI_A1_VECTOR
//__interrupt void USCI_A1_ISR(void)
//{
//  switch(__even_in_range(UCA1IV,4))
//  {
//  case 0:break;                             // Vector 0 - no interrupt
//  case 2:                                   // Vector 2 - RXIFG
//   UCA1IFG &= ~(UCRXIFG);
//     break;
//  case 4:                                   // Vector 4 - TXIFG
//    
//   // UCA1TXBUF = send[txcount++];            //send char and inc txcount
//    UCA1IFG &= UCTXIFG;                 //reset TX flag
//   // if(txcount > 16)
//    {
////      txcount = 0;                  ///reset txcount
//    }
//    
//    break;
//  default: break;
//  }
//}
