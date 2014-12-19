int RedPin = 5;
int GreenPin = 6;
int BluePin = 7;

void setup() {
  // put your setup code here, to run once:
  pinMode(RedPin, OUTPUT);
  pinMode(GreenPin, OUTPUT);
  pinMode(BluePin, OUTPUT);
}

void loop() {
  randomSeed(analogRead(0));
  // put your main code here, to run repeatedly: 
  analogWrite(RedPin,random(255));
  analogWrite(GreenPin,random(255));
  analogWrite(BluePin,random(255));
  
  delay(500);
}
