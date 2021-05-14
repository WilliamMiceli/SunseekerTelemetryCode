/*
 * Battery Protection Software for BPS PCB Development
 * Originally Written for BPS_V1 2012
 *
 * Modified for BPS_V2 2015 by Scott Haver
 * WMU Sunseeker 2015
 *
 *  I/O Initialization
 */

// Include files
#include "SunseekerTelemetry2021.h"

/*
 * Initialise I/O port directions and states
 * Drive unused pins as outputs to avoid floating inputs
 *
 */
void io_init( void )
{
	/******************************PORT 1**************************************/
	P1OUT = 0x00;		   				                    // Pull pins low
  	P1DIR = P1_UNUSED;	//set to output
  	P1DIR &= ~(RTC_MFP | IMU_INTn); //set to input
	/*Interrupts Enable*/
	//    P1SEL = RTC_MFP | IMU_INTn;
    //    P1IE  = RTC_MFP | IMU_INTn; // Enable Interrupts
    P1IES = IMU_INTn; 		//high to low
    P1IFG = 0x00;   										//Clears all interrupt flags on Port 1
    delay();

	/******************************PORT 2**************************************/
	P2OUT = 0x00;			// Pull pins low
 	P2DIR = P2_UNUSED;		//set to output
 		/*Interrupts Enable */
    //    P2SEL = CAN0_INTn | CAN1_INTn | GPS_INTn; //Interrupts Select
	//    P2IES = CAN0_INTn | CAN1_INTn | GPS_INTn;
	//    P2IE  = CAN0_INTn | CAN1_INTn | GPS_INTn; // Enable Interrupts
	//    P2SEL |= CAN0_RXB0n | CAN0_RXB1n | CAN1_RXB0n | CAN1_RXB1n; //Interrupts Select
	//    P2IES |= CAN0_RXB0n | CAN0_RXB1n | CAN1_RXB0n | CAN1_RXB1n;
	//	  P2IE  |= CAN0_RXB0n | CAN0_RXB1n | CAN1_RXB0n | CAN1_RXB1n; // Enable Interrupts
    P2IFG = 0x00;       				//Clears all interrupt flags on Port 2
    delay();

    /******************************PORT 3**************************************/  
	P3OUT = 0x00; 						// Pull pins low       	
   	P3DIR =  CAN0_SCLK | CAN0_MOSI | SDC_SCLK | SDC_SIMO | CAN1_SCLK | P3_UNUSED;
   	P3OUT |= CAN0_SCLK | CAN0_MOSI | SDC_SCLK | SDC_SIMO | CAN1_SCLK;
   	P3DIR &= ~(CAN0_MISO | SDC_SOMI);
    P3SEL = CAN0_SCLK | CAN0_MOSI | CAN0_MISO | SDC_SCLK | SDC_SIMO | SDC_SOMI | CAN1_SCLK | IMU_SDA;
    
 	/******************************PORT 4**************************************/ 
	P4OUT = 0x00;						// Pull pins low      	
    P4DIR = CAN0_RSTn | CAN0_CSn | CAN1_RSTn | CAN1_CSn | P4_UNUSED;
	P4OUT = CAN0_RSTn | CAN0_CSn | CAN1_RSTn | CAN1_CSn;
    delay();
	P4OUT &= ~(CAN0_RSTn | CAN1_RSTn) ;
    delay();
    delay();
	P4OUT |= (CAN0_RSTn | CAN1_RSTn) ;
	
    /******************************PORT 5**************************************/    
	P5OUT = 0x00;						// Pull pins low       	
    P5DIR = XT2OUT | CAN1_MOSI | P5_UNUSED;
	P5OUT = CAN1_MOSI;
	P5DIR &= ~(CAN1_MISO);
	P5SEL = XT2IN | XT2OUT | IMU_SCL | CAN1_MOSI | CAN1_MISO;
    
    /******************************PORT 6**************************************/   
	P6OUT = 0x00;						// Pull pins low   
	P6DIR = P6_UNUSED;
    P6SEL = 0x00;
    
    /******************************PORT 7**************************************/ 
	P7OUT = 0x00;						// Pull pins low  
	P7DIR = XT1OUT | P7_UNUSED;
    P7SEL = XT1IN | XT1OUT;
    
	/******************************PORT 8**************************************/ 
	P8OUT = 0x00;						// Pull pins low   
	P8DIR = LEDG | LEDR | LEDY0 | LEDY1 | P8_UNUSED;
	P8DIR &= ~(Button0);
	P8SEL = 0x00;
	
    /******************************PORT 9**************************************/    
	P9OUT = 0x00;						// Pull pins low  
	P9DIR = USB_TX | SDC_CSn | GPS_CSn | P9_UNUSED;
	P9OUT = SDC_CSn | GPS_CSn;
	P9SEL = RTC_SDA | RTC_SCL | USB_TX | USB_RX;
	
    /******************************PORT 10**************************************/ 
	P10OUT = 0x00;						// Pull pins low   
	P10DIR =  BT_CSn | BT_MOSI | BT_SCLK | UART_TX | BT_EN |P10_UNUSED;
	P10OUT =  BT_CSn | BT_MOSI | BT_SCLK;
	P10SEL =  BT_MOSI | BT_MISO | BT_SCLK | UART_TX | UART_RX;
	
	
 	/******************************PORT 11**************************************/ 
 	P11OUT = 0x00;						// Pull pins low
 	P11DIR = ACLK_TEST | MCLK_TEST | SMCLK_TEST; 	
 	P11OUT = ACLK_TEST | MCLK_TEST | SMCLK_TEST;
 	P11SEL = ACLK_TEST | MCLK_TEST | SMCLK_TEST;

    /******************************PORT J**************************************/ 
	PJOUT = 0x00;
	PJDIR = 0x0F;     					//set to output as per user's guide
        
}


