#ifndef char_FIFO_H
#define char_FIFO_H

#ifndef fifo_size
#define fifo_size 16
#endif

typedef struct _char_fifo
{
  unsigned char fifo[fifo_size];
  unsigned char *PutPt;
  unsigned char *GetPt;
} char_fifo;

extern void CHAR_FIFO_INIT(char_fifo queue);
extern int CHAR_FIFO_PUT(char_fifo *queue, char data);
extern int CHAR_FIFO_GET(char_fifo *queue,unsigned char volatile *data);
extern int CHAR_FIFO_STAT(char_fifo *queue);



#endif

