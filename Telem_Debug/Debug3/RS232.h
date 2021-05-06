#ifndef RS232_PORTS_H_
#define RS232_PORTS_H_

//public declarations constants

static char RS232Cmd[5] = "+++\r\0";
static char RS232_Test1[13] = "Sunseeker \n\r\0";
static char RS232_Test2[9] = "2014. \n\r\0";
static char Parse_header[6][5] = {"LTC \0","ADC \0","ISH \0","ERR \0","BPS \0","BPC \0"};

/*********************************************************************************/
// BPS to PC External RS-232 (voltage isolated)
/*********************************************************************************/

void UART_init();
void UART_putchar(char data);
unsigned char UART_getchar(void);

int UART_gets(char *ptr);
int UART_puts(char *str);

void UART_put_int(void);


#endif /*RS232_PORTS_H_*/
