/*
 * Clock Initialization Code MSP430F5438A
 * Initialize Clock Source XT1 @ 32768 Hz
 * Initialize Clock Source XT2 @ 20MHz
 *
 * Sunseeker Telemetry 2021
 *
 * Last modified October 2015 by Scott Haver
 * Last modified May 2021 by B. Bazuin
 *
 * Main CLK     :  MCLK  = XT2     = 20 MHz
 * Sub-Main CLK :  SMCLK = XT2/2   = 10  MHz
 * Aux CLK      :  ACLK  = XT1     = 32.768 kHz
 *
 */

#ifndef CLOCK_INIT_H_
#define CLOCK_INIT_H_

#include "SunseekerTelemetry2021.h"

void Port_Init(void);
void Clock_XT1_Init(void);
void Clock_XT2_Init(void);
void SetVCoreUp(unsigned int level);

#endif /* CLOCK_INIT_H_ */
