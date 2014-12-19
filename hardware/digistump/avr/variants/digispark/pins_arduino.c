/*
  pins_arduino.c - pin definitions for the Arduino board
  Part of Arduino / Wiring Lite

  Copyright (c) 2005 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA

  $Id: pins_arduino.c 565 2009-03-25 10:50:00Z dmellis $

  Modified 28-08-2009 for attiny84 R.Wiersma
  Modified 09-10-2009 for attiny45 A.Saporetti
  Modified for Atmel ATTiny2313 mcu by René Bohne

  Corrected 17-05-2010 for ATtiny84 B.Cook ...

    The default analog_reference leaves chip pin 13 (digital pin 10; PA0) 
    unconnected.  So the pin can be set to a non-floating state and so the 
    pin can be used as another digital pin, support for digital pin 10 was 
    added.
*/

#include <avr/io.h>
#include "pins_arduino.h"
#include "wiring_private.h"


#if defined( __AVR_ATtinyX313__ )

// On the Arduino board, digital pins are also used
// for the analog output (software PWM).  Analog input
// pins are a separate set.

// ATMEL ATTINY2313
//
//                   +-\/-+
//      (D 17) PA2  1|    |29  VCC
// RX   (D  0) PD0  2|    |19  PB7 (D  16)
// TX   (D  1) PD1  3|    |18  PB6 (D  15)
//      (D  2) PA1  4|    |17  PB5 (D  14)
//      (D  3) PA0  5|    |16  PB4 (D  13)*
// INT0 (D  4) PD2  6|    |15  PB3 (D  12)*
// INT1 (D  5) PD3  7|    |14  PB2 (D  11)*
//      (D  6) PD4  8|    |13  PB1 (D  10)
//     *(D  7) PD5  9|    |12  PB0 (D  9)
//             GND 10|    |11  PD6 (D  8)
//                   +----+
//
// * indicates PWM port

// these arrays map port names (e.g. port B) to the
// appropriate addresses for various functions (e.g. reading
// and writing)
const uint8_t PROGMEM port_to_mode_PGM[] = 
{
	NOT_A_PORT,
	&DDRA,
	&DDRB,
	NOT_A_PORT,
	&DDRD,
};

const uint8_t PROGMEM port_to_output_PGM[] = 
{
	NOT_A_PORT,
	&PORTA,
	&PORTB,
	NOT_A_PORT,
	&PORTD,
};

const uint8_t PROGMEM port_to_input_PGM[] = 
{
	NOT_A_PORT,
	&PINA,
	&PINB,
	NOT_A_PORT,
	&PIND,
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] = 
{
	PORT_D_ID, /* 0 */
	PORT_D_ID,
	PORT_A_ID,
	PORT_A_ID,
	PORT_D_ID,
	PORT_D_ID,
	PORT_D_ID,
	PORT_D_ID,
	PORT_D_ID, /* 8 */
	PORT_B_ID,
	PORT_B_ID,
	PORT_B_ID,
	PORT_B_ID,
	PORT_B_ID,
	PORT_B_ID, /* 14 */
	PORT_B_ID,
	PORT_B_ID,
  PORT_A_ID,
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = 
{
	_BV(0), /* 0 */
	_BV(1),
	_BV(1),
	_BV(0),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
	_BV(6), /* 8 */
	_BV(0),
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5), /* 14 */
	_BV(6),
	_BV(7),
  _BV(2),
};

const uint8_t PROGMEM digital_pin_to_timer_PGM[] = 
{
	NOT_ON_TIMER, 
	NOT_ON_TIMER,
	NOT_ON_TIMER,
  NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	TIMER0B,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	TIMER0A,
	TIMER1A,
	TIMER1B,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
};

#endif


#if defined( __AVR_ATtinyX4__ )

// ATMEL ATTINY84 / ARDUINO
//
//                           +-\/-+
//                     VCC  1|    |14  GND
//             (D  0)  PB0  2|    |13  AREF (D 10)
//             (D  1)  PB1  3|    |12  PA1  (D  9) 
//                     PB3  4|    |11  PA2  (D  8) 
//  PWM  INT0  (D  2)  PB2  5|    |10  PA3  (D  7) 
//  PWM        (D  3)  PA7  6|    |9   PA4  (D  6) 
//  PWM        (D  4)  PA6  7|    |8   PA5  (D  5)        PWM
//                           +----+

// these arrays map port names (e.g. port B) to the
// appropriate addresses for various functions (e.g. reading
// and writing)
const uint8_t PROGMEM port_to_mode_PGM[] = 
{
  NOT_A_PORT,
  &DDRA,
  &DDRB,
};

const uint8_t PROGMEM port_to_output_PGM[] = 
{
  NOT_A_PORT,
  &PORTA,
  &PORTB,
};

const uint8_t PROGMEM port_to_input_PGM[] = 
{
  NOT_A_PORT,
  &PINA,
  &PINB,
};

const uint8_t PROGMEM port_to_pcmask_PGM[] = 
{
  NOT_A_PORT,
  &PCMSK0,
  &PCMSK1,
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] = 
{
  PORT_B_ID, /* 0 */
  PORT_B_ID,
  PORT_B_ID,
  PORT_A_ID,
  PORT_A_ID,
  PORT_A_ID,
  PORT_A_ID,
  PORT_A_ID,
  PORT_A_ID, /* 8 */
  PORT_A_ID,
  PORT_A_ID,
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = 
{
  _BV(0), /* 0, port B */
  _BV(1),
  _BV(2),
  _BV(7), /* 3 port B */
  _BV(6),
  _BV(5),
  _BV(4),
  _BV(3),
  _BV(2), 
  _BV(1),
  _BV(0),
};

const uint8_t PROGMEM digital_pin_to_timer_PGM[] = 
{
  NOT_ON_TIMER,
  NOT_ON_TIMER,
  TIMER0A, /* OC0A */
  TIMER0B, /* OC0B */
  TIMER1A, /* OC1A */
  TIMER1B, /* OC1B */
  NOT_ON_TIMER,
  NOT_ON_TIMER,
  NOT_ON_TIMER, 
  NOT_ON_TIMER,
  NOT_ON_TIMER,
  NOT_ON_TIMER,
};

#endif


#if defined( __AVR_ATtinyX5__ )

// ATMEL ATTINY45 / ARDUINO
//
//                           +-\/-+
//  Ain0       (D  5)  PB5  1|    |8   VCC
//  Ain3       (D  3)  PB3  2|    |7   PB2  (D  2)  INT0  Ain1
//  Ain2       (D  4)  PB4  3|    |6   PB1  (D  1)        pwm1
//                     GND  4|    |5   PB0  (D  0)        pwm0
//                           +----+

// these arrays map port names (e.g. port B) to the
// appropriate addresses for various functions (e.g. reading
// and writing) tiny45 only port B 
const uint8_t PROGMEM port_to_mode_PGM[] = 
{
	NOT_A_PORT,
	&DDRB,
};

const uint8_t PROGMEM port_to_output_PGM[] = 
{
	NOT_A_PORT,
	&PORTB,
};

const uint8_t PROGMEM port_to_input_PGM[] = 
{
	NOT_A_PIN,
	&PINB,
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] = 
{
	PORT_B_ID, /* 0 */
	PORT_B_ID,
	PORT_B_ID,
	PORT_B_ID,
	PORT_B_ID, 
	PORT_B_ID, /* 5 */

};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = 
{
	_BV(0), /* 0, port B */
	_BV(1),
	_BV(2),
	_BV(3), /* 3 port B */
	_BV(4),
	_BV(5),

};

const uint8_t PROGMEM digital_pin_to_timer_PGM[] = 
{
	TIMER0A, /* OC0A */
	TIMER1A, /* OC1A? */
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
};

#endif
