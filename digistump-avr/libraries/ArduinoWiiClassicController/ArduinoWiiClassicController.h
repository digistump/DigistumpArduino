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

#ifndef ArduinoWiiClassicController_H
#define ArduinoWiiClassicController_H

#include <Arduino.h>

class ArduinoWiiClassicController
{
  public:
    int leftAnalogX;
    int leftAnalogY;
    int rightAnalogX;
    int rightAnalogY;

    int leftTrigger; //Analog, when partially pressed
    int rightTrigger; //Analog, when partially pressed

    int dpadUp;
    int dpadDown;
    int dpadLeft;
    int dpadRight;

    int leftTriggerButton; //Digital, when fully pressed
    int rightTriggerButton; //Digital, when fully pressed
    int minusButton;
    int homeButton;
    int plusButton;
    int zLeftButton;
    int bButton;
    int yButton;
    int aButton;
    int xButton;
    int zRightButton;

    void init();
    void update();

  private:
    void _sendByte(byte data, byte location);
};

#endif
