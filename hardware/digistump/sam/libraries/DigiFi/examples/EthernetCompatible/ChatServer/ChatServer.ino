/*
 Chat  Server

 A simple server that distributes any incoming messages to all
 connected clients.  To use telnet to  your device's IP address and type.
 You can see the client's input in the serial monitor as well.
 Using a DigiX.

 Circuit:

 * Analog inputs attached to pins A0 through A5 (optional)

 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 modified Dec 22, 2013 for use with DigiX by Erik Kettenburg

 */

#include <DigiFi.h>


DigiFi server;
boolean alreadyConnected = false; // whether or not the client was connected previously

void setup() {

  
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  //DigiX trick - since we are on serial over USB wait for character to be entered in serial terminal
  while(!Serial.available()){
    Serial.println("Enter any key to begin");
    delay(1000);
  }

  // start listening for clients
  server.begin();
  server.server(8080); //connect to it on port 8080

  while (server.ready() != 1)
    {
      Serial.println("Connecting to network...");
      delay(1000);
    } 

  Serial.print("Chat server address:");
  Serial.println(server.localIP());
}

void loop() {
  // wait for a new client:

  // when the client sends the first byte, say hello:
  if (server.available() > 0) {
    if (!alreadyConnected) {
      // clead out the input buffer:
      server.flush();
      Serial.println("We have a new client");
      server.println("Hello, client!");
      alreadyConnected = true;
    }

    
      // read the bytes incoming from the client:
      char thisChar = server.read();
      // echo the bytes back to the client:
      server.write(thisChar);
      // echo the bytes to the server as well:
      Serial.write(thisChar);
  }
  
}



