#include "io_init.h"

void io_init(void)
{
    /****************************** PORT 1 **************************************/
    P1OUT = 0x00;                                                               // Set output register to all low. Pulling enabled pins low, only affects ports set as output, no effect on inputs
    P1DIR = P1_UNUSED;                                                          // Set all unused pins to output
    P1DIR &= ~(RTC_MFP | IMU_INTn);                                             // Set specific pins to input
        /*Interrupts Enable*/
//  P1SEL = RTC_MFP | IMU_INTn;                                                 // Peripheral module function selected for specific pins
//  P1IE  = RTC_MFP | IMU_INTn;                                                 // Enable Interrupts
    P1IES = IMU_INTn;                                                           // High to low
    P1IFG = 0x00;                                                               // Clear all interrupt flags on Port 1
    delay();

    /****************************** PORT 2 **************************************/
    P2OUT = 0x00;                                                               // Set output register to all low. Pulling enabled pins low, only affects ports set as output, no effect on inputs
    P2DIR = P2_UNUSED;                                                          // Set all unused pins to output
        /*Interrupts Enable */
//  P2SEL = CAN0_INTn | CAN1_INTn | GPS_INTn;                                   // Peripheral module function selected for specific pins
//  P2IES = CAN0_INTn | CAN1_INTn | GPS_INTn;
//  P2IE  = CAN0_INTn | CAN1_INTn | GPS_INTn;                                   // Enable Interrupts
//  P2SEL |= CAN0_RXB0n | CAN0_RXB1n | CAN1_RXB0n | CAN1_RXB1n;                 // Peripheral module function selected for specific pins
//  P2IES |= CAN0_RXB0n | CAN0_RXB1n | CAN1_RXB0n | CAN1_RXB1n;
//  P2IE  |= CAN0_RXB0n | CAN0_RXB1n | CAN1_RXB0n | CAN1_RXB1n;                 // Enable Interrupts
    P2IFG = 0x00;                                                               // Clear all interrupt flags on Port 2
    delay();

    /****************************** PORT 3 **************************************/
    P3OUT = 0x00;                                                               // Set output register to all low. Pulling enabled pins low, only affects ports set as output, no effect on inputs
    P3DIR =  CAN0_SCLK | CAN0_MOSI | SDC_SCLK | SDC_MOSI | CAN1_SCLK | P3_UNUSED;   // Set to output
    P3OUT |= CAN0_SCLK | CAN0_MOSI | SDC_SCLK | SDC_MOSI | CAN1_SCLK;           // Pull used output pins high
    P3DIR &= ~(CAN0_MISO | SDC_MISO);                                           // Set specific pins to input
    P3SEL = CAN0_SCLK | CAN0_MOSI | CAN0_MISO | SDC_SCLK | SDC_MOSI | SDC_MISO | CAN1_SCLK | IMU_SDA;

    /****************************** PORT 4 **************************************/
    P4OUT = 0x00;                                                               // Set output register to all low. Pulling enabled pins low, only affects ports set as output, no effect on inputs
    P4DIR = CAN0_RSTn | CAN0_CSn | CAN1_RSTn | CAN1_CSn | P4_UNUSED;            // Set specific pins to output
    P4OUT = CAN0_RSTn | CAN0_CSn | CAN1_RSTn | CAN1_CSn;                        // Pull used output pins high
    delay();
    P4OUT &= ~(CAN0_RSTn | CAN1_RSTn);                                          // Set specific pins of output register to low
    delay();
    delay();
    P4OUT |= (CAN0_RSTn | CAN1_RSTn);                                           // Set specific pins of output register to high

    /****************************** PORT 5 **************************************/
    P5OUT = 0x00;                                                               // Set output register to all low. Pulling enabled pins low, only affects ports set as output, no effect on inputs
    P5DIR = XT2OUT | CAN1_MOSI | P5_UNUSED;                                     // Set specific pins to output
    P5OUT = CAN1_MOSI;                                                          // Set specific pins of output register to high
    P5DIR &= ~(CAN1_MISO);                                                      // Set specific pins to input
    P5SEL = XT2IN | XT2OUT | IMU_SCL | CAN1_MOSI | CAN1_MISO;                   // Peripheral module function selected for specific pins

    /****************************** PORT 6 **************************************/
    P6OUT = 0x00;                                                               // Set output register to all low. Pulling enabled pins low, only affects ports set as output, no effect on inputs
    P6DIR = P6_UNUSED;                                                          // Set all unused pins to output
    P6SEL = 0x00;                                                               // I/O function selected for all pins

    /****************************** PORT 7 **************************************/
    P7OUT = 0x00;                                                               // Set output register to all low. Pulling enabled pins low, only affects ports set as output, no effect on inputs
    P7DIR = XT1OUT | P7_UNUSED;                                                 // Set specific pins to output
    P7SEL = XT1IN | XT1OUT;                                                     // Peripheral module function selected for specific pins

    /****************************** PORT 8 **************************************/
    P8OUT = 0x00;                                                               // Set output register to all low. Pulling enabled pins low, only affects ports set as output, no effect on inputs
    P8DIR = LEDG | LEDR | LEDY0 | LEDY1 | P8_UNUSED;                            // Set specific pins to output
    P8DIR &= ~(Button0);                                                        // Set to input
    P8SEL = 0x00;                                                               // I/O function selected for all pins

    /****************************** PORT 9 **************************************/
    P9OUT = 0x00;                                                               // Set output register to all low. Pulling enabled pins low, only affects ports set as output, no effect on inputs
    P9DIR = USB_TX | SDC_CSn | GPS_CSn | P9_UNUSED;                             // Set specific pins to output
    P9OUT = SDC_CSn | GPS_CSn;                                                  // Set specific pins of output register to high
    P9SEL = RTC_SDA | RTC_SCL | USB_TX | USB_RX;                                // Peripheral module function selected for specific pins

    /****************************** PORT 10 **************************************/
    P10OUT = 0x00;                                                              // Set output register to all low. Pulling enabled pins low, only affects ports set as output, no effect on inputs
    P10DIR =  BT_CSn | BT_MOSI | BT_SCLK | UART_TX | BT_EN |P10_UNUSED;         // Set specific pins to output
    P10OUT =  BT_CSn | BT_MOSI | BT_SCLK;                                       // Set specific pins of output register to high
    P10SEL =  BT_MOSI | BT_MISO | BT_SCLK | UART_TX | UART_RX;                  // Peripheral module function selected for specific pins

    /****************************** PORT 11 **************************************/
    P11OUT = 0x00;                                                              // Set output register to all low. Pulling enabled pins low, only affects ports set as output, no effect on inputs
    P11DIR = ACLK_TEST | MCLK_TEST | SMCLK_TEST;                                // Set specific pins to output
    P11OUT = ACLK_TEST | MCLK_TEST | SMCLK_TEST;                                // Set specific pins of output register to high
    P11SEL = ACLK_TEST | MCLK_TEST | SMCLK_TEST;                                // Peripheral module function selected for specific pins

    /****************************** PORT J **************************************/
    PJOUT = 0x00;                                                               // Set output register to all low. Pulling enabled pins low, only affects ports set as output, no effect on inputs
    PJDIR = 0x0F;                                                               // Set to output as per user's guide

}
