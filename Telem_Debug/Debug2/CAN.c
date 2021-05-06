/*
* Tritium MCP2515 CAN Interface
* Copyright (c) 2006, Tritium Pty Ltd.  All rights reserved.
*
* Redistribution and use in source and binary forms, with or without modification, 
* are permitted provided that the following conditions are met:
*  - Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
*	- Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer 
*	  in the documentation and/or other materials provided with the distribution.
*	- Neither the name of Tritium Pty Ltd nor the names of its contributors may be used to endorse or promote products 
*	  derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
* IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
* OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
* OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
* OF SUCH DAMAGE. 
*
* Last Modified: J.Kennedy, Tritium Pty Ltd, 18 December 2006
*
* - Implements the following CAN interface functions
*	- can_init
*	- can_transmit
*	- can_receive
*
*/

// Include files
#include "Sunseeker2021.h"

#define can0_select() P4OUT &= ~CAN0_CSn
#define can0_deselect() P4OUT |= CAN0_CSn

// Public structures
can_message TX_can_message;
can_message RX_can_message;

// Public variables
unsigned char canctrl = 0;
unsigned char canctrlold = 0;
char canstat;
extern unsigned long can_msg_count;
extern unsigned long can_err_count;
extern unsigned long can_read_cnt;

  // Private variables
unsigned char buffer[16];

void can0_canctrl(unsigned char mask, unsigned char value) {
	can0_read(CANCTRL, &canctrlold, 1);
	canctrl = (canctrl & ~mask) | (value & mask);
	can0_write(CANCTRL, &canctrl, 1);
}

/**************************************************************************************************
* PUBLIC FUNCTIONS
*************************************************************************************************/

/*
* Initialises MCP2515 CAN controller
*	- Resets MCP2515 via SPI port (switches to config mode, clears errors)
*	- Changes CLKOUT to /4 rate (4 MHz)
*	- Sets up bit timing for 1 Mbit operation
*	- Sets up receive filters and masks
*		- Rx Filter 0 = Motor controller velocity
*		- Rx Filter 1 = Unused
*		- Rx Filter 2 = Driver controls packets (for remote frame requests)
*		- Rx Filter 3 = Unused
*		- Rx Filter 4 = Unused
*		- Rx Filter 5 = Unused
*		- Rx Mask 0   = Exact message must match (all 11 bits)
*		- Rx Mask 1   = Block address must match (upper 6 bits)
*	- Enables ERROR and RX interrupts on IRQ pin
*	- Switches to normal (operating) mode
*/
void can0_init()
{
  // Set up reset and clocking
  can0_reset();
  //can_mode(interface, CANCTRL, 0x03, 0x02); // CANCTRL register, modify lower 2 bits, CLK = /4
  canctrl = 0x87;

  
  // Set up bit timing & interrupts
  buffer[0] = 0x02;			    // CNF3 register: PHSEG2 = 3Tq, No wakeup, CLKOUT = CLK
  buffer[1] = 0xC9;			    // CNF2 register: set PHSEG2 in CNF3, Triple sample, PHSEG1= 2Tq, PROP = 2Tq
//  buffer[2] = 0x00;			    // CNF1 register: SJW = 1Tq, BRP = 0
  buffer[2] = 0x03;			    // CNF1 register: SJW = 1Tq, BRP = 0
  buffer[3] = 0x23;			    // CANINTE register: enable ERROR, RX0 & RX1 interrupts on IRQ pin
  buffer[4] = 0x00;			    // CANINTF register: clear all IRQ flags
  buffer[5] = 0x00;			    // EFLG register: clear all user-changable error flags
  can0_write(CNF3, &buffer[0], 6);// Write to registers
  
  // Set up external interrupt pins RX_BF and RX1BF
  buffer[0] = 0x0F;			    // BFPCTRL register: enabele RXxBF as interrupts
  buffer[1] = 0x00;			    // TXRTSCTRL register: request to send TX
  can0_write(BFPCTRL, &buffer[0], 2);// Write to registers

  // Set up receive filtering & masks
  // RXF0 - Buffer 0
  buffer[ 0] = (unsigned char)((DC_CAN_BASE + DC_SWITCH) >> 3);
  buffer[ 1] = (unsigned char)((DC_CAN_BASE + DC_SWITCH) << 5);
  buffer[ 2] = 0x00;
  buffer[ 3] = 0x00;
  // RXF1 - Buffer 0
  buffer[ 4] = 0x00;
  buffer[ 5] = 0x00;
  buffer[ 6] = 0x00;
  buffer[ 7] = 0x00;
  // RXF2 - Buffer 1
//  buffer[ 8] = (unsigned char)((DC_CAN_BASE + DC_SWITCH) >> 3);
//  buffer[ 9] = (unsigned char)((DC_CAN_BASE + DC_SWITCH) << 5);
  buffer[ 8] = (unsigned char)((MC_CAN_BASE1) >> 3);
  buffer[ 9] = (unsigned char)((MC_CAN_BASE1) << 5);
  buffer[10] = 0x00;
  buffer[11] = 0x00;
  can0_write(RXF0SIDH, &buffer[0], 12 );
  
  // RXF3 - Buffer 1
  buffer[ 0] = 0x00;
  buffer[ 1] = 0x00;
  buffer[ 2] = 0x00;
  buffer[ 3] = 0x00;
  // RXF4 - Buffer 1
  buffer[ 4] = 0x00;
  buffer[ 5] = 0x00;
  buffer[ 6] = 0x00;
  buffer[ 7] = 0x00;
  // RXF5 - Buffer 1
  buffer[ 8] = 0x00;
  buffer[ 9] = 0x00;
  buffer[10] = 0x00;
  buffer[11] = 0x00;
  can0_write(RXF3SIDH, &buffer[0], 12 );
  
  // RXM0 - Buffer 0
//  buffer[ 0] = 0xFF;       // Match entire 11 bit ID (ID is left-justified in 32-bit mask register)
  buffer[ 0] = 0xC0;     // Match 2 MSB of 11 bit ID (ID is left-justified in 32-bit mask register)
//  buffer[ 1] = 0xE0;
  buffer[ 1] = 0x00;
  buffer[ 2] = 0x00;
  buffer[ 3] = 0x00;
  // RXM1 - Buffer 1
//  buffer[ 4] = 0xFF;		// Match entire 11 bit ID (ID is left-justified in 32-bit mask register)
  buffer[ 4] = 0xC0;		//Match 2 MSB of 11 bit ID (ID is left-justified in 32-bit mask register)
//  buffer[ 5] = 0xE0;
  buffer[ 5] = 0x00;
  buffer[ 6] = 0x00;
  buffer[ 7] = 0x00;
  can0_write(RXM0SIDH, &buffer[0], 8 );
  
//  buffer[0] = 0x04;	//enable filters & rollover
//  can0_write(RXB0CTRL, &buffer[0], 1);
//  buffer[0] = 0x04;	//enable filters
//  can0_write(RXB1CTRL, &buffer[0], 1);
        
  // Switch out of config mode into normal operating mode
  //can_mode(interface, CANCTRL, 0xE0, 0x00 );			// CANCTRL register, modify upper 3 bits, mode = Normal
  can0_canctrl(0xE0, 0x00);
}

void can2_init()
{
  
}

void can0_sources(unsigned addr0, unsigned addr1) {
//	can_read(interface, CANCTRL, &buffer[0], 1);
//	buffer[0] = (buffer[0] & 0x1F) | 0x84;
//	can_write(interface, CANCTRL, &buffer[0], 1);

	can0_canctrl(0xE0, 0x80);
	
	buffer[0] = buffer[4] = 0xFF;
	buffer[1] = buffer[5] = 0xE0;
	buffer[2] = buffer[6] = 0x00;
	buffer[3] = buffer[7] = 0x00;
	can0_write(RXM0SIDH, &buffer[0], 8);

	buffer[0] = (unsigned char)(addr0 >> 3);
	buffer[1] = (unsigned char)(addr0 << 5);
	buffer[2] = buffer[3] = 0;
	buffer[4] = (unsigned char)(addr1 >> 3);
	buffer[5] = (unsigned char)(addr1 << 5);
	buffer[6] = buffer[7] = 0;
	buffer[8] = buffer[9] = 0;
	buffer[10] = buffer[11] = 0;
	can0_write(RXF0SIDH, &buffer[0], 12);
	
	buffer[0] = buffer[1] = 0;
	buffer[4] = buffer[5] = 0;
	can0_write(RXF3SIDH, &buffer[0], 12);
	
	//can_read(interface, CANCTRL, &buffer[0], 1);
	//buffer[0] = buffer[0] & 0x1F;
	//can_write(interface, CANCTRL, &buffer[0], 1);
	
	can0_canctrl(0xE0, 0x00);
}

/*
* Receives a CAN message from the MCP2515
*	- Run this routine when an IRQ is received
*	- Query the controller to identify the source of the IRQ
*		- If it was an ERROR IRQ, read & clear the Error Flag register, and return it
*		- If it was an RX IRQ, read the message and address, and return them
*		- If both, handle the error preferentially to the message
*	- Clear the appropriate IRQ flag bits
*/
void can0_receive()
{
  can_message *RXPtr_can_message;
  unsigned char flags;
  extern char ucFLAG;
     
  can_msg_count++;
//  if (can_msg_count == 0x0040)
//  {
//    can_msg_count = 0;	// Breakpoint to observe message queue
//  }
  
  RXPtr_can_message=&(*decode_queue.PutPt);
// RXPtr_can_message=&(decode_queue.msg_fifo[0]);
//  RXPtr_can_message=&decode_queue[decode_queue->PutPt];
//  RXPtr_can_message = &RX_can_message;	// Local storage
  
    // Read out the interrupt flags register
  can0_read(CANINTF, &flags, 1 );
  
  // Check for errors
  if(( flags & MCP_IRQ_ERR ) != 0x00 )
  {
    
// Read error flags and counters
    can0_read(EFLAG, &buffer[0], 1 );
    can0_read(TEC, &buffer[1], 2 );
    // Clear error flags
    can0_mode(EFLAG, buffer[0], 0x00 );	// Modify (to '0') all bits that were set
    // Return error code, a blank address field, and error registers in data field
    RXPtr_can_message->status = CAN_ERROR;
    RXPtr_can_message->address = 0x0000;
    RXPtr_can_message->data[0] = flags;		// CANINTF
    RXPtr_can_message->data[1] = buffer[0];	// EFLG
    RXPtr_can_message->data[2] = buffer[1];	// TEC
    RXPtr_can_message->data[3] = buffer[2];	// REC
    // Clear the IRQ flag
    can0_mode(CANINTF, MCP_IRQ_ERR, 0x00 );
    can_err_count++;
  }	
  
// No error, check for received messages, buffer 0
  else if(( flags & MCP_IRQ_RXB0 ) != 0x00 ){
    // Read in the info, address & message data
    can0_read(RXB0CTRL, &buffer[0], 14 );
    // Clear the IRQ flag as fast as possible
    can0_mode(CANINTF, MCP_IRQ_RXB0, 0x00 );
    
    // Fill out return structure
    // check for Remote Frame requests and indicate the status correctly
    if(( buffer[0] & MCP_RXB0_RTR ) == 0x00 )
    {
      RXPtr_can_message->status = CAN_OK;                         // We've received a standard data packet
      fill_data(RXPtr_can_message, buffer);                      // Fill in the data
    }
    else
    {
      RXPtr_can_message->status = CAN_RTR;                         // We've received a remote frame request - data irrelevant
    }
    RXPtr_can_message->address = ((int)(buffer[1]) << 3) | ((int)(buffer[2]) >> 5);          //convert address

    //add message to queue to be decoded
    message_fifo_PUT(&decode_queue, *RXPtr_can_message); 
    ucFLAG |= 0x20;
    can_read_cnt++;
  }
  
// No error, check for received messages, buffer 1
  else if(( flags & MCP_IRQ_RXB1 ) != 0x00 )
  {
    // Read in the info, address & message data
    can0_read(RXB1CTRL, &buffer[0], 14 );
    // Clear the IRQ flag as fast as possible
    can0_mode(CANINTF, MCP_IRQ_RXB1, 0x00 );

    // Fill out return structure
    // check for Remote Frame requests and indicate the status correctly
    if(( buffer[0] & MCP_RXB1_RTR ) == 0x00 )
    {
      RXPtr_can_message->status = CAN_OK;                         // We've received a standard data packet
      fill_data(RXPtr_can_message, buffer);                      // Fill in the data
    }
    else
    {
      RXPtr_can_message->status = CAN_RTR;                         // We've received a remote frame request - data irrelevant
    }
    RXPtr_can_message->address = ((int)(buffer[1]) << 3) | ((int)(buffer[2]) >> 5);          //convert address
    
    message_fifo_PUT(&decode_queue, *RXPtr_can_message);            //add message to queue to be decoded
    ucFLAG |= 0x20;
    can_read_cnt++;
  }
//not handled by buffers or errors
  else
  {
    RXPtr_can_message->status = CAN_ERROR;
    RXPtr_can_message->address = 0x0001;
    RXPtr_can_message->data[0] = flags;		// CANINTF
    can_err_count++;
  }
  
  //added to tritum code to account for the fact that we could have more then one intrrupt pending at a given time
  //NOTE: YOU MUST CLEAR THE MAIN_MODE STATUS BEFORE ENTERING THIS FUNCTION OR YOU WILL CLEAR THE RETURN FLAG
  if((P2IN & CAN0_INTn)==0){
  	can0_read(CANINTF, &flags, 1 );
    if(( flags & MCP_IRQ_ERR ) != 0x00 )
    {
      canstat = 1;
      can0_read(EFLAG, &buffer[0], 1 );
      can0_read(TEC, &buffer[1], 2 );
      // Clear error flags
      can0_mode(EFLAG, buffer[0], 0x00 );	// Modify (to '0') all bits that were set
      // Clear the IRQ flag
      can0_mode(CANINTF, MCP_IRQ_ERR, 0x00 );
    }
  }
}

/*
* Transmits a CAN message to the bus
*	- Busy waits while message is sent to CAN controller
*	- Uses all available transmit buffers (3 available in CAN controller) to maximise throughput
*	- Only modifies address information if it's different from what is already set up in CAN controller
*	- Assumes constant 8-byte data length value
*/
void can0_transmit()
{
  static unsigned int buf_addr[3] = {0xFFFF, 0xFFFF, 0xFFFF};
  
  // Fill data into buffer, it's used by any address
  // Allow room at the start of the buffer for the address info if needed
  buffer[ 5] = TX_can_message.data[0];
  buffer[ 6] = TX_can_message.data[1];
  buffer[ 7] = TX_can_message.data[2];
  buffer[ 8] = TX_can_message.data[3];
  buffer[ 9] = TX_can_message.data[4];
  buffer[10] = TX_can_message.data[5];
  buffer[11] = TX_can_message.data[6];
  buffer[12] = TX_can_message.data[7];
  
  // Check if the incoming address has already been configured in a mailbox
  if( TX_can_message.address == buf_addr[0] ){
    // Mailbox 0 setup matches our new message
    // Write to TX Buffer 0, start at data registers, and initiate transmission
    can0_write_tx(0x01, &buffer[5] );
    can0_rts(0);
  }
  else if( TX_can_message.address == buf_addr[1] ){
    // Mailbox 1 setup matches our new message
    // Write to TX Buffer 1, start at data registers, and initiate transmission
    can0_write_tx(0x03, &buffer[5] );
    can0_rts(1 );
  }
  else if( TX_can_message.address == buf_addr[2] ){
    // Mailbox 2 setup matches our new message
    // Write to TX Buffer 2, start at data registers, and initiate transmission
    can0_write_tx(0x05, &buffer[5] );
    can0_rts(2 );
  }
  else{
    // No matches in existing mailboxes
    // No mailboxes already configured, so we'll need to load an address - set it up
    buffer[0] = (unsigned char)(TX_can_message.address >> 3);
    buffer[1] = (unsigned char)(TX_can_message.address << 5);
    buffer[2] = 0x00;						// EID8
    buffer[3] = 0x00;						// EID0
    buffer[4] = 0x08;						// DLC = 8 bytes
    
    // Check if we've got any un-setup mailboxes free and use them
    // Otherwise, find a non-busy mailbox and set it up with our new address
    if( buf_addr[0] == 0xFFFF ){			// Mailbox 0 is free
      // Write to TX Buffer 0, start at address registers, and initiate transmission
      can0_write_tx(0x00, &buffer[0] );
      can0_rts(0);
      buf_addr[0] = TX_can_message.address;
    }									
    else if( buf_addr[1] == 0xFFFF ){		// Mailbox 1 is free
      // Write to TX Buffer 1, start at address registers, and initiate transmission
      can0_write_tx(0x02, &buffer[0] );
      can0_rts(1);
      buf_addr[1] = TX_can_message.address;
    }
    else if( buf_addr[2] == 0xFFFF ){		// Mailbox 2 is free
      // Write to TX Buffer 2, start at address registers, and initiate transmission
      can0_write_tx(0x04, &buffer[0] );
      can0_rts(2);
      buf_addr[2] = TX_can_message.address;
    }
    else {					
      
      // No mailboxes free, wait until at least one is not busy
      while(( can0_read_status() & 0x54 ) == 0x54);
      // Is it mailbox 0?
      if(( can0_read_status() & 0x04 ) == 0x00) {
        // Setup mailbox 0 and send the message
        can0_write_tx(0x00, &buffer[0] );
        can0_rts(0);
        buf_addr[0] = TX_can_message.address;
      }
      // Is it mailbox 1?
      else if(( can0_read_status() & 0x10 ) == 0x00) {
        // Setup mailbox 1 and send the message
        can0_write_tx(0x02, &buffer[0] );
        can0_rts(1);
        buf_addr[1] = TX_can_message.address;
      }
      // Is it mailbox 2?
      else if(( can0_read_status() & 0x40 ) == 0x00) {
        // Setup mailbox 2 and send the message
        can0_write_tx(0x04, &buffer[0] );
        can0_rts(2);
        buf_addr[2] = TX_can_message.address;
      }
    }			
  }
}


/**************************************************************************************************
* PRIVATE FUNCTIONS
*************************************************************************************************/

//fills data of a message from the buffer
void fill_data(can_message *can, unsigned char buff[])
{
      can->data[0] = buffer[ 6];
      can->data[1] = buffer[ 7];
      can->data[2] = buffer[ 8];
      can->data[3] = buffer[ 9];
      can->data[4] = buffer[10];
      can->data[5] = buffer[11];
      can->data[6] = buffer[12];
      can->data[7] = buffer[13];
}

/*
* Resets MCP2515 CAN controller via SPI port
*	- SPI port must be already initialised
*/
void can0_reset()
{
  can0_select();
  spi1_transmit(MCP_RESET );
  can0_deselect();
}

/*
* Reads data bytes from the MCP2515
*	- Pass in starting address, pointer to array of bytes for return data, and number of bytes to read
*/
unsigned char can0_result[10] = {0,0,0,0,0,0,0,0,0,0};

void can0_read(unsigned char address, unsigned char *ptr, unsigned char bytes )
{
  unsigned char i;
  
  if(bytes > 1) {
  	can0_select();
  	spi1_transmit(MCP_READ );
  	spi1_transmit(address );
  	for( i = 0; i < bytes; i++ )
  	{
	    *ptr++ = spi1_exchange(0x00);
  	}
  	can0_deselect();
  	
  	if(address == CANCTRL) can0_result[0] = *--ptr;
  } else {
  	can0_select();
  	spi1_transmit(MCP_READ);
  	spi1_transmit(address);
  	*ptr = spi1_exchange(0);
  	can0_deselect();
  	
  	if(address == CANCTRL) can0_result[0] = *ptr;
  	if(address == 0x2C) can0_result[1] = *ptr;
  }
}

/*
* Reads data bytes from receive buffers
*	- Pass in buffer number and start position as defined in MCP2515 datasheet
*		- For starting at data, returns 8 bytes
*		- For starting at address, returns 13 bytes
*/
void can0_read_rx(unsigned char address, unsigned char *ptr )
{
  unsigned char i;
  
  address &= 0x03;			// Force upper bits of address to be zero (they're invalid)
  address <<= 1;			// Shift input bits to correct location in command byte
  address |= MCP_READ_RX;		// Construct command byte for MCP2515
  
  can0_select();
  spi1_transmit(address );
  
  if(( address & 0x02 ) == 0x00 )       // Start at address registers
  {	
    for( i = 0; i < 13; i++ )
    {
      *ptr++ = spi1_exchange(0x00);
    }
  }
  else                                  // Start at data registers
  {									
    for( i = 0; i < 8; i++ )
    {
      *ptr++ = spi1_exchange(0x00 );
    }
  }
  can0_deselect();
}

/*
* Writes data bytes to the MCP2515
*	- Pass in starting address, pointer to array of bytes, and number of bytes to write
*/
void can0_write(unsigned char address, unsigned char *ptr, unsigned char bytes )
{
  unsigned char i;
  
  can0_select();
  spi1_transmit(MCP_WRITE );
  spi1_transmit(address );
  for( i = 0; i < (bytes-1); i++ ){
    spi1_transmit(*ptr++ );
  }
  spi1_transmit(*ptr );
  can0_deselect();
}

/*
* Writes data bytes to transmit buffers
*	- Pass in buffer number and start position as defined in MCP2515 datasheet
*		- For starting at data, accepts 8 bytes
*		- For starting at address, accepts 13 bytes
*/
void can0_write_tx(unsigned char address, unsigned char *ptr )
{
  unsigned char i;
  
  address &= 0x07;				// Force upper bits of address to be zero (they're invalid)
  address |= MCP_WRITE_TX;			// Construct command byte for MCP2515
  
  can0_select();
  spi1_transmit(address );
  
  if(( address & 0x01 ) == 0x00 ){		// Start at address registers
    for( i = 0; i < 13; i++ ){
      spi1_transmit(*ptr++);
    }
  }
  else{						// Start at data registers
    for( i = 0; i < 8; i++ ){
      spi1_transmit(*ptr++);
    }
  }
  can0_deselect();
}

/*
* Request to send selected transmit buffer
*	- Pass in address of buffer to transmit: 0, 1 or 2
*/
void can0_rts(unsigned char address )
{
  unsigned char i;
  
  // Set up address bits in command byte
  i = MCP_RTS;
  if( address == 0 ) i |= 0x01;
  else if( address == 1 ) i |= 0x02;
  else if( address == 2 ) i |= 0x04;
  
  // Write command
  can0_select();
  spi1_transmit(i);
  can0_deselect();
}

/*
* Reads MCP2515 status register
*/
unsigned char can0_read_status()
{
  unsigned char status;
  
  can0_select();
  spi1_transmit(MCP_STATUS );
  status = spi1_exchange(0x00 );
  can0_deselect();
  return status;
}

/*
* Reads MCP2515 RX status (filter match) register
*/
unsigned char can0_read_filter()
{
  unsigned char status;
  
  can0_select();
  spi1_transmit(MCP_FILTER );
  status = spi1_exchange(0x00);
  can0_deselect();
  return status;
}

/*
* Modifies selected register in MCP2515
*	- Pass in register to be modified, bit mask, and bit data
*/
void can0_mode(unsigned char address, unsigned char mask, unsigned char data )
{
  can0_select();
  spi1_transmit(MCP_MODIFY);
  spi1_transmit(address);
  spi1_transmit(mask);
  spi1_transmit(data);
  can0_deselect();
}
