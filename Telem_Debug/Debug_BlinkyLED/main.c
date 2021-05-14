/*
 * main.c
 */

#include "SunseekerTelemetry2021.h"



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
