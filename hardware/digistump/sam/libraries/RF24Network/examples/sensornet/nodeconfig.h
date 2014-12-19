/*
 Copyright (C) 2011 James Coliz, Jr. <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

#ifndef __NODECONFIG_H__
#define __NODECONFIG_H__

// Additional info
struct eeprom_info_t
{
  uint8_t flag;
  uint16_t address;
  int16_t temp_calibration;  // sensor adjustment in signed fixed-8.8 degrees, e.g. 0xFE80 is -1.5
  bool relay:1;

  static const uint8_t valid_flag = 0xdd;

  eeprom_info_t()
  {
    clear();
  }

  bool isValid() const
  {
    return (flag == valid_flag) && (address != 0xffff);
  }

  void clear()
  {
    flag = valid_flag;
    address = 0xffff;
    relay = false;
    temp_calibration = 0;
  }

};

const eeprom_info_t& nodeconfig_read(void);
void nodeconfig_listen(void);
void set_temp_calibration(int16_t val);

#endif // __NODECONFIG_H__

// vim:ai:cin:sts=2 sw=2 ft=cpp

