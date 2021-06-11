#include "interrupts.h"

extern volatile unsigned char status_flag;

/*
 * Timer B CCR0 Interrupt Service Routine
 *   - Interrupts on Timer B CCR0 match at 10Hz
 *   - Sets Time_Flag variable
 */

/*
 * GNU interrupt semantics
 * interrupt(TIMERB0_VECTOR) timer_b0(void)
 */

#pragma vector = TIMER0_B0_VECTOR
__interrupt void TIMER0_B0_ISR(void){
      static unsigned int status_count = TELEM_STATUS_COUNT;

      // Primary System Heart beat
      status_count--;
      if(status_count == 0){
          status_count = TELEM_STATUS_COUNT;
          status_flag = TRUE;
          P8OUT |= LEDG;                            // Turn on the green LED
      }else{
          P8OUT &= ~LEDG;                           // Turn off the green LED
      }
}
