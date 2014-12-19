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
  Serial.print("Server running at: ");
  String address = wifi.server(8080);//sets up server and returns IP
  Serial.println(address); 

//  wifi.close();
}

void loop()
{

  if ( wifi.serverRequest()){
      Serial.print("Request for: ");
     Serial.println(wifi.serverRequestPath());
     if(wifi.serverRequestPath()!="/")
       wifi.serverResponse("404 Not Found",404); 
      else
       wifi.serverResponse("<html><body><h1>This is a test</h1></body></html>"); //defaults to 200
  }

  delay(10);  
}