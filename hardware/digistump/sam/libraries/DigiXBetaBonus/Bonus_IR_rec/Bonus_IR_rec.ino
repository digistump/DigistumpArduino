int irPin=11;
 
void setup()
{
 pinMode(irPin,INPUT);
 pinMode(13,OUTPUT);
 Serial.begin(9600);
 digitalWrite(13,HIGH);
     Serial.println("You pressed a button");
     delay(1000);
     digitalWrite(13,LOW);
}
 
void loop()
{
 
  if(pulseIn(irPin,LOW))
  {
     //button pressed 
     delay(100);
     digitalWrite(13,HIGH);
     Serial.println("You pressed a button");
     delay(1000);
     digitalWrite(13,LOW);
  }
  
}