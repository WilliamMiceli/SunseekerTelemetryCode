#ifndef message_FIFO_H
#define message_FIFO_H

#ifndef msg_fifo_size
#define msg_fifo_size 16
#endif


//structure to hold an individual message for a can frame
typedef struct _can_message
{
  unsigned int status;
  unsigned int address;
  char data[8];
} can_message;

//structure to hold incoming can messages before decoding
typedef struct _message_fifo
{
  can_message msg_fifo[msg_fifo_size];
  can_message *PutPt;
  can_message *GetPt;
} message_fifo;


//public structure
extern message_fifo decode_queue;

//public functions
extern void message_fifo_INIT(void);
extern int message_fifo_PUT(message_fifo *queue, can_message toPut);
extern int message_fifo_GET(message_fifo *queue, can_message *toGet);
extern int message_fifo_STAT(message_fifo *queue);

#endif

