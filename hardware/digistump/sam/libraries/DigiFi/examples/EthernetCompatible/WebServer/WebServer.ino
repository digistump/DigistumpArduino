/*
  Web Server

 A simple web server that shows the value of the analog input pins.
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

// Initialize the DigiFi library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
DigiFi server;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  //DigiX trick - since we are on serial over USB wait for character to be entered in serial terminal
  while(!Serial.available()){
    Serial.println("Enter any key to begin");
    delay(1000);
  }


  // start the server:
  server.begin();
  server.server(8080); //start server on port 8080
  server.setTCPTimeout(1); //force wifi to close connection after idle for 1 second
  //fix for not being able to close client connections
  //see WebServerChunked for a better approach
  while (server.ready() != 1)
    {
      Serial.println("Connecting to network...");
      delay(1000);
    } 
  Serial.print("server is at ");
  Serial.println(server.localIP());
}

 boolean currentLineIsBlank = false;
void loop() {
  // listen for incoming
  // an http request ends with a blank line
 

  if (server.available()) {
    char c = server.read();
    Serial.write(c);
    // if you've gotten to the end of the line (received a newline
    // character) and the line is blank, the http request has ended,
    // so you can send a reply
    if (c == '\n' && currentLineIsBlank) {
      // send a standard http response header
      server.println("HTTP/1.1 200 OK");
      server.println("Content-Type: text/html");
      server.println("Connection: close");  // the connection will be closed after completion of the response
      server.println("Refresh: 5");  // refresh the page automatically every 5 sec
      server.println();
      server.println("<!DOCTYPE HTML>");
      server.println("<html>");
      // output the value of each analog input pin
      for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
        int sensorReading = analogRead(analogChannel);
        server.print("analog input ");
        server.print(analogChannel);
        server.print(" is ");
        server.print(sensorReading);
        server.println("<br />");
      }
      server.println("</html>");
      server.println("");
      server.println("");
      currentLineIsBlank = false;

    }
    else if (c == '\n') {
      // you're starting a new line
      currentLineIsBlank = true;
    }
    else if (c != '\r') {
      // you've gotten a character on the current line
      currentLineIsBlank = false;
    }
  }

// give the web browser time to receive the data
delay(1);

  
}
