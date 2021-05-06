#ifndef UART_H
#define UART_H

#define UART1CTL1  (*(volatile unsigned char *)(0x0600+0x00))
#define UART1CTL0  (*(volatile unsigned char *)(0x0600+0x01))
#define UART1BRW   (*(volatile unsigned int *)(0x0600+0x06))
#define UART1MCTL  (*(volatile unsigned char *)(0x0600+0x08))
#define UART1STAT  (*(volatile unsigned char *)(0x0600+0x0A))
#define UART1RXBUF (*(volatile unsigned char *)(0x0600+0x0C))
#define UART1TXBUF (*(volatile unsigned char *)(0x0600+0x0E))
#define UART1ABCTL (*(volatile unsigned char *)(0x0600+0x10))
#define UART1IE    (*(volatile unsigned char *)(0x0600+0x1C))
#define UART1IFG   (*(volatile unsigned char *)(0x0600+0x1D))
#define UART1IV    (*(volatile unsigned int *)(0x0600+0x1E))

#define UART2CTL1  (*(volatile unsigned char *)(0x0640+0x00))
#define UART2CTL0  (*(volatile unsigned char *)(0x0640+0x01))
#define UART2BRW   (*(volatile unsigned int *)(0x0640+0x06))
#define UART2MCTL  (*(volatile unsigned char *)(0x0640+0x08))
#define UART2STAT  (*(volatile unsigned char *)(0x0640+0x0A))
#define UART2RXBUF (*(volatile unsigned char *)(0x0640+0x0C))
#define UART2TXBUF (*(volatile unsigned char *)(0x0640+0x0E))
#define UART2ABCTL (*(volatile unsigned char *)(0x0640+0x10))
#define UART2IE    (*(volatile unsigned char *)(0x0640+0x1C))
#define UART2IFG   (*(volatile unsigned char *)(0x0640+0x1D))
#define UART2IV    (*(volatile unsigned int *)(0x0640+0x1E))

//USB interface
#define UART3CTL1  (*(volatile unsigned char *)(0x0680+0x00))
#define UART3CTL0  (*(volatile unsigned char *)(0x0680+0x01))
#define UART3BRW   (*(volatile unsigned int *)(0x0680+0x06))
#define UART3MCTL  (*(volatile unsigned char *)(0x0680+0x08))
#define UART3STAT  (*(volatile unsigned char *)(0x0680+0x0A))
#define UART3RXBUF (*(volatile unsigned char *)(0x0680+0x0C))
#define UART3TXBUF (*(volatile unsigned char *)(0x0680+0x0E))
#define UART3ABCTL (*(volatile unsigned char *)(0x0680+0x10))
#define UART3IE    (*(volatile unsigned char *)(0x0680+0x1C))
#define UART3IFG   (*(volatile unsigned char *)(0x0680+0x1D))
#define UART3IV    (*(volatile unsigned int *)(0x0680+0x1E))

//UART1 init is in MODEM_init()!

//public functions
static void uart2_init() {
  //1. Set UCSWRST
  //2. Initialize all USCI registers with UCSWRST = 1 including UCAxCTL1
  //3. Configure Ports
  //4. Clear UCSWRST via software
  //5. Enable interrupts (optional) via UCRXIE a/o UCTXI

	UART2CTL1 |= UCSWRST | UCSSEL_2;	//put state machine in reset & SMCLK
	UART2CTL0 |= UCMODE_0;

	UART2BRW = (int)(SMCLK_RATE/UART_BR);	//1MHZ 115200
	UART2MCTL |= UCBRF_0 + (char)UART_UCBRS; //modulation UCBRSx=6 UCBRFx=0

	UART2IFG &= ~UCTXIFG;			//Clear Xmit and Rec interrupt flags
	UART2IFG &= ~UCRXIFG;
	UART2CTL1 &= ~UCSWRST;			//initalize state machine
	UART2ABCTL |= UCABDEN;

	//UART2IE |= UCRXIE|UCTXIE;		//enable TX & RX interrupts
}

static inline void tx1_poll ()
{
  int z;
    //this works!
    //while(UCA1IFG & UCTXIFG);
    //for (int z = 0; z < 50; z++);
    while((UART1IFG & UCTXIFG) == 0);
    for (z = 0; z < 1000; z++);
}

static inline void UART1_TXN(char data[], int size)
{
	int z;
	
	for (z = 0; z < size; z++) {	
		tx1_poll();
      		UART1TXBUF = data[z];
	}
}

static inline void UART1_TX(char data[])
{
	int z = 0;
  
	while(data[z] != '\0')
	{
		tx1_poll();
		UART1TXBUF = data[z++];
	}
}

static inline void UART1_RX(char data[])
{
	int z = 0;
//	int i;
  
	do
	{	
		while((UART1IFG & UCRXIFG) == 0);
//      	for (i = 0; i < 1000; i++);
		data[z++] = UART1RXBUF;
	} while(data[z-1] != '\0');
}

static inline void UART1_RXN(char data[], int size)
{
	int z = 0;
//	int i;
  
	do
	{	
		while((UART1IFG & UCRXIFG) == 0);
//		for (i = 0; i < 1000; i++);
		data[z++] = UART1RXBUF;
	} while(z < size);
}

#endif
