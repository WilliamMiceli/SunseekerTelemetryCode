/*
* Tritium MSP430 SPI interface header file
* Copyright (c) 2006, Tritium Pty Ltd.  All rights reserved.
*
* Redistribution and use in source and binary forms, with or without modification, 
* are permitted provided that the following conditions are met:
*  - Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
*	- Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer 
*	  in the documentation and/or other materials provided with the distribution.
*	- Neither the name of Tritium Pty Ltd nor the names of its contributors may be used to endorse or promote products 
*	  derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
* IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
* OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
* OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
* OF SUCH DAMAGE. 
*
* Last Modified: J.Kennedy, Tritium Pty Ltd, 18 December 2006
*
*/

#ifndef SPI_H
#define SPI_H

#define SPI1CTL1  (*(volatile unsigned char *)(0x05E0+0x00))
#define SPI1CTL0  (*(volatile unsigned char *)(0x05E0+0x01))
#define SPI1BR0   (*(volatile unsigned char *)(0x05E0+0x06))
#define SPI1BR1   (*(volatile unsigned char *)(0x05E0+0x07))
#define SPI1STAT  (*(volatile unsigned char *)(0x05E0+0x0A))
#define SPI1RXBUF (*(volatile unsigned char *)(0x05E0+0x0C))
#define SPI1TXBUF (*(volatile unsigned char *)(0x05E0+0x0E))
#define SPI1IE    (*(volatile unsigned char *)(0x05E0+0x1C))
#define SPI1IFG   (*(volatile unsigned char *)(0x05E0+0x1D))
#define SPI1IV    (*(volatile unsigned int *)(0x05E0+0x1E))

#define SPI2CTL1  (*(volatile unsigned char *)(0x0620+0x00))
#define SPI2CTL0  (*(volatile unsigned char *)(0x0620+0x01))
#define SPI2BR0   (*(volatile unsigned char *)(0x0620+0x06))
#define SPI2BR1   (*(volatile unsigned char *)(0x0620+0x07))
#define SPI2STAT  (*(volatile unsigned char *)(0x0620+0x0A))
#define SPI2RXBUF (*(volatile unsigned char *)(0x0620+0x0C))
#define SPI2TXBUF (*(volatile unsigned char *)(0x0620+0x0E))
#define SPI2IE    (*(volatile unsigned char *)(0x0620+0x1C))
#define SPI2IFG   (*(volatile unsigned char *)(0x0620+0x1D))
#define SPI2IV    (*(volatile unsigned int *)(0x0620+0x1E))

// Public Function prototypes
static void spi1_init()
{
	SPI1CTL1 |= UCSWRST;					//software reset
	SPI1CTL0 |= UCCKPH | UCMSB | UCMST | UCMODE_0 | UCSYNC; //data-capt then change; MSB first; Master; 3-pin SPI; sync
	SPI1CTL1 |= UCSSEL_2;				//set SMCLK
	SPI1BR0 = 0x01;						//set clk prescaler to 2
	SPI1BR1 = 0x00;
	SPI1STAT = 0x00;					//not in loopback mode
	SPI1CTL1 &= ~UCSWRST;					//SPI enable turn off software reset
}

static void spi2_init() {}	//FIXME:EMPTY

/*
* Transmits data on SPI connection
*	- Busy waits until entire shift is complete
*	- On devices with hardware SPI support, this function is identical to spi_exchange,
*	  with the execption of not returning a value
*	- On devices with software (bit-bashed) SPI support, this function can run faster
*	  because it does not require data reception code
*/
static inline void spi1_transmit(unsigned char data)
{
  SPI1TXBUF = data;
  while((SPI1IFG & UCRXIFG) == 0x00);	// Wait for Rx completion (implies Tx is also complete)
  SPI1RXBUF;
}

/*
* Exchanges data on SPI connection
*	- Busy waits until entire shift is complete
*	- This function is safe to use to control hardware lines that rely on shifting being finalised
*/
static inline unsigned char spi1_exchange(unsigned char data)
{
  SPI1TXBUF = data;
  while((SPI1IFG & UCRXIFG) == 0x00);	// Wait for Rx completion (implies Tx is also complete)
  return(SPI1RXBUF);
}

#define spi1_transmit2(data) SPI1TXBUF = data; forceread = SPI1RXBUF
#define spi1_wait() while((SPI1IFG & UCRXIFG) == 0x00)
static inline unsigned char spi1_exchange2(unsigned char data) {
	SPI1TXBUF = data;
	spi1_wait();
	return SPI1RXBUF;
}

#endif
