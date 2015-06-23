/*
  pins_arduino.h - Pin definition functions for Arduino
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2007 David A. Mellis

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

  $Id: wiring.h 249 2007-02-03 16:52:51Z mellis $

  Modified 28-08-2009 for attiny84 R.Wiersma
  Modified 14-10-2009 for attiny45 Saposoft
*/

#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <avr/pgmspace.h>

#include "core_build_options.h"

#if defined( __AVR_ATtinyX313__ )
#define PORT_A_ID 1
#define PORT_B_ID 2
#define PORT_D_ID 4
#endif

#if defined( __AVR_ATtinyX4__ )
#define PORT_A_ID 1
#define PORT_B_ID 2
#endif

#if defined( __AVR_ATtinyX5__ )
#define PORT_B_ID 1
#endif

#define NOT_A_PIN 0
#define NOT_A_PORT 0

#define NOT_ON_TIMER 0
#define TIMER0A 1
#define TIMER0B 2
#define TIMER1A 3
#define TIMER1B 4

//changed it to uint16_t to uint8_t
extern const uint8_t PROGMEM port_to_mode_PGM[];
extern const uint8_t PROGMEM port_to_input_PGM[];
extern const uint8_t PROGMEM port_to_output_PGM[];
extern const uint8_t PROGMEM port_to_pcmask_PGM[];

extern const uint8_t PROGMEM digital_pin_to_port_PGM[];
// extern const uint8_t PROGMEM digital_pin_to_bit_PGM[];
extern const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[];
extern const uint8_t PROGMEM digital_pin_to_timer_PGM[];

// Get the bit location within the hardware port of the given virtual pin.
// This comes from the pins_*.c file for the active board configuration.
// 
// These perform slightly better as macros compared to inline functions
//
#define digitalPinToPort(P) ( pgm_read_byte( digital_pin_to_port_PGM + (P) ) )
#define digitalPinToBitMask(P) ( pgm_read_byte( digital_pin_to_bit_mask_PGM + (P) ) )
#define digitalPinToTimer(P) ( pgm_read_byte( digital_pin_to_timer_PGM + (P) ) )
#define analogInPinToBit(P) (P)
// in the following lines modified pgm_read_word in pgm_read_byte, word doesn't work on attiny45
#define portOutputRegister(P) ( (volatile uint8_t *)( pgm_read_byte( port_to_output_PGM + (P))) )
#define portInputRegister(P) ( (volatile uint8_t *)( pgm_read_byte( port_to_input_PGM + (P))) )
#define portModeRegister(P) ( (volatile uint8_t *)( pgm_read_byte( port_to_mode_PGM + (P))) )
#define portPcMaskRegister(P) ( (volatile uint8_t *)( pgm_read_byte( port_to_pcmask_PGM + (P))) )

#if defined(__AVR_ATtinyX5__)
#define digitalPinToPCICR(p)    (((p) >= 0 && (p) <= 5) ? (&GIMSK) : ((uint8_t *)NULL))
#define digitalPinToPCICRbit(p) (PCIE)
#define digitalPinToPCMSK(p)    (((p) >= 0 && (p) <= 5) ? (&PCMSK) : ((uint8_t *)NULL))
#define digitalPinToPCMSKbit(p) (p)
#endif

#if defined(__AVR_ATtinyX4__)
#define digitalPinToPCICR(p)    (((p) >= 0 && (p) <= 10) ? (&GIMSK) : ((uint8_t *)NULL))
#define digitalPinToPCICRbit(p) (((p) <= 2) ? PCIE1 : PCIE0)
#define digitalPinToPCMSK(p)    (((p) <= 2) ? (&PCMSK1) : (((p) <= 10) ? (&PCMSK0) : ((uint8_t *)NULL)))
#define digitalPinToPCMSKbit(p) (((p) <= 2) ? (p) : (10 - (p)))
#endif

#if defined(__AVR_ATtiny4313__)
#define digitalPinToPCX(p,s1,s2,s3,s4,s5) \
    (((p) >= 0) \
        ? (((p) <=  1) ? (s1)  /*  0 -  1  ==>  D0 - D1 */  \
        : (((p) <=  3) ? (s2)  /*  2 -  3  ==>  A1 - A0 */  \
        : (((p) <=  8) ? (s3)  /*  4 -  8  ==>  D2 - D6 */  \
        : (((p) <= 16) ? (s4)  /*  9 - 16  ==>  B0 - B7 */  \
        : (s5))))) \
        : (s5))
//                                                   s1 D     s2 A     s3 D     s4 B
#define digitalPinToPCICR(p)    digitalPinToPCX( p, &GIMSK,  &GIMSK,  &GIMSK,  &GIMSK,  NULL )
#define digitalPinToPCICRbit(p) digitalPinToPCX( p, PCIE2,   PCIE1,   PCIE2,   PCIE0,   0    )
#define digitalPinToPCMSK(p)    digitalPinToPCX( p, &PCMSK2, &PCMSK1, &PCMSK2, &PCMSK0, NULL )
#define digitalPinToPCMSKbit(p) digitalPinToPCX( p, p,       3-p,     p-2,     p-9,     0    )
#endif

#endif
