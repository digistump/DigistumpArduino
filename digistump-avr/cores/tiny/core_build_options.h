/*==============================================================================

  core_build_options.h - Various options for mapping functionality to hardware.

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

#ifndef core_build_options_h
#define core_build_options_h


/*=============================================================================
  Low power / smaller code options
=============================================================================*/

#define INITIALIZE_ANALOG_TO_DIGITAL_CONVERTER    1
#define INITIALIZE_SECONDARY_TIMERS               1


/*=============================================================================
  Build options for the ATtinyX313 processor
=============================================================================*/

#if defined( __AVR_ATtiny2313__ ) || defined( __AVR_ATtiny4313__ )
#define __AVR_ATtinyX313__
#endif

#if defined( __AVR_ATtinyX313__ )

/*
  The old standby ... millis on Timer 0.
*/
#define TIMER_TO_USE_FOR_MILLIS                   0

/*
  Tone goes on whichever timer was not used for millis.
*/
#if TIMER_TO_USE_FOR_MILLIS == 1
#define TIMER_TO_USE_FOR_TONE                     0
#else
#define TIMER_TO_USE_FOR_TONE                     1
#endif

#define HAVE_ADC                                  0

#define DEFAULT_TO_TINY_DEBUG_SERIAL              0

#endif


/*=============================================================================
  Build options for the ATtiny84 processor
=============================================================================*/

#if defined( __AVR_ATtiny24__ ) || defined( __AVR_ATtiny44__ ) || defined( __AVR_ATtiny84__ )
#define __AVR_ATtinyX4__
#endif

#if defined( __AVR_ATtinyX4__ )

/*
  The old standby ... millis on Timer 0.
*/
#define TIMER_TO_USE_FOR_MILLIS                   0

/*
  Tone goes on whichever timer was not used for millis.
*/
#if TIMER_TO_USE_FOR_MILLIS == 1
#define TIMER_TO_USE_FOR_TONE                     0
#else
#define TIMER_TO_USE_FOR_TONE                     1
#endif

#define HAVE_ADC                                  1

#define DEFAULT_TO_TINY_DEBUG_SERIAL              1

#endif


/*=============================================================================
  Build options for the ATtiny85 processor
=============================================================================*/

#if defined( __AVR_ATtiny25__ ) || defined( __AVR_ATtiny45__ ) || defined( __AVR_ATtiny85__ )
#define __AVR_ATtinyX5__
#endif

#if defined( __AVR_ATtinyX5__ )

/*
  For various reasons, Timer 1 is a better choice for the millis timer on the
  '85 processor.
*/
#define TIMER_TO_USE_FOR_MILLIS                   1

/*
  If the following is true (non-zero) there will be two phase-correct PWM 
  pins and one fast PWM pin.  If false there will be one phase-correct PWM 
  pin and two fast PWM pins.
*/
#define FAVOR_PHASE_CORRECT_PWM                   1

/*
  Tone goes on whichever timer was not used for millis.
*/
#if TIMER_TO_USE_FOR_MILLIS == 1
#define TIMER_TO_USE_FOR_TONE                     0
#else
#define TIMER_TO_USE_FOR_TONE                     1
#endif

#define HAVE_ADC                                  1

#define DEFAULT_TO_TINY_DEBUG_SERIAL              1

#endif


/*=============================================================================
  There doesn't seem to be many people using a bootloader so we'll assume 
  there isn't one.  If the following is true (non-zero), the timers are 
  reinitialized to their power-up state in init just in case the bootloader 
  left them in a bad way.
=============================================================================*/

#define HAVE_BOOTLOADER                           0


/*=============================================================================
  Allow the ADC to be optional for low-power applications
=============================================================================*/

#if ! defined( HAVE_ADC )
  #define HAVE_ADC   0
#endif

#if ! HAVE_ADC
  #undef INITIALIZE_ANALOG_TO_DIGITAL_CONVERTER
  #define INITIALIZE_ANALOG_TO_DIGITAL_CONVERTER  0
#else
  #if ! defined( INITIALIZE_ANALOG_TO_DIGITAL_CONVERTER )
    #define INITIALIZE_ANALOG_TO_DIGITAL_CONVERTER   1
  #endif
#endif


/*=============================================================================
  Allow the "secondary timers" to be optional for low-power applications
=============================================================================*/

#if ! defined( INITIALIZE_SECONDARY_TIMERS )
  #define INITIALIZE_SECONDARY_TIMERS               1
#endif


#endif
