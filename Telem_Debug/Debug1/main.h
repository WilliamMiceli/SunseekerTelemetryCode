/*
 * Sunseeker Telemetry 2021
 *
* Last modified May 2021 by B. Bazuin
 *
 * Main CLK     :  MCLK  = XT2     = 20 MHz
 * Sub-Main CLK :  SMCLK = XT2/2   = 10  MHz
 * Aux CLK      :  ACLK  = XT1     = 32.768 kHz
 *
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <msp430x54xa.h>

#include <stdio.h>

void clock_init(void);
void timerB_init(void);



// Event timing
#define SMCLK_RATE		10000000	// Hz
#define ACLK_RATE		32768	// Hz
#define TICK_RATE		100		// Hz
#define TELEM_STATUS_COUNT			13			// Number of ticks per event: ~0.133 sec
#define CAN_COMMS_COUNT			50			// Number of ticks per event: 0.5 sec

// Constant Definitions
#define	TRUE				1
#define FALSE				0

/******************** Pin Definitions *************************/

// PORT 1
#define LED0		      	0x01
#define LED1			    0x02
#define BUTTON1				0x04	// BPS Reset
#define BUTTON2 			0x08
#define CAN_RSTn	        0x10
#define CAN_CSn		        0x20
#define CAN_RX0n			0x40
#define CAN_RX1n		    0x80
#define P1_UNUSED			0x00

// PORT 2
#define CAN_INTn			0x01
#define ADC1_RDYn			0x02
#define ADC2_RDYn			0x04
#define ADC3_RDYn			0x08
#define ADC4_RDYn			0x10
#define ADC5_RDYn			0x20
#define ADC6_RDYn			0x40
#define ADC7_RDYn			0x80
#define P2_UNUSED			0x00

// PORT 3
#define ADC_bus1_CLK		0x01
#define CAN_SIMO			0x02
#define CAN_SOMI			0x04
#define CAN_SCLK			0x08
#define ADC_bus1_MOSI		0x10
#define ADC_bus1_MISO		0x20
#define ADC_misc_CLK		0x40
#define ADC_bus2_MOSI		0x80
#define P3_UNUSED			0x00

// PORT 4
#define ADC_CS1				0x01
#define ADC_CS2				0x02
#define ADC_CS3				0x04
#define ADC_CS4				0x08
#define ADC_CS5				0x10
#define ADC_CS6				0x20
#define ADC_CS7			    0x40
#define ADC_RSTn			0x80
#define P4_UNUSED			0x00

// PORT 5
#define P50					0x01
#define P51					0x02
#define XT2IN				0x04
#define XT2OUT				0x08
#define ADC_bus2_MISO		0x10
#define ADC_bus2_CLK		0x20
#define ADC_misc_MOSI	    0x40
#define ADC_misc_MISO		0x80
#define P5_UNUSED			0x01 | 0x02

// PORT 6
#define LED2				0x01
#define LED3				0x02
#define LED4				0x04
#define LED5				0x08
#define RELAY_BATT_OPEN		0x10
#define RELAY_ARRAY			0x20
#define RELAY_MCPC			0x40
#define RELAY_MC		    0x80
#define P6_UNUSED			0x00

// PORT 7
#define XT1IN				0x01
#define XT1OUT				0x02
#define P72					0x04
#define P73					0x08
#define RELAY_SCPC			0x10
#define RELAY_SC			0x20
#define EXT_RELAY_SCPC		0x40
#define EXT_RELAY_MCPC		0x80
#define P7_UNUSED			0x04 | 0x08

// PORT 8
#define P80				    0x01
#define P81			    	0x02
#define P82			        0x04
#define P83					0x08
#define P84					0x10
#define LT_CS3		    	0x20
#define LT_CS2		    	0x40
#define LT_CS1			    0x80
#define P8_UNUSED			0x01 | 0x02 | 0x04 | 0x08 | 0x10

// PORT 9
#define LT_SCLK_1			0x01
#define LT_MOSI_2			0x02
#define LT_MISO_2			0x04
#define LT_SCLK_2			0x08
#define LT_MOSI_1			0x10
#define LT_MISO_1			0x20
#define P96				    0x40
#define p97				    0x80
#define P9_UNUSED			0x40 | 0x80

// PORT 10
//#define P100				0x01
#define RELAY_BATT			0x01
#define LT_MOSI_3			0x02
#define LT_MISO_3			0x04
#define LT_SCLK_3			0x08
#define EXT_TX				0x10
#define EXT_RX				0x20
#define P106				0x40
#define P107     			0x80
//#define P10_UNUSED		0x01 | 0x40 | 0x80
#define P10_UNUSED			0x40 | 0x80

// PORT 11
#define ACLK_TEST			0x01
#define MCLK_TEST			0x02
#define SMCLK_TEST		   	0x04

// PORT J
#define JTAG_TDO			0x01
#define JTAG_TDI			0x02
#define JTAG_TMS			0x04
#define JTAG_TCK			0x08

#endif /* MAIN_H_ */
