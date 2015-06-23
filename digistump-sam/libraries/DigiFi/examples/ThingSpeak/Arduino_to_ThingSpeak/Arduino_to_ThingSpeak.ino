
#include <DigiFi.h>

// ThingSpeak Settings
char thingSpeakAddress[] = "api.thingspeak.com";
String writeAPIKey = "XXXMX2WYYR0EV68M";
const int updateThingSpeakInterval = 16 * 1000;      // Time interval in milliseconds to update ThingSpeak (number of seconds * 1000 = interval)

// Variable Setup
long lastConnectionTime = 0; 
int failedCounter = 0;

// Initialize DigiFi
DigiFi client;

void setup()
{
  // Start Serial for debugging on the Serial Monitor
  Serial.begin(9600);
  //DigiX trick - since we are on serial over USB wait for character to be entered in serial terminal
  while(!Serial.available()){
    Serial.println("Enter any key to begin");
    delay(1000);
  }
    // Start DigiFi
  startDigiFi();
}

void loop()
{
  // Read value from Analog Input Pin 0
  String analogPin0 = String(analogRead(A0), DEC);
  
 
  // Update ThingSpeak
  if(millis() - lastConnectionTime > updateThingSpeakInterval)
  {
    updateThingSpeak("field1="+analogPin0);
  }
  
  // Check if Arduino Ethernet needs to be restarted
  if (failedCounter > 3 ) {startDigiFi();}
  
}

void updateThingSpeak(String tsData)
{
  if (client.connect(thingSpeakAddress, 80))
  {         

    lastConnectionTime = millis();
    if(wifi.post("api.thingspeak.com","/update","api_key="++writeAPIKey++"&status="+wifi.URLEncode(tsData)))
    {
      Serial.println("Sent to ThingSpeak"); 
      Serial.println();  
      failedCounter = 0;
      Serial.println(wifi.body());
    }
    else
    {
      failedCounter++;
  
      Serial.println("Connection to ThingSpeak failed ("+String(failedCounter, DEC)+")");   
      Serial.println();
    }
    
  }
  else
  {
    failedCounter++;
    
    Serial.println("Connection to ThingSpeak Failed ("+String(failedCounter, DEC)+")");   
    Serial.println();
    
    lastConnectionTime = millis(); 
  }
}

void startDigiFi()
{
  
  

  Serial.println("Connecting Arduino to network...");
  Serial.println();  

  delay(1000);
  
  // Connect to network amd obtain an IP address using DHCP
  client.begin();
  client.setDebug(true);
  while (client.ready() != 1)
  {
    Serial.println("Connecting to network...");
    delay(1000);
  }  
  
  delay(1000);
}