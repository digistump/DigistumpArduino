/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

#ifndef __S_MESSAGE_H__
#define __S_MESSAGE_H__

// STL headers
// C headers
// Framework headers
// Library headers
// Project headers

/**
 * Sensor message (type 'S') 
 */

struct S_message
{
  uint16_t temp_reading;
  uint16_t voltage_reading;
  static char buffer[];
  S_message(void): temp_reading(0), voltage_reading(0) {}
  char* toString(void);
};

#endif // __S_MESSAGE_H__
// vim:cin:ai:sts=2 sw=2 ft=cpp
