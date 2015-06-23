/*
 Copyright (C) 2011 James Coliz, Jr. <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

/**
 * Example of a sensor network 
 *
 * This sketch demonstrates how to use the RF24Network library to
 * manage a set of low-power sensor nodes which mostly sleep but
 * awake regularly to send readings to the base.
 *
 * The example uses TWO sensors, a 'temperature' sensor and a 'voltage'
 * sensor.
 *
 * To see the underlying frames being relayed, compile RF24Network with
 * #define SERIAL_DEBUG.
 *
 * The logical node address of each node is set in EEPROM.  The nodeconfig
 * module handles this by listening for a digit (0-9) on the serial port,
 * and writing that number to EEPROM.
 */

#include <avr/pgmspace.h>
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include <Tictocs.h>
#include <Button.h>
#include <TictocTimer.h>
#include "nodeconfig.h"
#include "sleep.h"
#include "S_message.h"
#include "printf.h"

// This is for git version tracking.  Safe to ignore
#ifdef VERSION_H
#include "version.h"
#else
#define __TAG__ "Unknown"
#endif

// Pin definitions
#ifndef PINS_DEFINED
#define __PLATFORM__ "Getting Started board"

// Pins for radio
const int rf_ce = 9;
const int rf_csn = 10;

// Pins for sensors
const int temp_pin = A2;
const int voltage_pin = A3;

// Pins for status LED, or '0' for no LED connected
const int led_red = 0; 
const int led_yellow = 0; 
const int led_green = 0; 

// Button to control modes
const int button_a = 4;

// What voltage is a reading of 1023?
const unsigned voltage_reference = 5 * 256; // 5.0V
#endif

RF24 radio(rf_ce,rf_csn);
RF24Network network(radio);

// Our node configuration 
eeprom_info_t this_node;

// Sleep constants.  In this example, the watchdog timer wakes up
// every 4s, and every single wakeup we power up the radio and send
// a reading.  In real use, these numbers which be much higher.
// Try wdt_8s and 7 cycles for one reading per minute.> 1
const wdt_prescalar_e wdt_prescalar = wdt_8s;
const int sleep_cycles_per_transmission = 1;

// Non-sleeping nodes need a timer to regulate their sending interval
Timer send_timer(8000);

// Button controls functionality of the unit
Button ButtonA(button_a);

// Long-press button
Button ButtonLong(button_a,1000);

/**
 * Convenience class for handling LEDs.  Handles the case where the
 * LED may not be populated on the board, so always checks whether
 * the pin is valid before setting a value.
 */

class LED
{
private:
  int pin;
public:
  LED(int _pin): pin(_pin)
  {
    if (pin > 0)
    {
      pinMode(pin,OUTPUT);
      digitalWrite(pin,LOW);
    }
  }
  void write(bool state) const
  {
    if (pin > 0)
      digitalWrite(pin,state?HIGH:LOW);
  }
  void operator=(bool state)
  {
    write(state);
  }

};

/**
 * Startup LED sequence.  Lights up the LEDs in sequence first, then dims 
 * them in the same sequence.
 */

class StartupLEDs: public Timer
{
private:
  const LED** leds;
  const LED** current;
  const LED** end;
  bool state;
protected:
  virtual void onFired(void)
  {
    (*current)->write(state);
    ++current;
    if ( current >= end )
    {
      if ( state )
      {
	state = false;
	current = leds;
      }
      else
	disable();
    }
  }
public:
  StartupLEDs(const LED** _leds, int _num): Timer(250), leds(_leds), current(_leds), end(_leds+_num), state(true)
  {
  }
};

/**
 * Calibration LED sequence.  Flashes all 3 in unison
 */
class CalibrationLEDs: public Timer
{
  const LED** leds;
  const LED** end;
  bool state;
protected:
  void write()
  {
    const LED** current = end;
    while (current-- > leds)
      (*current)->write(state);
  }
  virtual void onFired() 
  {
    state = ! state;
    write();
  }
public:
  CalibrationLEDs(const LED** _leds, int _num, unsigned long duration = 500): Timer(duration), leds(_leds), end(_leds+_num), state(false)
  {
    Timer::disable();
  }
  void begin()
  {
    Updatable::begin();
  }
  void reset()
  {
    state = true;
    write();
    Timer::reset();
  }
  void disable()
  {
    state = false;
    write();
    Timer::disable();
  }
};

LED Red(led_red), Yellow(led_yellow), Green(led_green);

const LED* leds[] = { &Red, &Yellow, &Green }; 
const int num_leds = sizeof(leds)/sizeof(leds[0]);
StartupLEDs startup_leds(leds,num_leds);
CalibrationLEDs calibration_leds(leds,num_leds);

// Nodes in test mode do not sleep, but instead constantly try to send
bool test_mode = false;

// Nodes in calibration mode are looking for temperature calibration
bool calibration_mode = false;

// Helper functions to take readings

// How many measurements to take.  64*1024 = 65536, so 64 is the max we can fit in a uint16_t.
const int num_measurements = 64;

uint32_t measure_temp()
{
    int i = num_measurements;
    uint32_t reading = 0;
    while(i--)
      reading += analogRead(temp_pin);
    
    // Convert the reading to celcius*256
    // This is the formula for MCP9700.
    // C = reading * 1.1
    // C = ( V - 1/2 ) * 100
    //
    // Then adjust for the calibation value on this sensor
    return ( ( ( ( reading * 0x120 ) - 0x800000 ) * 0x64 ) >> 16 ) + this_node.temp_calibration;
}

uint32_t measure_voltage()
{
    // Take the voltage reading 
    int i = num_measurements;
    uint32_t reading = 0;
    while(i--)
      reading += analogRead(voltage_pin);

    // Convert the voltage reading to volts*256
    return ( reading * voltage_reference ) >> 16; 
}

struct CalibrationData
{
  uint8_t measurements_remaining;
  static const uint8_t measurements_needed = 16;
  int16_t accumulator;

  CalibrationData()
  {
    measurements_remaining = measurements_needed;
    accumulator = 0;
  }
  void add(int16_t reading)
  {
    accumulator += reading / measurements_needed;
    if ( measurements_remaining )
      --measurements_remaining;
  }
  bool done() const
  {
    return measurements_remaining == 0;
  }
  int16_t result() const
  {
    return accumulator;
  }
};

CalibrationData calibration_data;

void setup(void)
{
  //
  // Print preamble
  //
  
  Serial.begin(57600);
  printf_begin();
  printf_P(PSTR("\n\rRF24Network/examples/sensornet/\n\r"));
  printf_P(PSTR("PLATFORM: " __PLATFORM__ "\n\r"));
  printf_P(PSTR("VERSION: " __TAG__ "\n\r"));
  
  //
  // Pull node address out of eeprom 
  //

  // Which node are we?
  this_node = nodeconfig_read();

  //
  // Prepare sleep parameters
  //

  // Only the leaves sleep.  Nodes 01-05 are presumed to be relay nodes. 
  if ( ! this_node.relay )
    Sleep.begin(wdt_prescalar,sleep_cycles_per_transmission);

  //
  // Set up board hardware
  //
  ButtonA.begin();
  ButtonLong.begin();

  // Sensors use the stable internal 1.1V voltage
#ifdef INTERNAL1V1
  analogReference(INTERNAL1V1);
#else
  analogReference(INTERNAL);
#endif

  // Prepare the startup sequence
  send_timer.begin();
  startup_leds.begin();
  calibration_leds.begin();

  //
  // Bring up the RF network
  //

  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 92, /*node address*/ this_node.address);
}

void loop(void)
{
  // Update objects
  theUpdater.update();

  // Pump the network regularly
  network.update();

  // Is there anything ready for us?
  while ( network.available() )
  {
    // If so, grab it and print it out
    RF24NetworkHeader header;
    S_message message;
    network.read(header,&message,sizeof(message));
    printf_P(PSTR("%lu: APP Received #%u type %c %s from 0%o\n\r"),millis(),header.id,header.type,message.toString(),header.from_node);

    // If we get a message, that's a little odd, because this sketch doesn't run on the
    // base node.  Possibly it's a test message from a child node.  Possibly it's a sensor
    // calibration message from a parent node.
    //
    // Either way, it only matters if we're NOT sleeping, and also only useful it we have
    // a temp sensor

    // If we have a temp sensor AND we are not sleeping
    if ( temp_pin > -1 && ( ! Sleep || test_mode ) )
    {
      // if the received message is a test message, we can respond with a 'C' message in return
      if ( header.type == 'c' )
      {
	// Take a reading
	S_message response;
	response.temp_reading = measure_temp(); 
	response.voltage_reading = measure_voltage();

	// Send it back as a calibration message
	RF24NetworkHeader response_header(/*to node*/ header.from_node, /*type*/ 'C');
	network.write(response_header,&response,sizeof(response));
      }
      else if ( header.type == 'C' )
      {
	// This is a calibration message.  Calculate the diff
	uint16_t diff = message.temp_reading - measure_temp();
	printf_P(PSTR("%lu: APP Calibration received %04x diff %04x\n\r"),millis(),message.temp_reading,diff);
	calibration_data.add(diff);

	if ( calibration_data.done() )
	{
	  calibration_mode = false;
	  calibration_leds.disable();
	  printf_P(PSTR("%lu: APP Stop calibration mode. Calibrate by %04x\n\r"),millis(),calibration_data.result());

	  // Now apply the calibration
	  this_node.temp_calibration += calibration_data.result();
	  // And save it to eeprom...
	  set_temp_calibration( this_node.temp_calibration );
	}

      }
    }
  }

  // If we are the kind of node that sends readings, 
  // AND we have a temp sensor
  // AND it's time to send a reading 
  // AND we're in the mode where we send readings...
  if ( this_node.address > 0 && temp_pin > -1 && ( ( Sleep && ! test_mode ) || send_timer.wasFired() ) && ! startup_leds )
  {
    // Transmission beginning, TX LED ON
    Yellow = true;
    if ( test_mode && ! calibration_mode )
    {
      Green = false;
      Red = false;
    }

    S_message message;
    message.temp_reading = measure_temp(); 
    message.voltage_reading = measure_voltage(); 

    char message_type = 'S';
    if ( calibration_mode )
      message_type = 'c';
    else if (test_mode )
      message_type = 't';

    // By default send to the base
    uint16_t to_node = 0;
    if ( test_mode )
      // In test mode, sent to our parent.
      to_node = network.parent();

    printf_P(PSTR("---------------------------------\n\r"));
    printf_P(PSTR("%lu: APP Sending type-%c %s to 0%o...\n\r"),millis(),message_type,message.toString(),to_node);
   
    RF24NetworkHeader header(to_node,message_type);
    bool ok = network.write(header,&message,sizeof(message));
    if (ok)
    {
      if ( test_mode && ! calibration_mode )
	Green = true;
      printf_P(PSTR("%lu: APP Send ok\n\r"),millis());
    }
    else
    {
      if ( test_mode && ! calibration_mode )
	Red = true;
      printf_P(PSTR("%lu: APP Send failed\n\r"),millis());
    }

    // Transmission complete, TX LED OFF
    Yellow = false;
   
    if ( Sleep && ! test_mode ) 
    {
      // Power down the radio.  Note that the radio will get powered back up
      // on the next write() call.
      radio.powerDown();

      // Be sure to flush the serial first before sleeping, so everything
      // gets printed properly
      Serial.flush();
      
      // Sleep the MCU.  The watchdog timer will awaken in a short while, and
      // continue execution here.
      Sleep.go();
    }
  }

  // Button
  unsigned a = ButtonA.wasReleased();
  if ( a && a < 500 )
  {
    // Pressing the button during startup sequences engages test mode.
    // Pressing it after turns off test mode.
    if ( startup_leds )
    {
      test_mode = true;
      send_timer.setInterval(1000);
      printf_P(PSTR("%lu: APP Start test mode\n\r"),millis());
    }
    else if ( calibration_mode )
    {
      calibration_mode = false;
      calibration_leds.disable();
      printf_P(PSTR("%lu: APP Stop calibration mode\n\r"),millis());
    }
    else if ( test_mode )
    {
      test_mode = false;
      Green = false;
      Red = false;
      send_timer.setInterval(8000);
      printf_P(PSTR("%lu: APP Stop test mode\n\r"),millis());
    }
  }

  // Long press
  if ( ButtonLong.wasPressed() && test_mode )
  {
    calibration_mode = true;
    calibration_leds.reset();
    calibration_data = CalibrationData();
    printf_P(PSTR("%lu: APP Start calibration mode\n\r"),millis());
  }

  // Listen for a new node address
  nodeconfig_listen();
}
// vim:ai:cin:sts=2 sw=2 ft=cpp
