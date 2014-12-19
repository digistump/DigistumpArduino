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
#include <RF24.h>
#include <RF24Network.h>
// Project headers
// This component's header
#include <Globals.h>

#include "WProgram.h"

/****************************************************************************/

HardwareSPI SPI(2);

// Radio on Maple Native w/ Getting Started board
RF24 radio(7,6);
RF24Network network(radio);

// vim:cin:ai:sts=2 sw=2 ft=cpp
