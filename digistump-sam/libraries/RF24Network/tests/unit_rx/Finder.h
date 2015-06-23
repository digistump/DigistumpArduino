/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

#ifndef __FINDER_H__
#define __FINDER_H__

// STL headers
// C headers
// Framework headers
#include <Arduino.h>
// Library headers
// Project headers

/**
 * Manage a child-finder request 
 */

class Finder
{
private:
  uint16_t this_node;
  enum state_e { state_none = 0, state_waiting, state_sending, state_done, state_invalid };
  state_e state;
  static const unsigned long interval = 50;
  unsigned long last_sent;
  uint16_t to_node;
  uint16_t child_increment;
protected:
  
public:
  Finder(void);
  void begin(uint16_t);
  void update(void);
};

#endif // __FINDER_H__
// vim:cin:ai:sts=2 sw=2 ft=cpp
