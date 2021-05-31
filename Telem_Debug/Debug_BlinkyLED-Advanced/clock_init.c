#include "clock_init.h"

void clock_init(void)
{
    WDTCTL = WDTPW + WDTHOLD;   // Stop watchdog timer; `WDTPW` is the "WatchDog Timer PassWord", required for all `WDTCTL` operations

    Port_Init();                //ensure clock pins are configured

    SetVCoreUp(1);              //Configure MCU core voltage for HF clock
    SetVCoreUp(2);              //
    SetVCoreUp(3);              //

    Clock_XT1_Init();           //LF clock source init
    Clock_XT2_Init();           //HF clock source init

}

void Port_Init(void)
{
    //Clock Source TEST PINS ACLK/MCLK/SMCLK
    P11DIR |= ACLK_TEST | MCLK_TEST | SMCLK_TEST;       // Set P11.0:P11.2 as output ACLK/MCLK/SMCLK
    P11SEL |= ACLK_TEST | MCLK_TEST | SMCLK_TEST;       // Set P11.0:P11.2 as ACLK/MCLK/SMCLK function

    //XT1 ALTERNATE PIN CONFIG
    P7SEL |= XT1IN | XT1OUT;                            // Set P7.0 & P7.1 as XT1IN/XT1OUT peripheral
    P7DIR |= XT1IN | XT1OUT;

    //XT2 ALTERNATE PIN CONFIG
    P5SEL |= XT2IN | XT2OUT;                            // Set P5.2 & P5.3 as XT2IN/XT2OUT peripheral
    P5DIR |= XT2IN | XT2OUT;
}

void Clock_XT1_Init(void)
{
    char i;

    //XT1 CLOCK CONFIG
    UCSCTL6 &= ~(XT1OFF);                                           // Enable XT1
    UCSCTL6 &= ~(XT1DRIVE1 | XT1DRIVE0);                            // Lowest drive current LF 32KHz oscillator

    do{
        UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + XT1HFOFFG + DCOFFG);     // Clear XT2,XT1,DCO fault flags
        SFRIFG1 &= ~OFIFG;                                          // Clear fault flags
        for(i=255;i>0;i--){                                         // Delay for oscillator to stabilize
            _NOP();                                                 // "No Operation"
        }
    }
    while((SFRIFG1 & OFIFG) != 0);                                  // Test oscillator fault flag

    UCSCTL4 |= SELA__XT1CLK;                                        // Clock Source ACLK = XT1 = 32kHz
    UCSCTL5 |= DIVA_0;                                              // Divide ACLK/1 = 32kHz

}

void Clock_XT2_Init(void)
{
    char i;

    //XT2 CLOCK CONFIG
    UCSCTL6 &= ~(XT2OFF);                                           // Enable XT2
    UCSCTL6 |= XT2DRIVE_3;                                          // Drive current 16-24 MHz Clock
    UCSCTL6 &= ~XT2BYPASS;                                          // XT2 Sourced Externally from pin - 20MHz

    do{
        UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + XT1HFOFFG + DCOFFG);     // Clear XT2,XT1,DCO fault flags
        SFRIFG1 &= ~OFIFG;                                          // Clear fault flags
        for(i=255;i>0;i--){                                         // Delay for oscillator to stabilize
            _NOP();                                                 // "No Operation"
        }
     }
     while((SFRIFG1 & OFIFG) != 0);                                 // Test oscillator fault flag

     UCSCTL4 |= (SELS__XT2CLK | SELM__XT2CLK);                      // Clock Source SMCLK=MCLK = XT2 = 20MHz
     UCSCTL5 |= DIVM_0 | DIVS_1;                                    // MCLK:XT2/1 = 20MHz SMCLK:XT2/2 = 10MHz

}

/*************************************************************
/ Name: SetVCoreUp
/ IN: int Level
/ OUT:  void
/ DESC:  This function is used to set the voltage of the VCORE to
/        The level specified in input
/ Reference: Users Guide page 74
************************************************************/
void SetVCoreUp (unsigned int level)
{
    PMMCTL0_H = 0xA5;                                                   // Open PMM registers for write access
    SVSMHCTL = SVSHE + SVSHRVL0 * level + SVMHE + SVSMHRRL0 * level;    // Set SVS/SVM high side new level
    SVSMLCTL = SVSLE + SVMLE + SVSMLRRL0 * level;                       // Set SVM low side to new level
    while((PMMIFG & SVSMLDLYIFG) == 0);                                 // Wait till SVM is settled
    PMMIFG &= ~(SVMLVLRIFG + SVMLIFG);                                  // Clear already set flags
    PMMCTL0_L = PMMCOREV0 * level;                                      // Set VCore to new level
    if(PMMIFG & SVMLIFG){
        while((PMMIFG & SVMLVLRIFG) == 0);                              // Wait till new level reached
    }
    SVSMLCTL = SVSLE + SVSLRVL0 * level + SVMLE + SVSMLRRL0 * level;    // Set SVS/SVM low side to new level
    while((PMMIFG & SVSMLDLYIFG) == 0);                                 // Wait till SVM is settled
    PMMCTL0_H = 0x00;                                                   // Lock PMM registers for write access
}
