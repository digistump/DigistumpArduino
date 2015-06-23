/*
  Web client
 
 This sketch connects to a website (http://www.google.com)
 using an DigiX. 

 Based on example by David A. Mellis, Tom Igoe, and Adrian McEwen
 
 */

#include <DigiFi.h>


// if you don't want to use DNS - though it is handeled by the WiFI module
//so there is no size penalty for using it -
// use the numeric IP instead of the name for the server:
//IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
char server[] = "digistump.com";    // name address for Google (using DNS)

// Initialize the Wifi  library (client/server/and main all in one)
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):
DigiFi client;

void setup() {
 // Open serial communications and wait for port to open:
  Serial.begin(9600);

  
//DigiX trick - since we are on serial over USB wait for character to be entered in serial terminal
  while(!Serial.available()){
    Serial.println("Enter any key to begin");
    delay(1000);
  }


  // start the connection:
   client.begin(9600);
   //client.setDebug(true); 
  //wait for module to be ready
  while (client.ready() != 1)
  {
    Serial.println("Connecting to network...");
    delay(1000);
  }  
  
  
  Serial.println("connecting...");

  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    Serial.println("connected");
    // Make a HTTP request:
    client.println("GET /test.txt HTTP/1.1");
    client.println("Host: www.digistump.com");
    client.println("Connection: close");
    client.println();
  } 
  else {
    // kf you didn't get a connection to the server:
    Serial.println("connection failed");
  }
  
}

void loop()
{
  // if there are incoming bytes available 
  // from the server, read them and print them:
  //while(!Serial1.available()){}
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();

    // do nothing forevermore:
    while(true);
  }
}

