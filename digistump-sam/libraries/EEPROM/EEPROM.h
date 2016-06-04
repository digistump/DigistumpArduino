/*
  EEPROM.h - EEPROM library for the Digistump DigiX

  This is mostly the merger of the stock Arduino EEPROM library, original
  Copyright (c) 2006 David A. Mellis/Christopher Andrews 2015 (LGPL 2.1) with a
  few functions from Dennis Schweer (Inglorious Engineer)'s Extensive TWI/I2C
  EEPROM Library for 24LCxxx devices v0.4.1 (CC-BY-SA), as distributed with the
  DigiX core.

  Merging work by SukkoPera <software@sukkology.net>, 2016.

  The basic idea is to create a library that is interface-compatible with
  Arduino's stock library that is used on non-Due boards, to achieve
  compatibility at source level, so that the usual EEPROM.write()/read()/get()
  /put() calls can be used.

  I'm not really sure of what license the resulting code should be released
  under, so I'm leaving the LGPL 2.1, as most of the code here actually came
  under that license, and the rest is pretty basic stuff, but let me know if I
  should change that.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef EEPROM_h
#define EEPROM_h

#include <inttypes.h>
#include <Wire.h>

static const int DIGIX_EEPROM_ADDR = 0x50;

static const int E2END = 0xFFF;

/***
    EERef class.

    This object references an EEPROM cell.
    Its purpose is to mimic a typical byte of RAM, however its storage is the EEPROM.
    This class has an overhead of two bytes, similar to storing a pointer to an EEPROM cell.
***/

struct EERef{
private:
	void extEEPROMwrite(int EEPROM_addr, int addr, byte data)
	{
	  Wire.beginTransmission(EEPROM_addr);  //Start transmission to EEPROM
	  Wire.write(highByte(addr));           // send high byte of address
	  Wire.write(lowByte(addr));            // send low byte of address
	  Wire.write((byte) data);              // send data
	  Wire.endTransmission(true);           // stop transmitting
	  delay(6);                             // wait for a successful write
	}

	byte extEEPROMread(int EEPROM_addr, int addr) const
	{
	  Wire.beginTransmission(EEPROM_addr);           //Start transmission to EEPROM
	  Wire.write(highByte(addr));                    // send high byte of address
	  Wire.write(lowByte(addr));                     // send low byte of address
	  Wire.endTransmission(true);                    // stop transmitting
	  Wire.requestFrom(EEPROM_addr, 0x01, true);     // request 1 byte form the device attached to EEPROM_addr
	  byte data_out = 64;
	  // read that byte
	  while(Wire.available() == 0) {}                // wait for data
	  data_out = Wire.read();                        //read single byte
	  return data_out;
	}

public:
    EERef( const int index )
        : index( index )                 {}

    //Access/read members.
    uint8_t operator*() const            { return extEEPROMread( DIGIX_EEPROM_ADDR, index ); }
    operator const uint8_t() const       { return **this; }

    //Assignment/write members.
    EERef &operator=( const EERef &ref ) { return *this = *ref; }
    EERef &operator=( uint8_t in )       { return extEEPROMwrite( DIGIX_EEPROM_ADDR, index, in ), *this;  }
    EERef &operator +=( uint8_t in )     { return *this = **this + in; }
    EERef &operator -=( uint8_t in )     { return *this = **this - in; }
    EERef &operator *=( uint8_t in )     { return *this = **this * in; }
    EERef &operator /=( uint8_t in )     { return *this = **this / in; }
    EERef &operator ^=( uint8_t in )     { return *this = **this ^ in; }
    EERef &operator %=( uint8_t in )     { return *this = **this % in; }
    EERef &operator &=( uint8_t in )     { return *this = **this & in; }
    EERef &operator |=( uint8_t in )     { return *this = **this | in; }
    EERef &operator <<=( uint8_t in )    { return *this = **this << in; }
    EERef &operator >>=( uint8_t in )    { return *this = **this >> in; }

    EERef &update( uint8_t in )          { return  in != *this ? *this = in : *this; }

    /** Prefix increment/decrement **/
    EERef& operator++()                  { return *this += 1; }
    EERef& operator--()                  { return *this -= 1; }

    /** Postfix increment/decrement **/
    uint8_t operator++ (int){
        uint8_t ret = **this;
        return ++(*this), ret;
    }

    uint8_t operator-- (int){
        uint8_t ret = **this;
        return --(*this), ret;
    }

    int index; //Index of current EEPROM cell.
};

/***
    EEPtr class.

    This object is a bidirectional pointer to EEPROM cells represented by EERef objects.
    Just like a normal pointer type, this can be dereferenced and repositioned using
    increment/decrement operators.
***/

struct EEPtr{

    EEPtr( const int index )
        : index( index )                {}

    operator const int() const          { return index; }
    EEPtr &operator=( int in )          { return index = in, *this; }

    //Iterator functionality.
    bool operator!=( const EEPtr &ptr ) { return index != ptr.index; }
    EERef operator*()                   { return index; }

    /** Prefix & Postfix increment/decrement **/
    EEPtr& operator++()                 { return ++index, *this; }
    EEPtr& operator--()                 { return --index, *this; }
    EEPtr operator++ (int)              { return index++; }
    EEPtr operator-- (int)              { return index--; }

    int index; //Index of current EEPROM cell.
};

/***
    EEPROMClass class.

    This object represents the entire EEPROM space.
    It wraps the functionality of EEPtr and EERef into a basic interface.
    This class is also 100% backwards compatible with earlier Arduino core releases.
***/

struct EEPROMClass{
	boolean inited;

	// c'tor
	EEPROMClass ()                       { inited = false; }

	// Check if the i2c library was initialized, and do it if not.
	// FIXME: What happens if the sketch has already called Wire.begin() on its
	// own?
	void checkInited ()                  { if (!inited) { Wire.begin(); inited = true; } }

    //Basic user access methods.
    EERef operator[]( const int idx )    { return idx; }
    uint8_t read( int idx )              { checkInited (); return EERef( idx ); }
    void write( int idx, uint8_t val )   { checkInited (); (EERef( idx )) = val; }
    void update( int idx, uint8_t val )  { checkInited (); EERef( idx ).update( val ); }

    //STL and C++11 iteration capability.
    EEPtr begin()                        { return 0x00; }
    EEPtr end()                          { return length(); } //Standards requires this to be the item after the last valid entry. The returned pointer is invalid.
    uint16_t length()                    { return E2END + 1; }

    //Functionality to 'get' and 'put' objects to and from EEPROM.
    template< typename T > T &get( int idx, T &t ){
		checkInited ();

        EEPtr e = idx;
        uint8_t *ptr = (uint8_t*) &t;
        for( int count = sizeof(T) ; count ; --count, ++e )  *ptr++ = *e;
        return t;
    }

    template< typename T > const T &put( int idx, const T &t ){
		checkInited ();

        EEPtr e = idx;
        const uint8_t *ptr = (const uint8_t*) &t;
        for( int count = sizeof(T) ; count ; --count, ++e )  (*e).update( *ptr++ );
        return t;
    }
};

static EEPROMClass EEPROM;
#endif
