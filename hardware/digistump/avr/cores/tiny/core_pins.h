/*==============================================================================

  core_pins.h - Pin definitions.

  Copyright 2010 Rowdy Dog Software.

  This file is part of Arduino-Tiny.

  Arduino-Tiny is free software: you can redistribute it and/or modify it 
  under the terms of the GNU Lesser General Public License as published by 
  the Free Software Foundation, either version 3 of the License, or (at your
  option) any later version.

  Arduino-Tiny is distributed in the hope that it will be useful, but 
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
  License for more details.

  You should have received a copy of the GNU Lesser General Public License 
  along with Arduino-Tiny.  If not, see <http://www.gnu.org/licenses/>.

==============================================================================*/

#ifndef core_pins_h
#define core_pins_h

#include "core_build_options.h"


/*=============================================================================
  Pin definitions for the ATtinyX313
=============================================================================*/

#if defined( __AVR_ATtinyX313__ )

#define PIN_D0  ( 0)
#define PIN_D1  ( 1)
#define PIN_A1  ( 2)
#define PIN_A0  ( 3)
#define PIN_D2  ( 4)
#define PIN_D3  ( 5)
#define PIN_D4  ( 6)
#define PIN_D5  ( 7)
#define PIN_D6  ( 8)
#define PIN_B0  ( 9)
#define PIN_B1  (10)
#define PIN_B2  (11)
#define PIN_B3  (12)
#define PIN_B4  (13)
#define PIN_B5  (14)
#define PIN_B6  (15)
#define PIN_B7  (16)
#define PIN_A2  (17)  /* RESET */

#define CORE_DIGITAL_FIRST    (0)
#define CORE_DIGITAL_LAST     (17)
#define CORE_DIGITAL_COUNT    (CORE_DIGITAL_LAST-CORE_DIGITAL_FIRST+1)
#define CORE_RESET_INCLUDED   (1)

#define CORE_ANALOG_COUNT     (0)

#define CORE_INT0_PIN  PIN_D2
#define CORE_INT1_PIN  PIN_D3

#define CORE_OC0A_PIN  PIN_B2
#define CORE_OC0B_PIN  PIN_D5
#define CORE_OC1A_PIN  PIN_B3
#define CORE_OC1B_PIN  PIN_B4

#define CORE_PWM0_PIN       CORE_OC0A_PIN
#define CORE_PWM0_TIMER     0
#define CORE_PWM0_CHANNEL   A

#define CORE_PWM1_PIN       CORE_OC0B_PIN
#define CORE_PWM1_TIMER     0
#define CORE_PWM1_CHANNEL   B

#define CORE_PWM2_PIN       CORE_OC1A_PIN
#define CORE_PWM2_TIMER     1
#define CORE_PWM2_CHANNEL   A

#define CORE_PWM3_PIN       CORE_OC1B_PIN
#define CORE_PWM3_TIMER     1
#define CORE_PWM3_CHANNEL   B

#define CORE_PWM_COUNT      (4)

#endif


/*=============================================================================
  Pin definitions for the ATtiny84
=============================================================================*/

#if defined( __AVR_ATtinyX4__ )

#define PIN_A0  (10)
#define PIN_A1  ( 9)
#define PIN_A2  ( 8)
#define PIN_A3  ( 7)
#define PIN_A4  ( 6)
#define PIN_A5  ( 5)
#define PIN_A6  ( 4)
#define PIN_A7  ( 3)
#define PIN_B0  ( 0)
#define PIN_B1  ( 1)
#define PIN_B2  ( 2)
#define PIN_B3  (11)  /* RESET */

#define CORE_DIGITAL_FIRST    (0)
#define CORE_DIGITAL_LAST     (11)
#define CORE_DIGITAL_COUNT    (CORE_DIGITAL_LAST-CORE_DIGITAL_FIRST+1)
#define CORE_RESET_INCLUDED   (1)

#define CORE_ANALOG_FIRST     (CORE_DIGITAL_LAST+1)
#define CORE_ANALOG_COUNT     (8)
#define CORE_ANALOG_LAST      (CORE_ANALOG_FIRST+CORE_ANALOG_COUNT-1)

#define CORE_INT0_PIN  PIN_B2

#define CORE_OC0A_PIN  PIN_B2
#define CORE_OC0B_PIN  PIN_A7
#define CORE_OC1A_PIN  PIN_A6
#define CORE_OC1B_PIN  PIN_A5

#define CORE_PWM0_PIN       CORE_OC0A_PIN
#define CORE_PWM0_TIMER     0
#define CORE_PWM0_CHANNEL   A

#define CORE_PWM1_PIN       CORE_OC0B_PIN
#define CORE_PWM1_TIMER     0
#define CORE_PWM1_CHANNEL   B

#define CORE_PWM2_PIN       CORE_OC1A_PIN
#define CORE_PWM2_TIMER     1
#define CORE_PWM2_CHANNEL   A

#define CORE_PWM3_PIN       CORE_OC1B_PIN
#define CORE_PWM3_TIMER     1
#define CORE_PWM3_CHANNEL   B

#define CORE_PWM_COUNT      (4)

#endif


/*=============================================================================
  Pin definitions for the ATtiny85
=============================================================================*/

#if defined( __AVR_ATtinyX5__ )

#define PIN_B0  ( 0)
#define PIN_B1  ( 1)
#define PIN_B2  ( 2)
#define PIN_B3  ( 3)
#define PIN_B4  ( 4)
#define PIN_B5  ( 5)  /* RESET */

#define CORE_DIGITAL_FIRST    (0)
#define CORE_DIGITAL_LAST     (5)
#define CORE_DIGITAL_COUNT    (CORE_DIGITAL_LAST-CORE_DIGITAL_FIRST+1)
#define CORE_RESET_INCLUDED   (1)

#define CORE_ANALOG_FIRST     (CORE_DIGITAL_LAST+1)
#define CORE_ANALOG_COUNT     (4)
#define CORE_ANALOG_LAST      (CORE_ANALOG_FIRST+CORE_ANALOG_COUNT-1)

#define CORE_INT0_PIN  PIN_B2

#define CORE_OC0A_PIN  PIN_B0
#define CORE_OC0B_PIN  PIN_B1
#define CORE_OC1A_PIN  PIN_B1
#define CORE_OC1B_PIN  PIN_B4

/* Note: By default, CORE_OC1A_PIN is not used for PWM.  It overlaps with 
CORE_OC0B_PIN.  CORE_OC0B_PIN was used because it supports phase-correct PWM.
There is a build option in "core_build_options.h" to determine which channel 
to use */

#define CORE_PWM0_PIN       CORE_OC0A_PIN
#define CORE_PWM0_TIMER     0
#define CORE_PWM0_CHANNEL   A

#if FAVOR_PHASE_CORRECT_PWM
#define CORE_PWM1_PIN       CORE_OC0B_PIN
#define CORE_PWM1_TIMER     0
#define CORE_PWM1_CHANNEL   B
#else
#define CORE_PWM1_PIN       CORE_OC1A_PIN
#define CORE_PWM1_TIMER     1
#define CORE_PWM1_CHANNEL   A
#endif

#define CORE_PWM2_PIN       CORE_OC1B_PIN
#define CORE_PWM2_TIMER     1
#define CORE_PWM2_CHANNEL   B

#define CORE_PWM_COUNT      (3)

#endif


#endif
