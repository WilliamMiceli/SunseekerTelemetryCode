#include "timers.h"

void timerB_init(void)
{
    TBCTL = CNTL_0 | TBSSEL_1 | ID_3 | TBCLR;     // ACLK/8, clear TBR
    TBCCR0 = (ACLK_RATE/8/TICK_RATE);             // Set timer to count to this value = TICK_RATE overflow
    TBCCTL0 = CCIE;                               // Enable CCR0 interrupt
    TBCTL |= MC_1;                                // Set timer to 'up' count mode
}
