/*
 *  LTCspi.c
 *  
 *  Created by Byron Izenbaard on 6/8/11.
 *  Modified by Kenwood Hoben on 5/26/12
 *  
 *  Modified for BPS_V2 by Scott Haver on 11/03/15
 *
 *  Copyright 2015 Western Michigan University Sunseeker. All rights reserved.
 *
 */
#include<msp430x54xa.h>
 
void LTC1SPI_init(void) 
{
	UCA2CTL1 |= UCSWRST;	//software reset
	UCA2CTL0 = UCSYNC | UCMODE_0 | UCMST | UCMSB | UCCKPL; //data-capt then change; MSB first; Master; 3-pin SPI
	UCA2CTL1 |= UCSSEL_2;	// SMCLK - 8 MHz
	UCA2BR1 = 0x00;
	UCA2BR0 = 0x08;			// BR = 1 MHz (max for LTC)
	UCA2STAT &= ~UCLISTEN;	// not in loopback mode
	UCA2CTL1 &= ~UCSWRST;	//clear software reset
}

void LTC1_transmit(unsigned char data)
{
  UCA2TXBUF = data;
  while((UCA2STAT & UCBUSY) != 0);	// Wait for Rx completion (implies Tx is also complete)
  UCA2RXBUF;
}

unsigned char LTC1_exchange(unsigned char data)
{
  UCA2TXBUF = data;
  while((UCA2STAT & UCBUSY) != 0);	// Wait for Rx completion (implies Tx is also complete)
  return(UCA2RXBUF);
}

void LTC2SPI_init(void) 
{
	UCB2CTL1 |= UCSWRST;	//software reset
	UCB2CTL0 = UCSYNC | UCMODE_0 | UCMST | UCMSB | UCCKPL; //data-capt then change; MSB first; Master; 3-pin SPI
	UCB2CTL1 |= UCSSEL_2;	// SMCLK - 8 MHz
	UCB2BR1 = 0x00;
	UCB2BR0 = 0x08;			// BR = 1 MHz (max for LTC)
	UCB2STAT &= ~UCLISTEN;	// not in loopback mode
	UCB2CTL1 &= ~UCSWRST;	//clear software reset
}

void LTC2_transmit(unsigned char data)
{
  UCB2TXBUF = data;
  while((UCB2STAT & UCBUSY) != 0);	// Wait for Rx completion (implies Tx is also complete)
  UCB2RXBUF;
}

unsigned char LTC2_exchange(unsigned char data)
{
  UCB2TXBUF = data;
  while((UCB2STAT & UCBUSY) != 0);	// Wait for Rx completion (implies Tx is also complete)
  return(UCB2RXBUF);
}

void LTC3SPI_init(void) 
{
	UCB3CTL1 |= UCSWRST;	//software reset
	UCB3CTL0 = UCSYNC | UCMODE_0 | UCMST | UCMSB | UCCKPL; //data-capt then change; MSB first; Master; 3-pin SPI
	UCB3CTL1 |= UCSSEL_2;	// SMCLK - 8 MHz
	UCB3BR1 = 0x00;
	UCB3BR0 = 0x08;			// BR = 1 MHz (max for LTC)
	UCB3STAT &= ~UCLISTEN;	// not in loopback mode
	UCB3CTL1 &= ~UCSWRST;	//clear software reset
}

void LTC3_transmit(unsigned char data)
{
  UCB3TXBUF = data;
  while((UCB3STAT & UCBUSY) != 0);	// Wait for Rx completion (implies Tx is also complete)
  UCB3RXBUF;
}

unsigned char LTC3_exchange(unsigned char data)
{
  UCB3TXBUF = data;
  while((UCB3STAT & UCBUSY) != 0);	// Wait for Rx completion (implies Tx is also complete)
  return(UCB3RXBUF);
}
