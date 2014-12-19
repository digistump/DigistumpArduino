#include <DigiFi.h>
DigiFi wifi;

void setup()
{
  Serial.begin(9600); 
  wifi.begin(9600);

  //DigiX trick - since we are on serial over USB wait for character to be entered in serial terminal
  while(!Serial.available()){
    Serial.println("Enter any key to begin");
    delay(1000);
  }

  Serial.println("Starting");

  while (wifi.ready() != 1)
  {
    Serial.println("Error connecting to network");
    delay(15000);
  }  
  
  Serial.println("Connected to wifi!");
  wifi.startATMode();
  Serial.println(wifi.ping("192.168.2.1"));
  wifi.endATMode();
  wifi.close();
}

void loop()
{

}
