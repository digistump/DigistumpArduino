#include <DigiCDC.h>
void setup() {                
  SerialUSB.begin();
  // initialize the digital pin 1 as an DTR output.
  SerialUSB.setDtrPin(1);
}

// the loop routine runs over and over again forever:
void loop() {
   SerialUSB.delay(10);
}
