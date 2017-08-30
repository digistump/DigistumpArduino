/*
 * Use your Wii Classic Controller as a USB Joystick
 */
 
#include <DigiJoystick.h>
#include <ArduinoWiiClassicController.h>

ArduinoWiiClassicController classic = ArduinoWiiClassicController(); 

void setup() {
  classic.init();
}


void loop() {
  classic.update();
  
  //Left Stick
  DigiJoystick.setX(byte(map(classic.rightAnalogX,0,32,0,255)));
  DigiJoystick.setY(byte(map(classic.rightAnalogY,0,32,255,0))); //Invert Y axis. Check on Windows    

  //Right Stick
  DigiJoystick.setXROT(byte(map(classic.leftAnalogX,0,64,0,255)));
  DigiJoystick.setYROT(byte(map(classic.leftAnalogY,0,64,255,0))); //Invert Y axis. Check on Windows

  //Analog buttons
  DigiJoystick.setZROT(byte(map(classic.rightTriggerButton,0,32,0,255)));
  DigiJoystick.setSLIDER(byte(map(classic.leftTriggerButton,0,32,0,255)));
  
  byte buttonLSB = 0;
  bitWrite(buttonLSB, 0, classic.rightTriggerButton);
  bitWrite(buttonLSB, 1, classic.leftTriggerButton);
  bitWrite(buttonLSB, 2, classic.minusButton);
  bitWrite(buttonLSB, 3, classic.homeButton);
  bitWrite(buttonLSB, 4, classic.plusButton);
  bitWrite(buttonLSB, 5, classic.aButton);
  bitWrite(buttonLSB, 6, classic.bButton);
  bitWrite(buttonLSB, 7, classic.yButton);

  byte buttonMSB = 0;
  bitWrite(buttonMSB, 0, classic.xButton);
  bitWrite(buttonMSB, 1, classic.dpadUp);
  bitWrite(buttonMSB, 2, classic.dpadDown);
  bitWrite(buttonMSB, 3, classic.dpadRight);
  bitWrite(buttonMSB, 4, classic.dpadLeft);
  bitWrite(buttonMSB, 5, classic.zLeftButton);
  bitWrite(buttonMSB, 6, classic.zRightButton);
  bitWrite(buttonMSB, 7, 0);
  
  DigiJoystick.setButtons(buttonLSB,buttonMSB);
  DigiJoystick.delay(10);
}
