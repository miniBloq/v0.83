/*
  HardwareSerial.cpp - Hardware serial library for Wiring
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
  
  Modified 23 November 2006 by David A. Mellis
*/

#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "wiring.h"
#include "wiring_private.h"

#include "VirtualSerial.h"

//=======================================================================
// Virtual serial configuration
//=======================================================================
#include "HID.h"
#define STREAM_SEND	HID_Send
//=======================================================================

// Define constants and variables for buffering incoming serial data.  We're
// using a ring buffer (I think), in which rx_buffer_head is the index of the
// location to which to write the next incoming character and rx_buffer_tail
// is the index of the location from which to read.

#define RX_BUFFER_SIZE 128

struct ring_buffer {
  unsigned char buffer[RX_BUFFER_SIZE];
  int head;
  int tail;
};

ring_buffer rx_buffer_cdc = { { 0 }, 0, 0 };

inline void store_char(unsigned char c, ring_buffer *rx_buffer)
{
  int i = (rx_buffer->head + 1) % RX_BUFFER_SIZE;

  // if we should be storing the received character into the location
  // just before the tail (meaning that the head would advance to the
  // current location of the tail), we're about to overflow the buffer
  // and so we don't write the character or advance the head.
  if (i != rx_buffer->tail) {
    rx_buffer->buffer[rx_buffer->head] = c;
    rx_buffer->head = i;
  }
  
}

void VirtualSerial::store(char c){
	store_char(c, &rx_buffer_cdc);
}


// Constructors ////////////////////////////////////////////////////////////////
VirtualSerial::VirtualSerial(ring_buffer *rx_buffer){
	_rx_buffer = rx_buffer;
}
// Public Methods //////////////////////////////////////////////////////////////

void VirtualSerial::begin(long baud)
{
}

void VirtualSerial::end()
{ 
}

uint8_t VirtualSerial::available(void)
{	
	return (RX_BUFFER_SIZE + _rx_buffer->head - _rx_buffer->tail) % RX_BUFFER_SIZE;
}

int VirtualSerial::read(void)
{	
  // if the head isn't ahead of the tail, we don't have any characters
  if (_rx_buffer->head == _rx_buffer->tail) {
    return -1;
  } else {
    unsigned char c = _rx_buffer->buffer[_rx_buffer->tail];
    _rx_buffer->tail = (_rx_buffer->tail + 1) % RX_BUFFER_SIZE;
    return c;
  }
}

void VirtualSerial::flush()
{
  // don't reverse this or there may be problems if the RX interrupt
  // occurs after reading the value of rx_buffer_head but before writing
  // the value to rx_buffer_tail; the previous value of rx_buffer_head
  // may be written to rx_buffer_tail, making it appear as if the buffer
  // don't reverse this or there may be problems if the RX interrupt
  // occurs after reading the value of rx_buffer_head but before writing
  // the value to rx_buffer_tail; the previous value of rx_buffer_head
  // may be written to rx_buffer_tail, making it appear as if the buffer
  // were full, not empty.
  _rx_buffer->head = _rx_buffer->tail;
}

void VirtualSerial::write(uint8_t c){
	char str[2]={c,'\0'};
	STREAM_SEND(( uint8_t *)str);
}


void VirtualSerial::write(const uint8_t *buffer, size_t size)
{	
	STREAM_SEND(( uint8_t *)buffer);
}

void VirtualSerial::write(const char *str)
{	
	STREAM_SEND(( uint8_t *)str);
}



// Preinstantiate Objects //////////////////////////////////////////////////////
VirtualSerial Serial(&rx_buffer_cdc);
