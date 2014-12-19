/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

#ifndef __SYNC_H__
#define __SYNC_H__

// STL headers
// C headers
#include <stdlib.h>
#include <string.h>
// Framework headers
// Library headers
#include <RF24Network_config.h>
// Project headers

class RF24Network;

/**
 * Synchronizes a shared set of variables between multiple nodes
 */

class Sync
{
private:
  RF24Network& network;
  uint8_t* app_data; /**< Application's copy of the data */
  uint8_t* internal_data; /**< Our copy of the data */
  size_t len; /**< Length of the data in bytes */
  uint16_t to_node; /**< The other node we're syncing with */

protected:
public:
  /**
   * Constructor
   *
   * @param _network Which network to syncrhonize over
   */
  Sync(RF24Network& _network): network(_network), app_data(NULL),
    internal_data(NULL), len(0), to_node(0)
  {
  }
  /**
   * Begin the object
   *
   * @param _to_node Which node we are syncing with
   */
  void begin(uint16_t _to_node)
  {
    to_node = _to_node;
  }
  /**
   * Declare the shared data set
   *
   * @param _data Location of shared data to be syncrhonized
   */
  template <class T>
  void register_me(T& _data)
  {
    app_data = reinterpret_cast<uint8_t*>(&_data);
    len = sizeof(_data);
    internal_data = reinterpret_cast<uint8_t*>(malloc(len));
    reset();
  }

  /**
   * Reset the internal copy of the shared data set 
   */
  void reset(void)
  {
    memcpy(internal_data,app_data,len);
  }
  
  /**
   * Update the network and the shared data set
   */
  void update(void);
};

#endif // __SYNC_H__
// vim:cin:ai:sts=2 sw=2 ft=cpp
