/*
 *  I/O Initialization
 */

/*
 * Initialize I/O port directions and states
 * Drive unused pins as outputs to avoid floating inputs and wasting power
 *
 */

#ifndef IO_INIT_H_
#define IO_INIT_H_

#include "SunseekerTelemetry2021.h"

void io_init(void);

#endif /* IO_INIT_H_ */
