/*
  wiring.c - Partial implementation of the Wiring API for the ATmega8.
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2005-2006 David A. Mellis

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

  $Id: wiring.c 970 2010-05-25 20:16:15Z dmellis $

  Modified 28-08-2009 for attiny84 R.Wiersma
  Modified 14-10-2009 for attiny45 Saposoft
  Modified 20-11-2010 - B.Cook - Rewritten to use the various Veneers.
*/

#include "core_build_options.h"
#include "core_adc.h"
#include "core_timers.h"
#include "wiring_private.h"
#include "ToneTimer.h"
#if F_CPU != 16500000L
  #include <avr/boot.h>
#endif

#define millistimer_(t)                           TIMER_PASTE_A( timer, TIMER_TO_USE_FOR_MILLIS, t )
#define MillisTimer_(f)                           TIMER_PASTE_A( Timer, TIMER_TO_USE_FOR_MILLIS, f )
#define MILLISTIMER_(c)                           TIMER_PASTE_A( TIMER, TIMER_TO_USE_FOR_MILLIS, c )

#define MillisTimer_SetToPowerup                  MillisTimer_(SetToPowerup)
#define MillisTimer_SetWaveformGenerationMode     MillisTimer_(SetWaveformGenerationMode)
#define MillisTimer_GetCount                      MillisTimer_(GetCount)
#define MillisTimer_IsOverflowSet                 MillisTimer_(IsOverflowSet)
#define MillisTimer_ClockSelect                   MillisTimer_(ClockSelect)
#define MillisTimer_EnableOverflowInterrupt       MillisTimer_(EnableOverflowInterrupt)
#define MILLISTIMER_OVF_vect                      MILLISTIMER_(OVF_vect)


#define MS_TIMER_TICK_EVERY_X_CYCLES  64 /* Shall be a within 1, 8, 64, 256 or 1024. (default = 64) If set to 1, HW PWM is around 64.5KHz@16.5MHz with Digispark */

#if F_CPU >= 3000000L
#if !defined(MS_TIMER_TICK_EVERY_X_CYCLES)
  #define MillisTimer_Prescale_Index  MillisTimer_(Prescale_Value_64)
  #define MillisTimer_Prescale_Value  (64)
  #define ToneTimer_Prescale_Index    ToneTimer_(Prescale_Value_64)
  #define ToneTimer_Prescale_Value    (64)
#else
  #define Prescaler_Value(Val)        PRESCALER_VALUE(Val)
  #define PRESCALER_VALUE(Val)        Prescale_Value_##Val
  #define MillisTimer_Prescale_Index  MillisTimer_(Prescaler_Value(MS_TIMER_TICK_EVERY_X_CYCLES))
  #define MillisTimer_Prescale_Value  (MS_TIMER_TICK_EVERY_X_CYCLES)
  #define ToneTimer_Prescale_Index    ToneTimer_(Prescaler_Value(MS_TIMER_TICK_EVERY_X_CYCLES))
  #define ToneTimer_Prescale_Value    (MS_TIMER_TICK_EVERY_X_CYCLES)
#endif
#else
  #define MillisTimer_Prescale_Index  MillisTimer_(Prescale_Value_8)
  #define MillisTimer_Prescale_Value  (8)
  #define ToneTimer_Prescale_Index    ToneTimer_(Prescale_Value_8)
  #define ToneTimer_Prescale_Value    (8)
#endif

// the prescaler is set so that the millis timer ticks every MillisTimer_Prescale_Value (64) clock cycles, and the
// the overflow handler is called every 256 ticks.
#define MICROSECONDS_PER_MILLIS_OVERFLOW (clockCyclesToMicroseconds(MillisTimer_Prescale_Value * 256))

// the whole number of milliseconds per millis timer overflow
#define MILLIS_INC (MICROSECONDS_PER_MILLIS_OVERFLOW / 1000)

// the fractional number of milliseconds per millis timer overflow. we shift right
// by three to fit these numbers into a byte. (for the clock speeds we care
// about - 8 and 16 MHz - this doesn't lose precision.)
#define FRACT_INC ((MICROSECONDS_PER_MILLIS_OVERFLOW % 1000) >> 3)
#define FRACT_MAX (1000 >> 3)

volatile unsigned long millis_timer_overflow_count = 0;
volatile unsigned long millis_timer_millis = 0;
static unsigned char millis_timer_fract = 0;

// bluebie changed isr to noblock so it wouldn't mess up USB libraries
ISR(MILLISTIMER_OVF_vect, ISR_NOBLOCK)
{
  // copy these to local variables so they can be stored in registers
  // (volatile variables must be read from memory on every access)
  unsigned long m = millis_timer_millis;
  unsigned char f = millis_timer_fract;

/* rmv: The code below generates considerably less code (emtpy Sketch is 326 versus 304)...

  m += MILLIS_INC;
  f += FRACT_INC;
  if (f >= FRACT_MAX) {
    f -= FRACT_MAX;
    m += 1;
  }
...rmv */

  f += FRACT_INC;
  
  if (f >= FRACT_MAX) 
  {
    f -= FRACT_MAX;
    m = m + MILLIS_INC + 1;
  }
  else
  {
    m += MILLIS_INC;
  }

  millis_timer_fract = f;
  millis_timer_millis = m;
  millis_timer_overflow_count++;
}

unsigned long millis()
{
  unsigned long m;
  uint8_t oldSREG = SREG;

  // disable interrupts while we read millis_timer_millis or we might get an
  // inconsistent value (e.g. in the middle of a write to millis_timer_millis)
  cli();
  m = millis_timer_millis;
  SREG = oldSREG;

  return m;
}

unsigned long micros() 
{
  unsigned long m;
  uint8_t oldSREG = SREG, t;
  
  cli();
  m = millis_timer_overflow_count;
  t = MillisTimer_GetCount();
  
  if (MillisTimer_IsOverflowSet() && (t < 255))
    m++;

  SREG = oldSREG;
  
#if (MillisTimer_Prescale_Value >= clockCyclesPerMicrosecond())
  return ((m << 8) + t) * (MillisTimer_Prescale_Value / clockCyclesPerMicrosecond());
#else
  return ((m << 8) + t) / (clockCyclesPerMicrosecond() / MillisTimer_Prescale_Value);
#endif
}

void delay(unsigned long ms)
{
  uint16_t start = (uint16_t)micros();

  while (ms > 0) {
    if (((uint16_t)micros() - start) >= 1000) {
      ms--;
      start += 1000;
    }
  }
}

#if F_CPU == 16500000L
  // optimised delay loop from Bluebie contributed to Digispark project
  // deals accurately with half-mhz clock speed, but can only delay in increments of 2us rounded down
  // this loop has been tuned empirically with an oscilloscope and works in avr-gcc 4.5.1
  void delayMicroseconds(unsigned int us){
    us &= ((unsigned int) 0) - ((unsigned int) 2); // remove least signifficant bit
  	while (us > 1) {
  		// 16 nops
  		asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");
  		asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");
  		// 11 nops
  		asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");
  		asm("NOP");asm("NOP");asm("NOP");
    
  		us -= 2;
  	}
  }
#else
  /* Improved delayMicroseconds function
   * Copyright (c) 2011, Paul Stoffregen, paul at pjrc dot com
   * 
   * Permission is hereby granted, free of charge, to any person obtaining a copy
   * of this software and associated documentation files (the "Software"), to deal
   * in the Software without restriction, including without limitation the rights
   * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   * copies of the Software, and to permit persons to whom the Software is
   * furnished to do so, subject to the following conditions:
   * 
   * The above copyright notice and this permission notice shall be included in
   * all copies or substantial portions of the Software.
   * 
   * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   * THE SOFTWARE.
   */

  // modified by Bluebie in 2013 for Digispark project
  // #include <stdint.h>
  // #include <avr/io.h>
  
  void delayMicroseconds(uint16_t usec) {
		asm volatile(
		#if F_CPU == 16000000L
			"sbiw	%A0, 2"			"\n\t"	// 2
			"brcs	L_%=_end"		"\n\t"	// 1
			"breq	L_%=_end"		"\n\t"	// 1
			"lsl	%A0"			"\n\t"	// 1
			"rol	%B0"			"\n\t"	// 1
			"lsl	%A0"			"\n\t"	// 1
			"rol	%B0"			"\n\t"	// 1  overhead: (8)/4 = 2us
		#elif F_CPU == 8000000L
			"sbiw	%A0, 3"			"\n\t"	// 2
			"brcs	L_%=_end"		"\n\t"	// 1
			"breq	L_%=_end"		"\n\t"	// 1
			"lsl	%A0"			"\n\t"	// 1
			"rol	%B0"			"\n\t"	// 1  overhead: (6)/2 = 3 us
		#elif F_CPU == 4000000L
			"sbiw	%A0, 4"			"\n\t"	// 2
			"brcs	L_%=_end"		"\n\t"	// 1
			"breq	L_%=_end"		"\n\t"	// 1  overhead: (4) = 4 us
		#elif F_CPU == 2000000L
			"sbiw	%A0, 12"		"\n\t"	// 2
			"brcs	L_%=_end"		"\n\t"	// 1
			"breq	L_%=_end"		"\n\t"	// 1
			"lsr	%B0"			"\n\t"	// 1
			"ror	%A0"			"\n\t"	// 1  overhead: (6)*2 = 12 us
		#elif F_CPU == 1000000L
			"sbiw	%A0, 32"		"\n\t"	// 2
			"brcs	L_%=_end"		"\n\t"	// 1
			"breq	L_%=_end"		"\n\t"	// 1
			"lsr	%B0"			"\n\t"	// 1
			"ror	%A0"			"\n\t"	// 1
			"lsr	%B0"			"\n\t"	// 1
			"ror	%A0"			"\n\t"	// 1  overhead: (8)*4 = 32 us
		#endif
		"L_%=_loop:"
			"sbiw	%A0, 1"			"\n\t"	// 2
			"brne	L_%=_loop"		"\n\t"	// 2
		"L_%=_end:"
			: "+w" (usec)
			: "0" (usec)
		);
  }
#endif

static void initToneTimerInternal(void)
{
  // Stop the clock while we make changes
  ToneTimer_ClockSelect( ToneTimer_(Stopped) );

  // Set the timer to phase-correct PWM
  #if defined( TONETIMER_SUPPORTS_PHASE_CORRECT_PWM ) && TONETIMER_SUPPORTS_PHASE_CORRECT_PWM
    ToneTimer_SetWaveformGenerationMode( ToneTimer_(Phase_Correct_PWM_FF) );
  #else
    ToneTimer_SetWaveformGenerationMode( ToneTimer_(Fast_PWM_FF) );
  #endif

  // Timer is processor clock divided by ToneTimer_Prescale_Index (64)
  ToneTimer_ClockSelect( ToneTimer_Prescale_Index );
}

void initToneTimer(void)
{
  // Ensure the timer is in the same state as power-up
  ToneTimer_SetToPowerup();

  #if defined( INITIALIZE_SECONDARY_TIMERS ) && INITIALIZE_SECONDARY_TIMERS
    // Prepare the timer for PWM
    initToneTimerInternal();
  #endif
}

#if F_CPU != 16500000L
  // used to detect bootloader applying calibration in init
  byte read_factory_calibration(void)
  {
    byte SIGRD = 5; // for some reason this isn't defined...
    byte value = boot_signature_byte_get(1);
    return value;
  }
#endif

void init(void)
{
  // clock calibration stuff
  // recalibrate clock if it was calibrated by bootloader (like micronucleus)
  #if F_CPU != 16500000L
    if (OSCCAL != read_factory_calibration()) {
      // adjust the calibration down from 16.5mhz to 16.0mhz
      if (OSCCAL >= 128) {
        // maybe 8 is better? oh well - only about 0.3% out anyway
        OSCCAL -= 7;
      } else {
        OSCCAL -= 5;
      }
    }
  #endif
    
  // TODO: detect if fuses set to PLL, regular internal oscillator or external and change behaviour in this next section...
  #if F_CPU < 16000000L
    cli();
    CLKPR = 0b10000000;
    #if F_CPU == 8000000L
      CLKPR = 1; // div 2
    #elif F_CPU == 4000000L
      CLKPR = 2 // div 4
    #elif F_CPU == 2000000L
      CLKPR = 3; // div 8
    #elif F_CPU == 1000000L
      CLKPR = 4; // div 16
    #elif F_CPU == 500000L
      CLKPR = 5; // div 32 = 500khz
    #elif F_CPU == 250000L
      CLKPR = 6; // div 64 = 250khz
    #elif F_CPU == 125000L
      CLKPR = 7; // div 128 = 125khz cpu clock
    #else
      #warning "Cannot prescale chip to specified F_CPU speed"
    #endif
  #endif
  
  // this needs to be called before setup() or some functions won't work there
  sei();

  // In case the bootloader left our millis timer in a bad way
  #if defined( HAVE_BOOTLOADER ) && HAVE_BOOTLOADER
    MillisTimer_SetToPowerup();
  #endif

  // Use the Millis Timer for fast PWM
  MillisTimer_SetWaveformGenerationMode( MillisTimer_(Fast_PWM_FF) );

  // Millis timer is always processor clock divided by MillisTimer_Prescale_Value (64)
  MillisTimer_ClockSelect( MillisTimer_Prescale_Index );

  // Enable the overlow interrupt (this is the basic system tic-toc for millis)
  MillisTimer_EnableOverflowInterrupt();

  // Initialize the timer used for Tone
  #if defined( INITIALIZE_SECONDARY_TIMERS ) && INITIALIZE_SECONDARY_TIMERS
    initToneTimerInternal();
  #endif

  // Initialize the ADC
  #if defined( INITIALIZE_ANALOG_TO_DIGITAL_CONVERTER ) && INITIALIZE_ANALOG_TO_DIGITAL_CONVERTER
    ADC_PrescalerSelect( ADC_ARDUINO_PRESCALER );
    ADC_Enable();
  #endif
}

