/*
  Telnet client

 This sketch connects to a a telnet server (http://www.google.com)
 using a DigiX.  You'll need a telnet server
 to test this with.
 Processing's ChatServer example (part of the network library) works well,
 running on port 10002. It can be found as part of the examples
 in the Processing application, available at
 http://processing.org/


 created 14 Sep 2010
 modified 9 Apr 2012
 by Tom Igoe
  modified Dec 22, 2013 for use with DigiX by Erik Kettenburg


 */

#include <DigiFi.h>


// Enter the IP address of the server you're connecting to:
IPAddress server(1, 1, 1, 1);

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 23 is default for telnet;
// if you're using Processing's ChatServer, use  port 10002):
DigiFi client;

void setup() {
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
  
//DigiX trick - since we are on serial over USB wait for character to be entered in serial terminal
  while(!Serial.available()){
    Serial.println("Enter any key to begin");
    delay(1000);
  }


 
  Serial.println("Connecting...");
  // start the connection:
   client.begin(9600);
   client.connect(server,23);
   Serial.println("Connected.");
   //client.setDebug(true); 
  //wait for module to be ready
  while (client.ready() != 1)
  {
    Serial.println("Connecting to network...");
    delay(1000);
  }  
}

void loop()
{
  // if there are incoming bytes available
  // from the server, read them and print them:
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }

  // as long as there are bytes in the serial queue,
  // read them and send them out the socket if it's open:
  while (Serial.available() > 0) {
    char inChar = Serial.read();
    if (client.connected()) {
      client.print(inChar);
    }
  }

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
    // do nothing:
    while (true);
  }
}




