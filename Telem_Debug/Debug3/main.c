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

#include "SunseekerTelemetry2021.h"

// structures
can_message_fifo can0_queue;
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

char CAN0_INT_FLAG = FALSE;
char CAN1_INT_FLAG = FALSE;

int thrs, tmin, tsec;
char ucFLAG,usbENABLE;

volatile unsigned char status_flag = FALSE;
volatile unsigned char hs_comms_flag = FALSE;
volatile unsigned char ls_comms_flag = FALSE;
volatile unsigned char st_comms_flag = FALSE;

static char init_msg_data[21] = "0xHHHHHHHH,0xHHHHHHHH";
static char init_time_msg[17] = "TL_TIM,HH:MM:SS\r\n";

char time_test_msg[16] = "TL_TIM,HH:MM:SS\0";


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
	timerB_init();								//init timer B
	delay();

	io_init();
	delay();

	init_RTC();
	delay();

	getTime(&thrs,&tmin,&tsec);
	delay();

	UART_init();
	delay();

	insert_time(&time_test_msg[0]);
	UART_puts(time_test_msg);
	delay();


	can0spi_init();
	delay();

	can0_init();
	delay();

	can1spi_init();
	delay();

	can1_init();
	delay();

	can_fifo_INIT();






	_EINT(); 	//enable global interrupts

	ucMODE = INIT;

    while(1)
    {
    	if(status_flag){
    		status_flag = FALSE;
    		P8OUT ^= BIT3;                          // Toggle P1.0
    		P8OUT ^= BIT6;                          // Toggle P1.0
    	}

        CAN0_INT_FLAG = ((P2IN & CAN1_INTn)==0);
        if(CAN0_INT_FLAG){
//          ucFLAG |= 0x40;
          can_stall_cnt++;
          can0_receive();
        }


      // __delay_cycles(100000);                  // Delay
    }

	return 0;
}

/*
* Initialise Timer B
*	- Provides timer tick timebase at 100 Hz
*/
void timerB_init( void )
{
  TBCTL = CNTL_0 | TBSSEL_1 | ID_3 | TBCLR;		// ACLK/8, clear TBR
  TBCCR0 = (ACLK_RATE/8/TICK_RATE);				// Set timer to count to this value = TICK_RATE overflow
  TBCCTL0 = CCIE;								// Enable CCR0 interrrupt
  TBCTL |= MC_1;								// Set timer to 'up' count mode
}

/*
* Timer B CCR0 Interrupt Service Routine
*	- Interrupts on Timer B CCR0 match at 10Hz
*	- Sets Time_Flag variable
*/
/*
* GNU interropt symantics
* interrupt(TIMERB0_VECTOR) timer_b0(void)
*/
#pragma vector = TIMERB0_VECTOR
__interrupt void timer_b0(void)
{
	  static unsigned int status_count = TELEM_STATUS_COUNT;
	  static unsigned int hs_comms_count = HS_COMMS_SPEED;
	  static unsigned int ls_comms_count = LS_COMMS_SPEED;
	  static unsigned int st_comms_count = ST_COMMS_SPEED;

	  if (ucMODE != INIT)
	  {
	  // Trigger comms events (hs command packet transmission)
	  	hs_comms_count--;
	  	if( hs_comms_count == 0 ){
	   		hs_comms_count = HS_COMMS_SPEED;
	    	hs_comms_flag = TRUE;
	  	}

	  	ls_comms_count--;
	  	if( ls_comms_count == 0 ){
	    	ls_comms_count = LS_COMMS_SPEED;
	    	ls_comms_flag = TRUE;
	  	}

	  	st_comms_count--;
	  	if( st_comms_count == 0 ){
	    	st_comms_count = ST_COMMS_SPEED;
	    	st_comms_flag = TRUE;
	  	}
	  }

	  // Primary System Heart beat
	  status_count--;
	  if( status_count == 0 )
	  {
		  status_count = TELEM_STATUS_COUNT;
		  status_flag = TRUE;
	  }


}

