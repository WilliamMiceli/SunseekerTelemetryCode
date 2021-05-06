//
// Telemetry Character FIFO
//
#include "Sunseeker2021.h"


void CHAR_FIFO_INIT(char_fifo queue)
{
  queue.GetPt=&queue.fifo[0];
  queue.PutPt=&queue.fifo[0];
} 

int CHAR_FIFO_PUT(char_fifo *queue, char data)
{
  unsigned char *tempPt;
  tempPt = queue->PutPt;
  *(tempPt++) = data;
  if(tempPt == &queue->fifo[fifo_size])
  {
    tempPt = &queue->fifo[0];	//need to wrap around
  }
  if(tempPt == queue->GetPt)
  {
    return(0);		//Failure FIFO Full
  }
  else
  {
    queue->PutPt = tempPt;
    return(1);		//successful
  }
}

int CHAR_FIFO_GET(char_fifo *queue, unsigned char volatile *data)
{
  if(queue->PutPt == queue->GetPt)
  {
    return(0);	//failure FIFO empty
  }
  else
  {
    *data = *(queue->GetPt++);
    if(queue->GetPt == &queue->fifo[fifo_size])
    {
      queue->GetPt = &queue->fifo[0];	//need to wrap around
    }
    return(1);
  }
}

int CHAR_FIFO_STAT(char_fifo *queue)
{
  return (queue->GetPt != queue->PutPt);
}

