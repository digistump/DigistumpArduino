#include <DigiCDC.h>
void setup() {                
  // initialize the digital pin as an output.
  SerialUSB.begin(); 
  SerialUSB.println("CDC Test");  
}

// the loop routine runs over and over again forever:
void loop() {
  
  if (SerialUSB.available()) {
    SerialUSB.write(SerialUSB.read());
  }
  
   SerialUSB.delay(10);               // keep usb alive // can alos use SerialUSB.refresh();
}