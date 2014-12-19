
/*
 Copyright (C) 2011 James Coliz, Jr. <maniacbug@ymail.com>
 
 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

/**
 * @file sleep.h 
 *
 * Declaration of the sleep_c class. 
 */
 
#ifndef __SLEEP_H__
#define __SLEEP_H__

/**
 * Enums for the duration of the watchdog timer
 */

typedef enum { wdt_16ms = 0, wdt_32ms, wdt_64ms, wdt_128ms, wdt_250ms, wdt_500ms, wdt_1s, wdt_2s, wdt_4s, wdt_8s } wdt_prescalar_e;

/**
 * Simplified sleeping handler
 */

class sleep_c
{
public:
  /**
   * Enable the system to be able to sleep
   *
   * Does not actually do any sleeping.
   *
   * For example, to do something roughly evert minute, configure
   * it like this:
   * 
   * @code
   *  Sleep.begin(wdt_8s,7);
   * @endcode
   *
   * @param prescalar Duration of the watchdog timer interrupt.
   * The system will actually sleep for this long.
   * @param cycles Number of times the system will wake up before
   * returning from @p go().
   */
  void begin(wdt_prescalar_e prescalar,short cycles);

  /**
   * Go to sleep 
   *
   * This will return after the watchdog has awoken for the number
   * of times specified in begin().
   */
  void go(void);

  /**
   * Test whether the node sleeps
   *
   * @retval true if the node will sleep
   */
  operator bool(void) const 
  {
    return sleep_cycles_per_transmission;
  }

private:
  volatile short sleep_cycles_remaining;
  short sleep_cycles_per_transmission; 
};

/**
 * Singleton instance for general use
 *
 * @warning: This class is hard-coded to ONLY work with this singleton.
 * Any other instances will fail.
 */

extern sleep_c Sleep;

#endif // __SLEEP_H__
// vim:ai:cin:sts=2 sw=2 ft=cpp
