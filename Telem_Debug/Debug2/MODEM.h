#ifndef MODEM_H
#define MODEM_H

//public declarations constants

static char RFCommand[5] = "+++\r\0";
// 2008 Initialization using 9600 baud
//  Sunseeker telemetry PCB initialization
//	ATAM, MY 786, DT 786 <Enter>
//	ATRR 3, RN 4<Enter>
//	ATPK (message length),RB 474,RO 1B4<Enter>
//	ATPL 2<Enter>
//  ATBD 3<Enter>
//	ATCN<Enter> 	
//static char RFModem[74] = "ATAM,MY 786,DT 786\rATRR 3,RN 4\rATPK 474,RB 474,RO 1B4\rATPL 2\rATBD 3\rATCN\r";
// 2010 Initialization
static char RFModemH[76] = "ATAM,MY 786,DT 786\rATRR 3,RN 4\rATPK 201,RB 12F,RO 128\rATPL 2\rATBD 3\rATCN\r\0\0";
static char RFModemL[76] = "ATAM,MY 786,DT 786\rATRR 3,RN 4\rATPK 201,RB 12F,RO 128\rATPL 2\rATBD 3\rATCN\r\0\0";
static char RFModemS[76] = "ATAM,MY 786,DT 786\rATRR 3,RN 4\rATPK 201,RB 1A7,RO 128\rATPL 2\rATBD 3\rATCN\r\0\0";
//Baud rate change command
static char RFModemBaud[14] = "ATBD 7\rATCN\r\0\0";

extern void MODEM_init();
extern void MODEM_command (char data[]);
extern int  MODEM_high_baud();
extern int MODEM_TX(char data[], int size);
extern int MODEM_TX_PCK(char data[]);

#endif
