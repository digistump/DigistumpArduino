// DigiX WiFi module example - released by Digistump LLC/Erik Kettenburg under CC-BY-SA 3.0
// Inspired by HttpClient library by MCQN Ltd.

#include <DigiFi.h>

DigiFi wifi;

void setup()
{
  // initialize serial communications at 9600 bps:
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
  
//GET request example

  if(wifi.get("digistump.com","/test.txt")){
    String body = wifi.body();
    Serial.println(body);
  }
  else{
    Serial.println("error");
  
  }
  
  //POST request example
Serial.println("Sending tweet!");
 //To use thingspeak for sending tweets see: http://community.thingspeak.com/documentation/apps/thingtweet/
  if(wifi.post("api.thingspeak.com","/apps/thingtweet/1/statuses/update","api_key=[YOURTHINGTWEETAPIKEY]&status="+wifi.URLEncode("Tweet from my new DigiX! #digix #digistump http://digistump.com"))){
    String body = wifi.body();
    Serial.println(body);
  }
  else{
    Serial.println("error");
  
  }
  

  wifi.close();
}

void loop()
{
  

}