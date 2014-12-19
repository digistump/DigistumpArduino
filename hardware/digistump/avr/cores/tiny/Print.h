/*
  Print.h - Base class that provides print() and println()
  Copyright (c) 2008 David A. Mellis.  All right reserved.

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

  Modified 20-11-2010 by B.Cook ...

    http://arduiniana.org/libraries/flash/
    Printable support thanks to Mikal Hart
*/

#ifndef Print_h
#define Print_h

#include <inttypes.h>
#include <stdio.h> // for size_t
#include <avr/pgmspace.h>

#include "WString.h"

#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2
#define BYTE 0

#define ARDUINO_CORE_PRINTABLE_SUPPORT

class Print;

/* Printable...*/

class _Printable
{
public:
  virtual void print(Print &stream) const = 0;
};

/* ...Printable */
    
typedef struct
{
  char c;
}
fstr_t;

/* rmv: Use the macro below in preparation for the next Arduino release.
# define FSTR(s) ((fstr_t*)PSTR(s))
*/
# define F(s) ((fstr_t*)PSTR(s))

class Print
{
  private:
    void printNumber(unsigned long, uint8_t);
    void printFloat(double, uint8_t);
  protected:
    void setWriteError(int err = 1) { /*write_error = err;*/ }
  public:
    virtual size_t write(uint8_t) = 0;
    virtual void write(const char *str);
    virtual void write(const uint8_t *buffer, size_t size);
    
    void print(const String &);
    void print(const char[]);
    void print(char, int = BYTE);
    void print(unsigned char, int = BYTE);
    void print(int, int = DEC);
    void print(unsigned int, int = DEC);
    void print(long, int = DEC);
    void print(unsigned long, int = DEC);
    void print(double, int = 2);
    int  print( fstr_t* );

    void println(const String &s);
    void println(const char[]);
    void println(char, int = BYTE);
    void println(unsigned char, int = BYTE);
    void println(int, int = DEC);
    void println(unsigned int, int = DEC);
    void println(long, int = DEC);
    void println(unsigned long, int = DEC);
    void println(double, int = 2);
    int  println( fstr_t* );
    int  println(void);
  public:
    /* Printable...*/
    void println(const _Printable &obj)
    { obj.print(*this); println(); }
    void print(const _Printable &obj)
    { obj.print(*this); };
    /* ...Printable */
};

#endif
