/*==============================================================================

  core_adc.h - Veneer for the analog-to-digital converter.

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

#ifndef core_adc_h
#define core_adc_h

#include <avr/io.h>
#include <binary.h>

#include "core_build_options.h"
#include "core_macros.h"


/*=============================================================================
  Some common things
=============================================================================*/

#if defined( __AVR_ATtinyX4__ ) || defined( __AVR_ATtinyX5__ )

/*
  From the '84 and '85 datasheets... By default, the successive approximation 
  circuitry requires an input clock frequency between 50 kHz and 200 kHz to 
  get maximum resolution.
*/
#if F_CPU == 16000000
  // 16 MHz / 128 = 125 KHz
  #define ADC_ARDUINO_PRESCALER   ADC_Prescaler_Value_128
#elif F_CPU == 16500000
  // 8 MHz / 64 = 125 KHz
  #define ADC_ARDUINO_PRESCALER   ADC_Prescaler_Value_128  
#elif F_CPU == 8000000
  // 8 MHz / 64 = 125 KHz
  #define ADC_ARDUINO_PRESCALER   ADC_Prescaler_Value_64
#elif F_CPU == 1000000
  // 1 MHz / 8 = 125 KHz
  #define ADC_ARDUINO_PRESCALER   ADC_Prescaler_Value_8
#else
  #error Add an entry for the selected processor speed.
#endif

typedef enum
{
  ADC_Prescaler_Value_2      = B001,
  ADC_Prescaler_Value_4      = B010,
  ADC_Prescaler_Value_8      = B011,
  ADC_Prescaler_Value_16     = B100,
  ADC_Prescaler_Value_32     = B101,
  ADC_Prescaler_Value_64     = B110,
  ADC_Prescaler_Value_128    = B111,
  ADC_Prescaler_Index_1      = B001,
  ADC_Prescaler_Index_2      = B010,
  ADC_Prescaler_Index_3      = B011,
  ADC_Prescaler_Index_4      = B100,
  ADC_Prescaler_Index_5      = B101,
  ADC_Prescaler_Index_6      = B110,
  ADC_Prescaler_Index_7      = B111
}
adc_ps_t;

__attribute__((always_inline)) static inline void ADC_PrescalerSelect( adc_ps_t ps )
{
  ADCSRA = (ADCSRA & ~MASK3(ADPS2,ADPS1,ADPS0)) | (ps << ADPS0);
}

__attribute__((always_inline)) static inline void ADC_Enable( void )
{
  ADCSRA |= MASK1( ADEN );
}

#endif


/*=============================================================================
  Veneer for the ATtiny84 ADC
=============================================================================*/

#if defined( __AVR_ATtinyX4__ )

typedef enum
{
  ADC_Reference_VCC                             = B00,
  ADC_Reference_External                        = B01,
  ADC_Reference_Internal_1p1                    = B10,
  ADC_Reference_Reserved_1                      = B11
}
adc_vr_t;

__attribute__((always_inline)) static inline void ADC_SetVoltageReference( adc_vr_t vr )
{
  ADMUX = (ADMUX & ~MASK2(REFS1,REFS0)) | (((vr & B11) >> 0) << REFS0);
}

typedef enum
{
  ADC_Input_ADC0            = B000000,
  ADC_Input_ADC1            = B000001,
  ADC_Input_ADC2            = B000010,
  ADC_Input_ADC3            = B000011,
  ADC_Input_ADC4            = B000100,
  ADC_Input_ADC5            = B000101,
  ADC_Input_ADC6            = B000110,
  ADC_Input_ADC7            = B000111,

  ADC_Input_GND             = B100000,  // 0V (AGND)
  ADC_Input_1p1             = B100001,  // 1.1V (I Ref)
  ADC_Input_ADC8            = B100010,  // For temperature sensor.

  ADC_Input_Pos0_Neg0_20x   = B100011,  // For offset calibration, only.
  ADC_Input_Pos0_Neg1_1x    = B001000,
  ADC_Input_Pos0_Neg1_20x   = B001001,
  ADC_Input_Pos0_Neg3_1x    = B001010,
  ADC_Input_Pos0_Neg3_20x   = B001011,
  ADC_Input_Pos1_Neg0_1x    = B101000,
  ADC_Input_Pos1_Neg0_20x   = B101001,
  ADC_Input_Pos1_Neg2_1x    = B001100,
  ADC_Input_Pos1_Neg2_20x   = B001101,
  ADC_Input_Pos1_Neg3_1x    = B001110,
  ADC_Input_Pos1_Neg3_20x   = B001111,
  ADC_Input_Pos2_Neg1_1x    = B101100,
  ADC_Input_Pos2_Neg1_20x   = B101101,
  ADC_Input_Pos2_Neg3_1x    = B010000,
  ADC_Input_Pos2_Neg3_20x   = B010001,
  ADC_Input_Pos3_Neg0_1x    = B101010,
  ADC_Input_Pos3_Neg0_20x   = B101011,
  ADC_Input_Pos3_Neg1_1x    = B101110,
  ADC_Input_Pos3_Neg1_20x   = B101111,
  ADC_Input_Pos3_Neg2_1x    = B110000,
  ADC_Input_Pos3_Neg2_20x   = B110001,
  ADC_Input_Pos3_Neg3_1x    = B100100,  // For offset calibration, only.
  ADC_Input_Pos3_Neg3_20x   = B100101,  // For offset calibration, only.
  ADC_Input_Pos3_Neg4_1x    = B010010,
  ADC_Input_Pos3_Neg4_20x   = B010011,
  ADC_Input_Pos3_Neg5_1x    = B010100,
  ADC_Input_Pos3_Neg5_20x   = B010101,
  ADC_Input_Pos3_Neg6_1x    = B010110,
  ADC_Input_Pos3_Neg6_20x   = B010111,
  ADC_Input_Pos3_Neg7_1x    = B011000,
  ADC_Input_Pos3_Neg7_20x   = B011001,
  ADC_Input_Pos4_Neg3_1x    = B110010,
  ADC_Input_Pos4_Neg3_20x   = B110011,
  ADC_Input_Pos4_Neg5_1x    = B011010,
  ADC_Input_Pos4_Neg5_20x   = B011011,
  ADC_Input_Pos5_Neg3_1x    = B110100,
  ADC_Input_Pos5_Neg3_20x   = B110101,
  ADC_Input_Pos5_Neg4_1x    = B111010,
  ADC_Input_Pos5_Neg4_20x   = B111011,
  ADC_Input_Pos5_Neg6_1x    = B011100,
  ADC_Input_Pos5_Neg6_20x   = B011101,
  ADC_Input_Pos6_Neg3_1x    = B110110,
  ADC_Input_Pos6_Neg3_20x   = B110111,
  ADC_Input_Pos6_Neg5_1x    = B111100,
  ADC_Input_Pos6_Neg5_20x   = B111101,
  ADC_Input_Pos6_Neg7_1x    = B011110,
  ADC_Input_Pos6_Neg7_20x   = B011111,
  ADC_Input_Pos7_Neg3_1x    = B111000,
  ADC_Input_Pos7_Neg3_20x   = B111001,
  ADC_Input_Pos7_Neg6_1x    = B111110,
  ADC_Input_Pos7_Neg6_20x   = B111111,
  ADC_Input_Pos7_Neg7_1x    = B100110,  // For offset calibration, only.
  ADC_Input_Pos7_Neg7_20x   = B100111   // For offset calibration, only.
}
adc_ic_t;

__attribute__((always_inline)) static inline void ADC_SetInputChannel( adc_ic_t ic )
{
  ADMUX = (ADMUX & ~MASK6(MUX5,MUX4,MUX3,MUX2,MUX1,MUX0)) | (ic << MUX0);
}

__attribute__((always_inline)) static inline void ADC_StartConversion( void )
{
  ADCSRA |= MASK1( ADSC );
}

__attribute__((always_inline)) static inline uint8_t ADC_ConversionInProgress( void )
{
  return( (ADCSRA & (1<<ADSC)) != 0 );
}

__attribute__((always_inline)) static inline uint16_t ADC_GetDataRegister( void )
{
  return( ADC );
}

#endif


/*=============================================================================
  Veneer for the ATtiny85 ADC
=============================================================================*/

#if defined( __AVR_ATtinyX5__ )

typedef enum
{
  ADC_Reference_VCC                             = B000,
  ADC_Reference_External                        = B001,
  ADC_Reference_Internal_1p1                    = B010,
  ADC_Reference_Reserved_1                      = B011,
  ADC_Reference_Internal_2p56                   = B110,
  ADC_Reference_Internal_2p56_Bypass_Capacitor  = B111
}
adc_vr_t;

__attribute__((always_inline)) static inline void ADC_SetVoltageReference( adc_vr_t vr )
{
  ADMUX = (ADMUX & ~MASK3(REFS1,REFS0,REFS2)) 
      | (((vr & B011) >> 0) << REFS0) 
      | (((vr & B100) >> 2) << REFS2);
}

typedef enum
{
  ADC_Input_ADC0            = B0000,
  ADC_Input_ADC1            = B0001,
  ADC_Input_ADC2            = B0010,
  ADC_Input_ADC3            = B0011,

  ADC_Input_Pos2_Neg2_1x    = B0100,  // For offset calibration, only.
  ADC_Input_Pos2_Neg2_20x   = B0101,  // For offset calibration, only.
  ADC_Input_Pos2_Neg3_1x    = B0110,
  ADC_Input_Pos2_Neg3_20x   = B0111,
  ADC_Input_Pos0_Neg0_1x    = B1000,
  ADC_Input_Pos0_Neg0_20x   = B1001,
  ADC_Input_Pos0_Neg1_1x    = B1010,
  ADC_Input_Pos0_Neg1_20x   = B1011,

  ADC_Input_VBG             = B1100,
  ADC_Input_GND             = B1101,
  ADC_Input_NA              = B1110,
  ADC_Input_ADC4            = B1111   // For temperature sensor.
}
adc_ic_t;

__attribute__((always_inline)) static inline void ADC_SetInputChannel( adc_ic_t ic )
{
  ADMUX = (ADMUX & ~MASK4(MUX3,MUX2,MUX1,MUX0)) | (ic << MUX0);
}

__attribute__((always_inline)) static inline void ADC_StartConversion( void )
{
  ADCSRA |= MASK1( ADSC );
}

__attribute__((always_inline)) static inline uint8_t ADC_ConversionInProgress( void )
{
  return( (ADCSRA & (1<<ADSC)) != 0 );
}

__attribute__((always_inline)) static inline uint16_t ADC_GetDataRegister( void )
{
  return( ADC );
}

#endif


/*=============================================================================
  Veneer for the (non-existant) ATtinyX313 ADC
=============================================================================*/

#if defined( __AVR_ATtinyX313__ )

typedef enum
{
  ADC_No_Reference  = 0
}
adc_vr_t;

__attribute__((always_inline)) static inline void ADC_SetVoltageReference( adc_vr_t vr )
{
}

typedef enum
{
  ADC_No_Input = 0
}
adc_ic_t;

__attribute__((always_inline)) static inline void ADC_SetInputChannel( adc_ic_t ic )
{
}

__attribute__((always_inline)) static inline void ADC_StartConversion( void )
{
}

__attribute__((always_inline)) static inline uint8_t ADC_ConversionInProgress( void )
{
  return( 0 );
}

__attribute__((always_inline)) static inline uint16_t ADC_GetDataRegister( void )
{
  return( 0 );
}

#endif


#endif
