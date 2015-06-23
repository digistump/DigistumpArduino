/*

 Udp NTP Client

 Uses both Due_RTC library and DigiFi to connect to NTP server
 get current time and set RTC to that time.

 This code is in the public domain.

 */
 #include <rtc_clock.h>
#include <DigiFi.h>

RTC_clock rtc_clock(XTAL);

char* daynames[]={"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
int hh,mm,ss,dow,dd,mon,yyyy;

char timeServer[] = "time.nist.gov"; // time.nist.gov NTP server

const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message

uint8_t packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets

DigiFi client;

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  
//DigiX trick - since we are on serial over USB wait for character to be entered in serial terminal
  while(!Serial.available()){
    Serial.println("Enter any key to begin");
    delay(1000);
  }
    rtc_clock.init();
    // start the connection:
   client.begin(9600);
   //client.setDebug(true); 
  //wait for module to be ready
  while (client.ready() != 1)
  {
    Serial.println("Connecting to network...");
    delay(1000);
  } 
  client.setMode(UDP); //must come before connect
  rtc_clock.set_time(__TIME__);
  Serial.println("Setting up UDP connection");
  client.connect(timeServer,123);
  unsigned long ntpUnixTime = 0;
  while(ntpUnixTime == 0){
    sendNTPpacket(); // send an NTP packet to a time server
    delay(1000);
    ntpUnixTime = getNTPpacket();
  }
  Serial.print("Got NTP Timestamp: ");
  Serial.println(ntpUnixTime);
  Serial.println("Setting RTC Clock");
  rtc_clock.set_timestamp(ntpUnixTime);


}

void loop()
{
  Serial.print("Time: ");
  rtc_clock.get_time(&hh,&mm,&ss);
  rtc_clock.get_date(&dow,&dd,&mon,&yyyy);
  digitprint(hh, 2);
  Serial.print(":");
  digitprint(mm, 2);
  Serial.print(":");
  digitprint(ss, 2);
  Serial.println("");
  Serial.print("Date: ");
  Serial.print(daynames[dow-1]);
  Serial.print(" ");
  digitprint(dd, 2);
  Serial.print(".");
  digitprint(mon, 2);
  Serial.print(".");
  Serial.println(yyyy);
  Serial.println("");
  delay(1000);
}

// send an NTP request to the time server at the given address
unsigned long sendNTPpacket()
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  client.write(packetBuffer, NTP_PACKET_SIZE);

}

unsigned long getNTPpacket(){
  if ( client.available() ) {
    // We've received a packet, read the data from it
    client.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

    //the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, esxtract the two words:

    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    //Serial.print("Seconds since Jan 1 1900 = " );
    //Serial.println(secsSince1900);

    // now convert NTP time into everyday time:
    //Serial.print("Unix time = ");
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
    // subtract seventy years:
    unsigned long epoch = secsSince1900 - seventyYears;
    // print Unix time:
    return epoch;
  }
  else{
    return 0;
  }
}

void digitprint(int value, int lenght){
  for (int i = 0; i < (lenght - numdigits(value)); i++){
    Serial.print("0");
  }
  Serial.print(value);
}

int numdigits(int i){
  int digits;
  if (i < 10)
    digits = 1;
  else
    digits = (int)(log10((double)i)) + 1;
  return digits;
}
