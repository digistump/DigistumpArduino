/*
 * ArduinoWiiClassicController.h - Wii Classic Controller for Arduino / Digispark
 *
 * Copyright 2017 Eduardo Flores - Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License 
 * http://eduardofv.com/2017/02/06/wii-classic-controller-usb-arduino/
 * https://github.com/eduardofv/ArduinoWiiClassicController
 * 
 * Based on ArduinoNunchuck by Gabriel Bianconi
 *
 */

#include <Arduino.h>
#include <TinyWireM.h>
#include "ArduinoWiiClassicController.h"

#define ADDRESS 0x52

void ArduinoWiiClassicController::init()
{
  TinyWireM.begin();

  ArduinoWiiClassicController::_sendByte(0x55, 0xF0);
  ArduinoWiiClassicController::_sendByte(0x00, 0xFB);

  ArduinoWiiClassicController::update();
}

void ArduinoWiiClassicController::update()
{
  int count = 0;
  int values[6];

  //Read data
  TinyWireM.requestFrom(ADDRESS, 6);
  while(TinyWireM.available())
  {
    values[count] = TinyWireM.receive();
    count++;
  }

  //Left Analog Stick
  ArduinoWiiClassicController::leftAnalogX = values[0] & 63;
  ArduinoWiiClassicController::leftAnalogY = values[1] & 63;

  //Right Analog Stick
  //  efv: simplify these once sure they're ok
  unsigned int rx = (values[2] & 128) >> 7;
  rx = rx | ((values[1] & 192) >> 5);
  rx = rx | ((values[0] & 192) >> 3);
  ArduinoWiiClassicController::rightAnalogX = rx;
  ArduinoWiiClassicController::rightAnalogY = values[2] & 31;

  //Left Trigger
  unsigned int lt = (values[3] >> 5);
  lt = lt | ((values[2] & 96) >> 2);
  ArduinoWiiClassicController::leftTrigger = lt;
  
  //Right Trigger
  ArduinoWiiClassicController::rightTrigger = values[3] & 31;

  //Trigger digital buttons
  ArduinoWiiClassicController::leftTriggerButton = !((values[4] & 32) >> 5);
  ArduinoWiiClassicController::rightTriggerButton = !((values[4] & 2) >> 1);

  //DPad
  ArduinoWiiClassicController::dpadUp = !(values[5] & 1);
  ArduinoWiiClassicController::dpadLeft = !((values[5] >> 1) & 1);
  ArduinoWiiClassicController::dpadDown = !((values[4] >> 6) & 1);
  ArduinoWiiClassicController::dpadRight = !((values[4] >> 7) & 1);

  //All the other buttons
  ArduinoWiiClassicController::minusButton = !((values[4] >> 4) & 1);
  ArduinoWiiClassicController::homeButton = !((values[4] >> 3) & 1);
  ArduinoWiiClassicController::plusButton = !((values[4] >> 2) & 1);
  ArduinoWiiClassicController::zLeftButton = !((values[5] >> 7) & 1);
  ArduinoWiiClassicController::bButton = !((values[5] >> 6) & 1);
  ArduinoWiiClassicController::yButton = !((values[5] >> 5) & 1);
  ArduinoWiiClassicController::aButton = !((values[5] >> 4) & 1);
  ArduinoWiiClassicController::xButton = !((values[5] >> 3) & 1);
  ArduinoWiiClassicController::zRightButton = !((values[5] >> 2) & 1);

  ArduinoWiiClassicController::_sendByte(0x00, 0x00);
}

void ArduinoWiiClassicController::_sendByte(byte data, byte location)
{
  TinyWireM.beginTransmission(ADDRESS);

  TinyWireM.send(location);
  TinyWireM.send(data);

  TinyWireM.endTransmission();

  delay(10);
}

