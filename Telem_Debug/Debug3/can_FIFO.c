//
// Telemetry Messgae FIFO
//
#include "Sunseeker2021.h"

void can_fifo_INIT(void)
{
  can_struct *temptestPt;
  
  temptestPt = &can0_queue.msg_fifo[0];
  can0_queue.PutPt = temptestPt;
  can0_queue.GetPt = temptestPt;
} 

int can_fifo_PUT(can_message_fifo *queue, can_struct toPut)
{
  can_struct *tempPt;
  
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

int can_fifo_GET(can_message_fifo *queue, can_struct *toGet)
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

int can_fifo_STAT(can_message_fifo *queue)
{
  return (queue->GetPt != queue->PutPt);
}

