/*
 Copyright (C) 2011 James Coliz, Jr. <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

#include "RF24Network_config.h"
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include "nodeconfig.h"

// Where in EEPROM is the address stored?
uint8_t* address_at_eeprom_location = (uint8_t*)10;

eeprom_info_t eeprom_info;

const eeprom_info_t& nodeconfig_read(void)
{
  eeprom_read_block(&eeprom_info,address_at_eeprom_location,sizeof(eeprom_info));

  // Look for the token in EEPROM to indicate the following value is
  // a validly set node address 
  if ( eeprom_info.isValid() ) 
  {
    printf_P(PSTR("ADDRESS: %o\n\r"),eeprom_info.address);
    printf_P(PSTR("ROLE: %S\n\r"),eeprom_info.relay ? PSTR("Relay") : PSTR("Leaf") );
    printf_P(PSTR("TEMP: %04x\n\r"),eeprom_info.temp_calibration); 
  }
  else
  {
    eeprom_info.clear();

    printf_P(PSTR("*** No valid address found.  Send node address via serial of the form 011<cr>\n\r"));
    while(1)
    {
      nodeconfig_listen();
    }
  }
  
  return eeprom_info;
}

char serialdata[10];
char* nextserialat = serialdata;
const char* maxserial = serialdata + sizeof(serialdata) - 1;

void nodeconfig_listen(void)
{
  //
  // Listen for serial input, which is how we set the address
  //
  if (Serial.available())
  {
    // If the character on serial input is in a valid range...
    char c = tolower(Serial.read());
    if ( (c >= '0' && c <= '9' ) || (c >= 'a' && c <= 'f' ) ) 
    {
      *nextserialat++ = c;
      if ( nextserialat == maxserial )
      {
	*nextserialat = 0;
	printf_P(PSTR("\r\n*** Unknown serial command: %s\r\n"),serialdata);
	nextserialat = serialdata;
      }
    }
    else if ( tolower(c) == 'r' )
    {
      eeprom_info.relay = true;
      printf_P(PSTR("ROLE: %S\n\r"),eeprom_info.relay ? PSTR("Relay") : PSTR("Leaf") );
      eeprom_update_block(&eeprom_info,address_at_eeprom_location,sizeof(eeprom_info));
      printf_P(PSTR("RESET NODE before changes take effect\r\n"));
      if ( ! eeprom_info.isValid() )
	printf_P(PSTR("Please assign an address\r\n"));
    }
    else if ( tolower(c) == 'l' )
    {
      eeprom_info.relay = false;
      printf_P(PSTR("ROLE: %S\n\r"),eeprom_info.relay ? PSTR("Relay") : PSTR("Leaf") );
      eeprom_update_block(&eeprom_info,address_at_eeprom_location,sizeof(eeprom_info));
      printf_P(PSTR("RESET NODE before changes take effect\r\n"));
      if ( ! eeprom_info.isValid() )
	printf_P(PSTR("Please assign an address\r\n"));
    }
    else if ( tolower(c) == 't' )
    {
      // Send temperature calibration as 2-digit 4.4-fixed decimal signed degrees
      // celcius--followed by a 't'.  So, for -1.5 degrees, send "e8t".  Or for +2 degrees
      // send "20t"

      *nextserialat = 0;
      int8_t val = strtol(serialdata,NULL,16);
      nextserialat = serialdata;
      
      set_temp_calibration( val * 0x10 );
      
      printf_P(PSTR("RESET NODE before changes take effect\r\n"));
      if ( ! eeprom_info.isValid() )
	printf_P(PSTR("Please assign an address\r\n"));
    }
    else if ( c == 13 )
    {
      // Convert to octal
      char *pc = serialdata;
      uint16_t address = 0;
      while ( pc < nextserialat )
      {
	address <<= 3;
	address |= (*pc++ - '0');
      }

      // It is our address
      eeprom_info.address = address;
      eeprom_update_block(&eeprom_info,address_at_eeprom_location,sizeof(eeprom_info));

      // And we are done right now (no easy way to soft reset)
      printf_P(PSTR("\n\rManually set to address 0%o\n\rPress RESET to continue!"),address);
      while(1);
    }
  }
}

void set_temp_calibration(int16_t val)
{
  eeprom_info.temp_calibration = val;
  printf_P(PSTR("TEMP: %02x\n\r"),eeprom_info.temp_calibration);
  eeprom_update_block(&eeprom_info,address_at_eeprom_location,sizeof(eeprom_info));
}

// vim:ai:cin:sts=2 sw=2 ft=cpp
