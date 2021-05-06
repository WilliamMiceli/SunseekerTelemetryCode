//
// Telemetry Messgae FIFO
//
#include "Sunseeker2021.h"

void message_fifo_INIT(void)
{
  can_message *temptestPt;
  
  temptestPt = &decode_queue.msg_fifo[0];
  decode_queue.PutPt = temptestPt;
  decode_queue.GetPt = temptestPt;
} 

int message_fifo_PUT(message_fifo *queue, can_message toPut)
{
  can_message *tempPt;
  
  tempPt = queue->PutPt;
  *(tempPt++) = toPut;
  if(tempPt == &queue->msg_fifo[msg_fifo_size])
  {
    tempPt = &queue->msg_fifo[0];	//need to wrap around
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

int message_fifo_GET(message_fifo *queue, can_message *toGet)
{
  if(queue->PutPt == queue->GetPt)
  {
    return(0);	//failure FIFO empty
  }
  else
  {
    *toGet = *(queue->GetPt++);
    if(queue->GetPt == &queue->msg_fifo[msg_fifo_size])
    {
      queue->GetPt = &queue->msg_fifo[0];	//need to wrap around
    }
    return(1);
  }
}

int message_fifo_STAT(message_fifo *queue)
{
  return (queue->GetPt != queue->PutPt);
}

