/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

// STL headers
// C headers
// Framework headers
// Library headers
#include "RF24Network.h"

// Project headers
// This component's header
#include <Finder.h>

extern RF24Network network;

// Message buffer space
static uint8_t message[32];

/****************************************************************************/
  
Finder::Finder(void): this_node(0), state(state_waiting), 
  last_sent(millis()-interval), child_increment(-1) 
{
}

/****************************************************************************/

void Finder::begin(uint16_t _this_node)
{
  this_node = _this_node;
  
  if ( ! ( this_node & 07000 ) )
  {
    // Figure out the address of the first child.  e.g. if our node is 045, our
    // first child is 0145.  So we need to shift 01 up enough places to be the
    // highest digit
    child_increment = 01;
    uint16_t temp = this_node;
    while ( temp )
    {
      child_increment <<= 3;
      temp >>= 3;
    }
  }

  printf_P(PSTR("%lu: Node %o increment %o\r\n"),millis(),this_node,child_increment);
}

/****************************************************************************/

void Finder::update(void)
{
  // Check the network for traffic

  // If we got a new finder request, launch!
  if ( network.available() )
  {
    RF24NetworkHeader header;
    network.peek(header);
    if ( header.type == 'F' )
    {
      network.read(header,message,sizeof(message));
      uint16_t from = header.from_node;
      printf_P(PSTR("%lu: APP Received FINDER request from %o\r\n"),millis(),from);

      if ( state == state_sending )
      {
	printf_P(PSTR("%lu: APP ERROR, already sending a finder request\r\n"),millis());
      }
      else if ( child_increment == 0xffff )
      {
	printf_P(PSTR("%lu: APP This app has no children, done.\r\n"),millis());
	last_sent = millis() - interval;
	state = state_done;
	goto finish;
      }
      else
      {
	last_sent = millis() - interval;
	to_node = this_node + child_increment;
	state = state_sending;
	goto finish;
      }
    }
  }

  // If we're working but not ready, continue
  if ( state != state_waiting && millis() - last_sent <= interval )
    return;

  // If we're working, send!
  if ( state == state_sending )
  {
    RF24NetworkHeader header(to_node,'F');
    /*bool ok = */ network.write(header,message,sizeof(message));
    last_sent = millis();
    
    to_node += child_increment;
    
    // Done?
    if ( to_node > this_node + 5 * child_increment )
    {
      state = state_done;
      goto finish;
    }
  }
  
  // If we're now done, send the final 'E' back
  if ( state == state_done )
  {
    // Send an 'E' Echo response back to the BASE
    RF24NetworkHeader header(00,'E');
    network.write(header,message,sizeof(message));
    
    state = state_waiting;
    goto finish;
  }
finish:
  return;
}

/****************************************************************************/

// vim:cin:ai:sts=2 sw=2 ft=cpp
