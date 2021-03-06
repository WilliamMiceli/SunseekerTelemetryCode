#include <msp430.h> 

/*
 * main.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <msp430x54xa.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <float.h>

#include "SunseekerTelemetry2021.h"

// General Variables
volatile unsigned char status_flag = FALSE;		//status flag set on timer B

// CAN Communication Variables
volatile unsigned char cancomm_flag = FALSE;	//used for CAN transmission timing
volatile unsigned char send_can = FALSE;	//used for CAN transmission timing
volatile unsigned char rcv_can = FALSE;	//used for CAN transmission timing
volatile unsigned char can_full = FALSE;	//used for CAN transmission status
volatile unsigned char ac_charge_mode = FALSE;	//used for CAN transmission
volatile unsigned char dc_charge_mode = FALSE;	//used for CAN transmission
volatile unsigned char charge_mode = 0x00;	//used for CAN transmission

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	_DINT();     		    					//disables interrupts

	clock_init();								//Configure HF and LF clocks
	timerB_init();								//init timer B


	P8DIR |= BIT3;                            // P1.0 output
    P8DIR |= BIT4;                            // P1.0 output
    P8DIR |= BIT5;                            // P1.0 output
    P8DIR |= BIT6;                            // P1.0 output
    P8OUT &= ~BIT3;                          // Toggle P1.0
//    P8OUT &= ~BIT4;                          // Toggle P1.0
    P8OUT &= ~BIT5;                          // Toggle P1.0
//    P8OUT &= ~BIT6;                          // Toggle P1.0

	_EINT(); 	//enable global interrupts

    while(1)
    {
    	if(status_flag){
    		status_flag = FALSE;
    		P8OUT ^= BIT3;                          // Toggle P1.0
    		P8OUT ^= BIT4;                          // Toggle P1.0
    		P8OUT ^= BIT5;                          // Toggle P1.0
    		P8OUT ^= BIT6;                          // Toggle P1.0
    	}

      // __delay_cycles(100000);                  // Delay
    }

	return 0;
}

/*
* Initialize Timer B
*	- Provides timer tick timebase at 100 Hz
*/
void timerB_init( void )
{
  TBCTL = CNTL_0 | TBSSEL_1 | ID_3 | TBCLR;		// ACLK/8, clear TBR
  TBCCR0 = (ACLK_RATE/8/TICK_RATE);				// Set timer to count to this value = TICK_RATE overflow
  TBCCTL0 = CCIE;								// Enable CCR0 interrupt
  TBCTL |= MC_1;								// Set timer to 'up' count mode
}

/*
* Timer B CCR0 Interrupt Service Routine
*	- Interrupts on Timer B CCR0 match at 10Hz
*	- Sets Time_Flag variable
*/
/*
* GNU interrupt semantics
* interrupt(TIMERB0_VECTOR) timer_b0(void)
*/
#pragma vector = TIMERB0_VECTOR
__interrupt void timer_b0(void)
{
	static unsigned int status_count = TELEM_STATUS_COUNT;
	static unsigned int cancomm_count = CAN_COMMS_COUNT;

	// Primary System Heartbeat
	status_count--;
    if( status_count == 0 )
    {
    	status_count = TELEM_STATUS_COUNT;
    	status_flag = TRUE;
    }

	// Periodic CAN Status Transmission
    if(send_can) cancomm_count--;
    if( cancomm_count == 0 )
    {
        cancomm_count = CAN_COMMS_COUNT;
        cancomm_flag = TRUE;
    }
}

