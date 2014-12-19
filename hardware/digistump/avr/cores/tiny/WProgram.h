#ifndef WProgram_h
#define WProgram_h

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <avr/interrupt.h>

#include "core_build_options.h"
#include "core_pins.h"
#include "wiring.h"
#include "pins_arduino.h"

#ifdef __cplusplus
#include "WCharacter.h"
#include "WString.h"
#include "TinyDebugSerial.h"
#include "HardwareSerial.h"

uint16_t makeWord(uint16_t w);
uint16_t makeWord(byte h, byte l);

#define word(...) makeWord(__VA_ARGS__)

unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout = 1000000L);

void tone(uint8_t _pin, unsigned int frequency, unsigned long duration = 0);
void noTone(uint8_t _pin = 255);

// WMath prototypes
long random(long);
long random(long, long);
void randomSeed(unsigned int);
long map(long, long, long, long, long);

/*
  fix?  On the Mega processors, the analogs are also "extended" digital pins.
  To (sort of) work the same way with this core, the following constants 
  would have to be valid arguments to digitalRead, digitalWrite, and pinMode 
  ("the digitals").  Which means the digitals would have to check for pins 
  over A0 and then subtract A0.  The current plan is to wait until someone 
  wants this feature.
*/
#if CORE_ANALOG_COUNT >= 1
const static uint8_t A0 = CORE_ANALOG_FIRST + 0;
#endif

#if CORE_ANALOG_COUNT >= 2
const static uint8_t A1 = CORE_ANALOG_FIRST + 1;
#endif

#if CORE_ANALOG_COUNT >= 3
const static uint8_t A2 = CORE_ANALOG_FIRST + 2;
#endif

#if CORE_ANALOG_COUNT >= 4
const static uint8_t A3 = CORE_ANALOG_FIRST + 3;
#endif

#if CORE_ANALOG_COUNT >= 5
const static uint8_t A4 = CORE_ANALOG_FIRST + 4;
#endif

#if CORE_ANALOG_COUNT >= 6
const static uint8_t A5 = CORE_ANALOG_FIRST + 5;
#endif

#if CORE_ANALOG_COUNT >= 7
const static uint8_t A6 = CORE_ANALOG_FIRST + 6;
#endif

#if CORE_ANALOG_COUNT >= 8
const static uint8_t A7 = CORE_ANALOG_FIRST + 7;
#endif

#if CORE_ANALOG_COUNT >= 9
#error Update the A* definitions for the selected processor.
#endif

#endif

#endif
