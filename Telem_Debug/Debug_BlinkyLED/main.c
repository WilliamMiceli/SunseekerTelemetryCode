//
// Telemetry
//
// Modified by Erik in 2010-2011
//
/* Modifications for 2013 by B. Bazuin
 * - 2013v1
 * - reworked to operate continuously with CAN at 250 kbps
 * - no USB device code included
 * - no ADC code
 * - table values collected only the first time captured
 * - 2014V2 MODIFICATIONS
 * - Reordered and eliminated some messages
 * - HF rate 10 sec, LF rate 30 sec, ST rate 60 sec
 * - BP_PCDONE and BP_ISH added
 * - Precharge Controller Removed
 */
/* Modifications for 2016 by B. Bazuin
  * -New BPS and Array Controller
  *
 */

#include "Sunseeker2021.h"

// structures
//message_fifo decode_queue;
//char_fifo USB_FIFO, MODEM_FIFO;

//hf_packet pckHF;
//lf_packet pckLF;
//status_packet pckST;

unsigned volatile char forceread;

unsigned int can_mask0, can_mask1;

enum MODE {INIT, CANREAD, DECODE, MODEMTX, USBTX, LOWP, LOOP} ucMODE;
unsigned volatile char can_status_test, can_rcv_status_test;
unsigned long can_msg_count = 0, can_stall_cnt = 0;
unsigned long can_err_count = 0, can_read_cnt = 0;
int thrs, tmin, tsec;
char ucFLAG,usbENABLE;
char CAN1_INT_FLAG = FALSE;



// General Variables

// CAN Communication Variables
volatile unsigned char cancomm_flag = FALSE;	//used for CAN transmission timing
volatile unsigned char send_can = FALSE;	//used for CAN transmission timing
volatile unsigned char rcv_can = FALSE;	//used for CAN transmission timing
volatile unsigned char can_full = FALSE;	//used for CAN transmission status

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	_DINT();     		    					//disables interrupts

	clock_init();								//Configure HF and LF clocks
	delay();

	P8DIR

	_EINT(); 	//enable global interrupts

    while(1)
    {
    	P8OUT ^= BIT3;                          // Toggle P1.0
    	P8OUT ^= BIT6;                          // Toggle P1.0
    	delay();
    }
}
