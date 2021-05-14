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
#define P10                 0x01
#define P11                 0x02
#define P12                 0x04
#define P13                 0x08
#define P14                 0x10
#define P15                 0x20
#define RTC_MFP             0x40
#define IMU_INTn            0x80
#define P1_UNUSED           0x01 | 0x02 | 0x04 | 0x08 | 0x10 | 0x20

// PORT 2
#define CAN0_INTn           0x01
#define CAN0_RXB0n          0x02
#define CAN0_RXB1n          0x04
#define CAN1_INTn           0x08
#define CAN1_RXB0n          0x10
#define CAN1_RXB1n          0x20
#define GPS_INTn            0x40
#define P27                 0x80
#define P2_UNUSED           0x80

// PORT 3
#define CAN0_SCLK           0x01
#define SDC_SIMO            0x02
#define SDC_SOMI            0x04
#define SDC_SCLK            0x08
#define CAN0_MOSI           0x10
#define CAN0_MISO           0x20
#define CAN1_CLK            0x40
#define IMU_SDA             0x80
#define P3_UNUSED           0x00

// PORT 4
#define CAN0_RSTn           0x01
#define CAN0_CSn            0x02
#define CAN1_RSTn           0x04
#define CAN1_CSn            0x08
#define SDC_WPn             0x10
#define SDC_CDn             0x20
#define P46                 0x40
#define P47                 0x80
#define P4_UNUSED           0x40 | 0x80

// PORT 5
#define P50                 0x01
#define P51                 0x02
#define XT2IN               0x04
#define XT2OUT              0x08
#define IMU_SCL             0x10
#define P55                 0x20
#define CAN1_MOSI           0x40
#define CAN1_MISO           0x80
#define P5_UNUSED           0x01 | 0x02 | 0x20

// PORT 6
#define P60                 0x01
#define P61                 0x02
#define P62                 0x04
#define P63                 0x08
#define P64                 0x10
#define P65                 0x20
#define P66                 0x40
#define P67                 0x80
#define P6_UNUSED           0x01 | 0x02 | 0x04 | 0x08 | 0x10 | 0x20 | 0x40 | 0x80

// PORT 7
#define XT1IN               0x01
#define XT1OUT              0x02
#define P72                 0x04
#define P73                 0x08
#define P74                 0x10
#define P75                 0x20
#define P76                 0x40
#define P77                 0x80
#define P7_UNUSED           0x04 | 0x08 | 0x10 | 0x20 | 0x40 | 0x80

// PORT 8
#define P80                 0x01
#define P81                 0x02
#define P82                 0x04
#define LEDG                0x08
#define LEDR                0x10
#define LEDY0               0x20
#define LEDY1               0x40
#define Button0             0x80
#define P8_UNUSED           0x01 | 0x02 | 0x04

// PORT 9
#define P90                 0x01
#define RTC_SDA             0x02
#define RTC_SCL             0x04
#define P93                 0x08
#define USB_TX              0x10
#define USB_RX              0x20
#define SDC_CSn             0x40
#define GPS_CSn             0x80
#define P9_UNUSED           0x40 | 0x80

// PORT 10
#define BT_CSn              0x01
#define BT_MOSI             0x02
#define BT_MISO             0x04
#define BT_SCLK             0x08
#define UART_TX             0x10
#define UART_RX             0x20
#define BT_EN               0x40
#define P107                0x80
#define P10_UNUSED          0x80

// PORT 11
#define ACLK_TEST           0x01
#define MCLK_TEST           0x02
#define SMCLK_TEST          0x04

// PORT J
#define JTAG_TDO            0x01
#define JTAG_TDI            0x02
#define JTAG_TMS            0x04
#define JTAG_TCK            0x08

#endif /* MAIN_H_ */
