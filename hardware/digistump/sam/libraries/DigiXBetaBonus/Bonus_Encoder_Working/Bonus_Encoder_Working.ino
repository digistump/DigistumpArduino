/* Software Debouncing - Mechanical Rotary Encoder */

#define encoder0PinA 9
#define encoder0PinB 10
const int buttonPin = 12; 
volatile unsigned int encoder0Pos = 0;
static boolean rotating=false;
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

void setup() {
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, HIGH);
  pinMode(encoder0PinA, INPUT); 
  digitalWrite(encoder0PinA, HIGH);       
  pinMode(encoder0PinB, INPUT); 
  digitalWrite(encoder0PinB, HIGH); 

  attachInterrupt(encoder0PinA, rotEncoder, CHANGE);  
  SerialUSB.begin (9600);
}

void rotEncoder(){
  rotating=true; 
  // If a signal change (noise or otherwise) is detected
  // in the rotary encoder, the flag is set to true
}

void loop() {
  while(rotating) {
    delay(2);
    // When signal changes we wait 2 milliseconds for it to 
    // stabilise before reading (increase this value if there
    // still bounce issues)
    if (digitalRead(encoder0PinA) == digitalRead(encoder0PinB)) {  
      encoder0Pos++;
    } 
    else {                                   
      encoder0Pos--;
    }
    rotating=false; // Reset the flag back to false
    SerialUSB.println(encoder0Pos);
  }
  int reading = digitalRead(buttonPin);

  // check to see if you just pressed the button 
  // (i.e. the input went from LOW to HIGH),  and you've waited 
  // long enough since the last press to ignore any noise:  

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
    SerialUSB.print("Button: ");
    SerialUSB.println(reading);
  } 
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:
    buttonState = reading;
  }


  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastButtonState = reading;
}
