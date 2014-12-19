/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

// STL headers
// C headers
#include <stdlib.h>
// Framework headers
// Library headers
#include <RF24Network.h>
// Project headers
// This component's header
#include <Sync.h>

/****************************************************************************/

void Sync::update(void)
{
  // Pump the network
  network.update();

  // Look for changes to the data
  uint8_t message[32];
  uint8_t *mptr = message;
  unsigned at = 0;
  while ( at < len )
  {
    if ( app_data && internal_data && app_data[at] != internal_data[at] )
    {
      // Compose a message with the deltas
      *mptr++ = at + 1;
      *mptr++ = app_data[at];

      // Update our internal view
      internal_data[at] = app_data[at];
    }
    ++at;
  }
  // Zero out the remainder
  while ( at++ < sizeof(message) )
    *mptr++ = 0;

  // If changes, send a message
  if ( *message )
  {
    // TODO handle the case where this has to be broken into
    // multiple messages
    RF24NetworkHeader header(/*to node*/ to_node, /*type*/ 'S' /*Sync*/);
    network.write(header,message,sizeof(message));
  }

  // Look for messages from the network
  // Is there anything ready for us?
  if ( network.available() )
  {
    // If so, take a look at it
    RF24NetworkHeader header;
    network.peek(header);

    switch (header.type)
    {
    case 'S':
      IF_SERIAL_DEBUG(printf_P(PSTR("%lu: SYN Received sync message\n\r"),millis()));

      network.read(header,&message,sizeof(message));
      // Parse the message and update the vars
      mptr = message;
      at = 0;
      while ( mptr < message + sizeof(message) )
      {
        // A '0' in the first position means we are done
        if ( !*mptr )
          break;
        uint8_t pos = (*mptr++) - 1;
        uint8_t val = *mptr++;

        IF_SERIAL_DEBUG(printf_P(PSTR("%lu: SYN Updated position %u to value %u\n\r"),millis(),pos,val));

        app_data[pos] = val;
        internal_data[pos] = val;
      }
      break;
    default:
      // Leave other messages for the app
      break;
    };
  }
}
// vim:cin:ai:sts=2 sw=2 ft=cpp
