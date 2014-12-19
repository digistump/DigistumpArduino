/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

#ifndef __TIMER_H__
#define __TIMER_H__

// STL headers
// C headers
// Framework headers
// Library headers
// Project headers

/**
 * Simple timer 
 */

struct timer_t 
{
unsigned long last;
unsigned long interval;
timer_t(unsigned long _interval): interval(_interval) {}
operator bool(void)
{
  unsigned long now = millis();
  bool result = now - last >= interval;
  if ( result )
    last = now; 
  return result;
}
};

#endif // __TEMPLATE_H__
// vim:cin:ai:sts=2 sw=2 ft=cpp
