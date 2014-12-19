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
#include "RF24Network_config.h"
// Project headers
// This component's header
#include "S_message.h"

char S_message::buffer[32];

/****************************************************************************/

char* S_message::toString(void)
{
  snprintf(buffer,sizeof(buffer),"%2u.%02uC /%2u.%02uV",
      temp_reading >> 8,
      ( temp_reading & 0xFF ) * 100 / 256,
      voltage_reading >> 8,
      ( voltage_reading & 0xFF ) * 100 / 256
      );
  return buffer;
}

/****************************************************************************/
// vim:cin:ai:sts=2 sw=2 ft=cpp
