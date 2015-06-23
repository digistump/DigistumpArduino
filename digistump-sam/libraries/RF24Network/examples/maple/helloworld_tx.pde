/*
 Copyright (C) 2011 James Coliz, Jr. <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

/**
 * Simplest possible example of using RF24Network 
 *
 * TRANSMITTER NODE
 * Every 2 seconds, says hello to the receiver node.
 */

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

//
// Maple specific setup.  Other than this section, the sketch is the same on Maple as on
// Arduino
//

#ifdef MAPLE_IDE

// External startup function
extern void board_start(const char* program_name);

// Use SPI #2.
HardwareSPI SPI(2);

inline void serial_begin(int _baud)
{
}
#else
inline void serial_begin(int _baud)
{
  Serial.begin(_baud);
}
#define board_startup printf
#define toggleLED(x) (x)
#endif

//
// Hardware configuration
//

// Set up nRF24L01 radio on SPI bus plus pins 7 & 6
// (This works for the Getting Started board plugged into the
// Maple Native backwards.)

RF24 radio(7,6);

// Network uses that radio
RF24Network network(radio);

// Address of our node
const uint16_t this_node = 1;

// Address of the other node
const uint16_t other_node = 0;

// How often to send 'hello world to the other unit
const unsigned long interval = 2000; //ms

// When did we last send?
unsigned long last_sent;

void setup(void)
{
  serial_begin(57600);
  board_start("RF24Network/examples/helloworld_tx/");
 
  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ this_node);
}

void loop(void)
{
  // Pump the network regularly
  network.update();

  // If it's time to send a message, send it!
  unsigned long now = millis();
  if ( now - last_sent > interval  )
  {
    last_sent = now;

    toggleLED();
    printf("Sending...\r\n");
    const char* hello = "Hello, world!";
    RF24NetworkHeader header(/*to node*/ other_node);
    bool ok = network.write(header,hello,strlen(hello));
    if (ok)
      printf("\tok.\r\n");
    else
    {
      printf("\tfailed.\r\n");
      delay(250); // extra delay on fail to keep light on longer
    }
    toggleLED();
  }
}
// vim:ai:cin:sts=2 sw=2 ft=cpp
