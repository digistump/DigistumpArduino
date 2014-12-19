// DigiX WiFi module example - released by Digistump LLC/Erik Kettenburg under CC-BY-SA 3.0

#include "DigiFi.h"

#define DEBUG

 bool digiFiDebugState = false;
 uint8_t digiFiMode = TCP;
 bool digiFiServer = false;
 uint32_t digiFiActivityTimeout = 0;

DigiFi::DigiFi()
{

}

/* Stream Implementation */
int DigiFi::available( void )
{
    uint8_t available = Serial1.available();
    if(available>0)
        digiFiActivityTimeout = millis() +1000;
    return available;
}
int DigiFi::peek( void )
{
    return Serial1.peek();
}
int DigiFi::read( void )
{
    return Serial1.read();
}
int DigiFi::read(uint8_t *buf, size_t size)
{
    return Serial1.readBytes((char*)buf,size);
}
void DigiFi::flush( void )
{
    return Serial1.flush();
}
void DigiFi::stop( void )
{
    startATMode();
    setTCPConn("off");
    endATMode();
}
void DigiFi::setFlowControl( boolean en )
{
    Serial1.setCTSPin(DIGIFI_CTS);
    Serial1.enableCTS(en);
}
size_t DigiFi::write( const uint8_t c )
{

    digiFiActivityTimeout = millis() + (requestTimeout*1000); 
    return Serial1.write(c);
}
size_t DigiFi::write(const uint8_t *buf, size_t size)
{
    digiFiActivityTimeout = millis() + (requestTimeout*1000); 
    return Serial1.write(buf,size);
}
void DigiFi::closeChunk()
{
    Serial1.println('0');
    Serial1.println();
}

void DigiFi::printChunk(int str)
{
    printChunk(String(str));
}
void DigiFi::printChunk(long str)
{
    printChunk(String(str));
}
void DigiFi::printChunk(const char *str)
{
    printChunk(String(str));
}
void DigiFi::printChunk(String str)
{
    Serial1.println(str.length()+2,HEX);
    Serial1.println(str);
    Serial1.println();
}




DigiFi::operator bool() {
  return Serial1;
}

void DigiFi::begin(int aBaud, bool en)
{
    setFlowControl(en);
    Serial1.begin(aBaud);
    
    /** /
    //Enable USART HW Flow Control
    USART0->US_MR |= US_MR_USART_MODE_HW_HANDSHAKING;
    
    //Disable PIO Control of URTS pin
    PIOB->PIO_ABSR |= (0u << 25);
    PIOB->PIO_PDR |= PIO_PB25A_RTS0;
    
    //Disable PIO Control of UCTS pin
    PIOB->PIO_ABSR |= (0u << 26);
    PIOB->PIO_PDR |= PIO_PB26A_CTS0;
    
    //Disable PIO Control of WRTS pin
    PIOC->PIO_ABSR |= (0u << 27);
    PIOC->PIO_PDR |= (1u << 27);
    
    //Disable PIO Control of WCTS pin
    PIOC->PIO_ABSR |= (0u << 20);
    PIOC->PIO_PDR |= (1u << 20);
    /**/
    while(Serial1.available()){Serial1.read();} 
}
void DigiFi::startATMode()
{
    bool ATsuccess = false;
	// ensure the module properly acknowledges
	// our request for AT mode. Otherwise retry.
	int retries = 0; // TODO: make constant
    do {
	  if (retries > 5) {
	    debug("Retried 5 times, bailing");
		// need to change return-types perhaps to
		// trigger some kind of reset higher up.
		return;
	  }
	  ATsuccess = startATSequence();
	  retries += 1;
	} while (!ATsuccess);
	debug("Send client acknowledge AT mode");
    Serial1.print("a"); 
    debug(readResponse(0));
    debug("echo off");
    Serial1.print("AT+E\r");
    debug(readResponse(0));
}
bool DigiFi::startATSequence(){
    //silly init sequence for wifi module
    delay(50); // changed from 100
	// clear the incoming buffer
    while(Serial1.available()){Serial1.read();} 
    debug("start at mode");
    debug("next");
    Serial1.write("+++");
    debug("wait for a");
	// there's a ~4s (see datasheet for newer G2 module)
	// time within which the handshake must complete
	// if time is longer than that, it's failed.
	// TODO: turn the timeout into a constant
	unsigned long timeout = millis() + (4*1000);
    while(!Serial1.available()){
	  delay(1);
      if (millis() > timeout) {
	    debug("FAILED: AT handshake timeout");
	    return false;
	  }
	}
    debug("check for a");
    char resp = Serial1.read();
	if (resp == 'a') {
	  debug("OK: module acknowledge AT mode");
	  return true;
	}
    // otherwise it's failed.
	debug("FAILED: module acknowledge AT mode");
	return false;
}
void DigiFi::endATMode()
{
    //back to transparent mode
    Serial1.print("AT+E\r");
    debug(readResponse(0)); 
    Serial1.print("AT+ENTM\r");
    debug(readResponse(0));
    debug("exit at mode");
}
 bool DigiFi::ready(){
    startATMode();
    //debug("send cmd");
    //+ok=<ret><CR>< LF ><CR>< LF >
    //”Disconnected”, if no WiFi connection;
    //”AP’ SSID（AP’s MAC” ）, if WiFi connection available;
    //”RF Off”, if WiFi OFF;
    debug("Check Link");
    String ret = STALinkStatus();
    debug("OUT");
    debug(ret);
    endATMode();
    debug(ret);
    //change this to report the AP it is connected to
    if(ret.substring(0,10) == "+ok=RF Off" || ret.substring(0,16) == "+ok=Disconnected")
        return 0;
    else
        return 1;
}

uint8_t DigiFi::maintain() {return 0;}


IPAddress DigiFi::localIP(){

  startATMode();
  String response = getSTANetwork();
  endATMode(); 
   response = response.substring(response.indexOf(",")+1);
  response = response.substring(0,response.indexOf(","));
  String ip1 = response.substring(0,response.indexOf("."));
  String ip2 = response.substring(response.indexOf(".")+1);
  String ip3 = ip2.substring(ip2.indexOf(".")+1);
  String ip4 = ip3.substring(ip3.indexOf(".")+1);
  ip2 = ip2.substring(0,ip2.indexOf("."));
  ip3 = ip3.substring(0,ip3.indexOf("."));
  IPAddress ip(ip1.toInt(),ip2.toInt(),ip3.toInt(),ip4.toInt());
  return ip;
}

IPAddress DigiFi::subnetMask(){

  startATMode();
  String response = getSTANetwork();
  endATMode();
    response = response.substring(response.indexOf(",")+1);
  response = response.substring(response.indexOf(",")+1);
  response = response.substring(0,response.indexOf(","));
  String ip1 = response.substring(0,response.indexOf("."));
  String ip2 = response.substring(response.indexOf(".")+1);
  String ip3 = ip2.substring(ip2.indexOf(".")+1);
  String ip4 = ip3.substring(ip3.indexOf(".")+1);
  ip2 = ip2.substring(0,ip2.indexOf("."));
  ip3 = ip3.substring(0,ip3.indexOf("."));
  IPAddress ip(ip1.toInt(),ip2.toInt(),ip3.toInt(),ip4.toInt());
  return ip;
}

IPAddress DigiFi::gatewayIP(){

  startATMode();
  String response = getSTANetwork();
  endATMode();
    response = response.substring(response.indexOf(",")+1);
  response = response.substring(response.indexOf(",")+1);
  response = response.substring(response.indexOf(",")+1);
  response = response.substring(0,response.indexOf("\r"));
  String ip1 = response.substring(0,response.indexOf("."));
  String ip2 = response.substring(response.indexOf(".")+1);
  String ip3 = ip2.substring(ip2.indexOf(".")+1);
  String ip4 = ip3.substring(ip3.indexOf(".")+1);
  ip2 = ip2.substring(0,ip2.indexOf("."));
  ip3 = ip3.substring(0,ip3.indexOf("."));
   IPAddress ip(ip1.toInt(),ip2.toInt(),ip3.toInt(),ip4.toInt());
   return ip;
}

IPAddress DigiFi::dnsServerIP(){

  startATMode();
  String response = getSTADNS();
  endATMode();
  response = response.substring(4,response.indexOf("\r"));
  String ip1 = response.substring(0,response.indexOf("."));
  String ip2 = response.substring(response.indexOf(".")+1);
  String ip3 = ip2.substring(ip2.indexOf(".")+1);
  String ip4 = ip3.substring(ip3.indexOf(".")+1);
  ip2 = ip2.substring(0,ip2.indexOf("."));
  ip3 = ip3.substring(0,ip3.indexOf("."));
  IPAddress ip(ip1.toInt(),ip2.toInt(),ip3.toInt(),ip4.toInt());
  return ip;
}

//server functions

String DigiFi::server(uint16_t port){
  startATMode();

  while(Serial1.available()){Serial1.read();}
  String conn=getNetParams();

  String isServer = conn.substring(conn.indexOf("+ok"),conn.length());

  isServer = isServer.substring(8,14);

  setNetParams("TCP","SERVER",port,"127.0.0.1");
  //setTCPConn("On"); //is this needed?

    if(isServer != "Server"){
  
        debug("restart for switch to server mode");
        reset();
        delay(3000);
        

        uint32_t startTime = millis();
        while(!ready() && millis()-startTime < 30000){
            delay(1000);
        }
        startATMode();

    }
  String response = getSTANetwork();
  response = response.substring(response.indexOf(",")+1);
  response = response.substring(0,response.indexOf(","));
  endATMode();
  return response;
}

bool DigiFi::serverRequest(){
  if(Serial1.available()){
    String response = readResponse(0);
    response = response.substring(4);
    response = response.substring(0,response.indexOf("\n"));
    response = response.substring(0,response.lastIndexOf("HTTP/")-1);
    debug(response);
    serverRequestPathString = response;
    return true;
  }
  else
    return false;
}

String DigiFi::serverRequestPath(){
    return serverRequestPathString;

}
void DigiFi::serverResponse(String response, int code) //defaults to code = 200
{
    Serial1.print("HTTP/1.1 ");
    Serial1.print(code);
    if(code==200)
        Serial1.print(" OK");
    else if(code==404)
        Serial1.print(" Not Found");
    else
        Serial1.print(" OK"); //left as OK to not mess anything up
    Serial1.print(" \r\n");
    Serial1.print("Content-Type: text/html;\r\n");
    Serial1.print("Content-Length: ");
    Serial1.print(response.length());
    Serial1.print("\r\n");
    Serial1.print("Connection: close\r\n\r\n");
    Serial1.print(response);
    Serial1.print("\r\n\r\n");

    return;
}

void DigiFi::setTCPTimeout(uint16_t timeout){
    startATMode();
    Serial1.print("AT+TCPTO=");
    Serial1.print(timeout);
    Serial1.print("\r");
    endATMode();

}

uint8_t DigiFi::connected(){

    uint8_t ret = 0;

    
    if(Serial1.available() > 0)
            return 1;
    
    if(millis() < digiFiActivityTimeout)
        return 1;
    

    startATMode();

    debug("Checking for link build up");
    String status=getTCPLnk();

    if (status.substring(0,6)=="+ok=on")
        ret = 1;

    endATMode();

    

    return ret;

}


//client functions

int DigiFi::connect(IPAddress ip, uint16_t port = 80){
    //uint8_t* server = rawIPAddress(ip);
    String server = String(ip[0]) + "." + String(ip[1])+ "." + String(ip[2])+ "." + String(ip[3]);
    return connect(server.c_str(),port);
}
int DigiFi::connect(const char *host, uint16_t port = 80){
    debug("::connect(*char host, uint port)");
    uint8_t lastMode = TCP;
    debug("Connect");
    startATMode();
    debug("send client settings");
    setTCPConn("off");
    //assuming port 80 for now
    String conn=getNetParams();
    String isServer = conn.substring(8,14);

    if(conn.substring(4,7)=="UDP")
        lastMode = UDP;

    debug(conn.substring(4,7));
    conn=conn.substring(conn.lastIndexOf(',')+1,conn.length()-1);
    debug(conn);
    debug(host);
    
    debug(isServer);
    if(conn != host || isServer == "Server" || lastMode != digiFiMode){
        if(digiFiMode == TCP)
            setNetParams("TCP","CLIENT",port,host);
        else
            setNetParams("UDP","CLIENT",port,host);

        debug("setting net params");
    }
    else{
        debug("skipping net params");
    }

    //lastHost = conn;
    
    if(isServer == "Server" || lastMode != digiFiMode){
        debug("restart for switch to client mode");
        reset();
        delay(3000);
        startATMode();
        setTCPConn("off");
    }

    setTCPConn("On");
    uint32_t linkStart = millis();
    if(digiFiMode == TCP){
        
        getNetParams();
        
        debug("Checking for link build up");
        String status=getTCPLnk();
        while(status.substring(0,6)!="+ok=on"){
            debug("Status:");
            debug(status);
            debug("Re-checking for link build up");
            status=getTCPLnk();
            debug(status);
            if(millis()-linkStart > (requestTimeout*1000)){
                endATMode();
                return 0;
            }
        }
    }
    else{
        debug("Checking for host ready");
        String status = ping((char*)host);
        if(status.substring(0,11)!="+ok=Success"){
            while(status.substring(0,11)!="+ok=Success"){
                debug("Re-checking for host ready");
                status=ping((char*)host);
                debug(status);
                if(millis()-linkStart > (requestTimeout*1000)){
                    endATMode();
                    return 0;
                }
            }
            debug("Wait for UDP to be ready to receive as well");
            delay(2000);
        }

    }

    endATMode();
    
    return 1;
}

int DigiFi::disconnect() {
    debug("::disconnect(*char host, uint port)");
    startATMode();
    setTCPConn("off");
	endATMode();
	return 1;
}
String DigiFi::body(){
    return aBody;
}
String DigiFi::header(){
    return aHeader;
}
void DigiFi::setDebug(bool debugStateVar){
    digiFiDebugState = debugStateVar; 
}
void DigiFi::setMode(uint8_t protocol){
    digiFiMode = protocol; 
}
void DigiFi::debug(String output){
    if(digiFiDebugState == true)
        Serial.println(output);
    
}
void DigiFi::debugWrite(char output){
    if(digiFiDebugState == true)
        Serial.write(output);
    
}
/*
Return value should be the HTTP return code (i.e. 100 and above).
If something else fails, the non-HTTP error codes are negative numbers.
-1 - connect failure
-2 - connect successful, but request failed
-3 - invalid HTTP return-code returned
*/
int DigiFi::get(char *aHost, char *aPath){
    if(connect(aHost) == 1){
        //delay(500);
        Serial1.print("GET ");
        Serial1.print(aPath);
        Serial1.print(" HTTP/1.1\r\nHost: ");
        Serial1.print(aHost);
        Serial1.print("\r\nCache-Control: no-cache\r\nConnection: close\r\n\r\n");
        Serial1.flush();

        //don't block while awating reply
        debug("wait for response...");
        bool success = true;
        int i=0;
        int st = millis();
        while(!Serial1.available()){
            if(millis() - st > requestTimeout * 1000) {
                success = false; 
                break;
            } 
            if(((millis() - st) % 1000) == 1)
                debugWrite('.');
            i++; 
        }
        debug("get header");
        if(success == false)
            return -2;
        aHeader = readResponse(0);
        debug(aHeader);

        String contentLength = aHeader.substring(aHeader.lastIndexOf("Content-Length: "));
        contentLength = contentLength.substring(16,contentLength.indexOf("\r"));
        debug("Length:"+contentLength+";");

		if(contentLength.toInt() != 0) {
			debug("get body for later");
            aBody = readResponse(contentLength.toInt());
		}
		else
		{
			debug("Skip body");
		}
        debug("return from get");
		
		// work out the returncode
		int iRetCode = aHeader.substring(9,12).toInt();
		if (iRetCode == 0) {
			debug("Invalid return code");
			return -3;
		}
        return iRetCode;
    }
    else
        return -1;

    //To do:
    /*
    User agent!
    Better handle timeouts/other errors
    Efficiency!
    */
}
String DigiFi::URLEncode(String smsg)
{
    const char *msg = smsg.c_str();
    const char *hex = "0123456789abcdef";
    String encodedMsg = "";

    while (*msg!='\0'){
        if( ('a' <= *msg && *msg <= 'z')
                || ('A' <= *msg && *msg <= 'Z')
                || ('0' <= *msg && *msg <= '9') ) {
            encodedMsg += *msg;
        } else {
            encodedMsg += '%';
            encodedMsg += hex[*msg >> 4];
            encodedMsg += hex[*msg & 15];
        }
        msg++;
    }
    return encodedMsg;
}
/*
Return value should be the HTTP return code (i.e. 100 and above).
If something else fails, the non-HTTP error codes are negative numbers.
-1 - connect failure
-2 - connect successful, but request failed
-3 - invalid HTTP return-code returned
*/
int DigiFi::post(char *aHost, char *aPath, String postData) {
    if(connect(aHost) == 1){
        Serial1.print("POST ");
        Serial1.print(aPath);
        Serial1.print(" HTTP/1.1\r\nHost: ");
        Serial1.print(aHost);
        Serial1.print("\r\nCache-Control: no-cache\r\nContent-Type: application/x-www-form-urlencoded\r\nConnection: close\r\n");
        Serial1.print("Content-Length: ");
        Serial1.print(postData.length());
        Serial1.print("\r\n\r\n");
        Serial1.print(postData);
        Serial1.print("\r\n\r\n");
        Serial1.flush();

        debug("wait for response...");
        bool success = true;
        int i=0;
        int st = millis();
        while(!Serial1.available()){
            if(millis() - st > requestTimeout * 1000) {
                success = false; 
                break;
            } 
            if(((millis() - st) % 1000) == 1)
                debugWrite('.');
            i++; 
        }
        
        if(success == false)
            return -2;

        debug("Get header");
        aHeader = readResponse(0);

        debug(aHeader);

        String contentLength = aHeader.substring(aHeader.lastIndexOf("Content-Length: "));
        contentLength = contentLength.substring(16,contentLength.indexOf("\n"));
        debug(contentLength);
		
		if(contentLength.toInt() != 0) {
			debug("Get body");
            aBody = readResponse(contentLength.toInt());
		}
		else
		{
			debug("Skip body");
		}
        
		// connection: close hard-coded, so disconnect here.
		disconnect();
		// TODO: 
		// + make connection: close header optional
		//   and run disconnect dependent on that option
		// + remove the disconnect command (TCPDIS=off)
		//   from the start of the connect command.
		//   but need to have connection checking upfront
		//   first.
		
		// work out the returncode
		int iRetCode = aHeader.substring(9,12).toInt();
		if (iRetCode == 0) {
			debug("Invalid return code");
			return -3;
		}
        return iRetCode;
    }
    else
        return -1;

    //To do:
    /*
    User agent!
    accept post data as array or array or string, etc
    Better handle timeouts/other errors
    Efficiency!
    */

}
void DigiFi::close()
{
    //clear buffer
    while(Serial1.available()){Serial1.read();}
    Serial1.end();
}
String DigiFi::readResponse(int contentLength) //0 = cmd, 1 = header, 2=body
{
    String stringBuffer;
    char inByte;
    int rCount = 0;
    int nCount = 0;
    int curLength = 0;
    bool end = false;
    Serial1.flush();
	bool timeout = false;
	int st = millis();

    while (!end)
    {
        //look for this to be four bytes in a row
        if (Serial1.available())
        {
            inByte = Serial1.read();
            curLength++;
            //debugWrite(inByte);// disabled, leads to lots of duplicate debug logging

            if(contentLength == 0){
                if (inByte == '\n' && rCount == 2 && nCount == 1)
                {
                    end = true;
                    int strLength = stringBuffer.length()-3;
                    stringBuffer = stringBuffer.substring(0,strLength);
                }
                else if (inByte == '\r')
                    rCount++;
                else if (inByte == '\n')
                    nCount++;
                else{
                    rCount = 0;
                    nCount = 0;
                }
            }
            else if(curLength>=contentLength)
                end = true;
            
            stringBuffer += inByte;
        }
		else
		{
			// need a timeout otherwise we can get stuck in 
			// here if the server drops out for some reason
			// does though imply that 15s is sufficient to
			// retrieve whatever it is your after
			// seems OK though as DigiX doesn't have a large
			// amount of memory
			if(millis() - st > requestTimeout * 1000) {
				timeout = true;
				break;
			}
		}
    }

    if(stringBuffer.substring(0,4) == "+ERR") {
        lastErr = stringBuffer.substring(5,2).toInt();
	} else if (timeout) {
		lastErr = -1;
	} else {
        lastErr = 0;
	}
    return stringBuffer;
}
int DigiFi::lastError()
{
    return lastErr;
}
String DigiFi::AT(char *cmd, char *params)
{
    Serial1.print("AT+");
    Serial1.print(cmd);
    if(sizeof(*params) > 0)
    {
        Serial1.print("=");
        Serial1.print(params);
    }
    Serial1.print("\r");
    return readResponse(0);
}
void DigiFi::toggleEcho() //E
{
    Serial1.print("AT+E\r");
    readResponse(0);
}
String DigiFi::getWifiMode() //WMODE AP STA APSTA
{
    Serial1.print("AT+WMODE\r");
    return readResponse(0);
}
void DigiFi::setWifiMode(char *mode)
{
    Serial1.print("AT+WMODE=");
    Serial1.print(mode);
    Serial1.print("\r");
    readResponse(0);
}
void DigiFi::setTransparent() //ENTM
{
    Serial1.print("AT+ENTM\r");
    readResponse(0);
}
String DigiFi::getTMode() //TMODE throughput cmd
{
    Serial1.print("AT+TMODE\r");
    return readResponse(0);
}
void DigiFi::setTMode(char *mode)
{
    Serial1.print("AT+TMODE=");
    Serial1.print(mode);
    Serial1.print("\r");
    readResponse(0);
}
String DigiFi::getModId() //MID
{
    Serial1.print("AT+MID\r");
    return readResponse(0);
}
String DigiFi::version() //VER
{
    Serial1.print("AT+VER\r");
    return readResponse(0);
}
void DigiFi::factoryRestore() //RELD rebooting...
{
    Serial1.print("AT+RELD\r");
    readResponse(0);
}
void DigiFi::reset() //Z (No return)
{
    Serial1.print("AT+Z\r");
    //readResponse(0);
    lastErr=0; //This command doesnt return anything.
}
String DigiFi::help()//H
{
    Serial1.print("AT+H\r");
    return readResponse(0);
}
int DigiFi::readConfig(byte* buffer)//CFGRD
{
    Serial1.print("AT+CFGRD\r");
    Serial1.readBytes((char*)buffer,4);
    if((char*)buffer=="+ERR")
        return -1; //TODO Set lastErr here (Technically it shouldn't ever error here)
    Serial1.readBytes((char*)buffer,2);
    int len=(int)word(buffer[1],buffer[0]);
    Serial1.readBytes((char*)buffer,len);
    return len;
}
void DigiFi::writeConfig(byte* config, int len)//CFGWR
{
    Serial1.print("AT+CFGWR=");
    Serial1.write(highByte(len));
    Serial1.write(lowByte(len));
    Serial1.write(config,len);
    Serial1.print("\r");
    readResponse(0);
}
int DigiFi::readFactoryDef(byte* buffer)//CFGFR
{
    Serial1.print("AT+CFGFR\r");
    Serial1.readBytes((char*)buffer,4);
    if((char*)buffer=="+ERR")
        return -1; //TODO Set lastErr here (Technically it shouldn't ever error here)
    Serial1.readBytes((char*)buffer,2);
    int len=(int)word(buffer[1],buffer[0]);
    Serial1.readBytes((char*)buffer,len);
    return len;
}
void DigiFi::makeFactory() //CFGTF
{
    Serial1.print("AT+CFGTF\r");
    readResponse(0);
}
String DigiFi::getUart()//UART baudrate,data_bits,stop_bit,parity
{
    Serial1.print("AT+UART\r");
    return readResponse(0);
}
void DigiFi::setUart(int baudrate,int data_bits,int stop_bit,char *parity)
{
    Serial1.print("AT+UART=");
    Serial1.print(baudrate);
    Serial1.print(",");
    Serial1.print(data_bits);
    Serial1.print(",");
    Serial1.print(stop_bit);
    Serial1.print(",");
    Serial1.print(parity);
    Serial1.print("\r");
    readResponse(0);
}
/*
String getAutoFrame(); //UARTF
void setAutoFrame(char *para);
int getAutoFrmTrigTime(); //UARTFT
void setAutoFrmTrigTime(int ms);
int getAutoFrmTrigLength(); //UARTFL
void setAutoFrmTrigLength(int v);
*/
void DigiFi::sendData(int len, char *data)//SEND
{
    Serial1.print("AT+SEND=");
    Serial1.print(len);
    Serial1.print(",");
    Serial1.print(data);
    Serial1.print("\r");
    readResponse(0);
}
String DigiFi::recvData(int len)//RECV len,data (+ok=0 if timeout (3sec))
{
    Serial1.print("AT+RECV=");
    Serial1.print(len);
    Serial1.print("\r");
    return readResponse(0);
}
String DigiFi::ping(char *ip)//PING Success Timeout Unknown host
{
    Serial1.print("AT+PING=");
    Serial1.print(ip);
    Serial1.print("\r");
    return readResponse(0);
}
String DigiFi::getNetParams()//NETP (TCP|UDP),(SERVER|CLIENT),port,IP 
{
    Serial1.print("AT+NETP\r");
    return readResponse(0);
}
void DigiFi::setNetParams(char *proto, char *cs, int port, const char *ip)
{
    if(cs == "SERVER")
        digiFiServer = true;
    else
        digiFiServer = false;

    Serial1.print("AT+NETP=");
    Serial1.print(proto);
    Serial1.print(",");
    Serial1.print(cs);
    Serial1.print(",");
    Serial1.print(port);
    Serial1.print(",");
    Serial1.print(ip);
    Serial1.print("\r");
    readResponse(0);
}
String DigiFi::getTCPLnk()//TCPLK on|off 
{
    Serial1.print("AT+TCPLK\r");
    return readResponse(0);
}
String DigiFi::getTCPTimeout()//TCPTO 0 <= int <= 600 (Def 300)
{
    Serial1.print("AT+TCPTO\r");
    return readResponse(0);
}
String DigiFi::getTCPConn()//TCPDIS On|off
{
    Serial1.print("AT+TCPDIS\r");
    return readResponse(0);
}
void DigiFi::setTCPConn(char *sta)
{
    Serial1.print("AT+TCPDIS=");
    Serial1.print(sta);
    Serial1.print("\r");
    readResponse(0);
}
String DigiFi::getWSSSID()//WSSSID
{
    Serial1.print("AT+WSSSID\r");
    return readResponse(0);
}
void DigiFi::setWSSSID(char *ssid)
{
    Serial1.print("AT+WSSSID=");
    Serial1.print(ssid);
    Serial1.print("\r");
    readResponse(0);
}
String DigiFi::getSTAKey()//WSKEY (OPEN|SHARED|WPAPSK|WPA2PSK),(NONE|WEP|TKIP|AES),key
{
    Serial1.print("AT+WSKEY\r");
    return readResponse(0);
}
void DigiFi::setSTAKey(char* auth,char *encry,char *key)
{
    Serial1.print("AT+WSKEY=");
    Serial1.print(auth);
    Serial1.print(",");
    Serial1.print(encry);
    Serial1.print(",");
    Serial1.print(key);
    Serial1.print("\r");
    readResponse(0);
}
String DigiFi::getSTANetwork()//WANN (static|DHCP),ip,subnet,gateway
{
    Serial1.print("AT+WANN\r");
    return readResponse(0);
}
void DigiFi::setSTANetwork(char *mode, char *ip, char *subnet, char *gateway)
{
    Serial1.print("AT+WANN=");
    Serial1.print(mode);
    Serial1.print(",");
    Serial1.print(ip);
    Serial1.print(",");
    Serial1.print(subnet);
    Serial1.print(",");
    Serial1.print(gateway);
    Serial1.print("\r");
    readResponse(0);
}
String DigiFi::getSTAMac()//WSMAC returns MAC
{
    Serial1.print("AT+WSMAC\r");
    return readResponse(0);
}
void DigiFi::setSTAMac(int code, char *mac)//Code default is 8888, no idea what its for
{
    Serial1.print("AT+WSSSID=");
    Serial1.print(code);
    Serial1.print(",");
    Serial1.print(mac);
    Serial1.print("\r");
    readResponse(0);
}
String DigiFi::STALinkStatus()//WSLK (Disconnected|AP SSID (AP MAC)|RF Off)
{
    Serial1.print("AT+WSLK\r");
    return readResponse(0);
}
String DigiFi::STASignalStrength()//WSLQ (Disconnected|Value)
{
    Serial1.print("AT+WSLQ\r");
    return readResponse(0);
}
String DigiFi::scan()//WSCAN returns list
{
    Serial1.print("AT+WSCAN\r");
    return readResponse(0);
}
String DigiFi::getSTADNS()//WSDNS address
{
    Serial1.print("AT+WSDNS\r");
    return readResponse(0);
}
void DigiFi::setSTADNS(char *dns)
{
    Serial1.print("AT+WSDNS=");
    Serial1.print(dns);
    Serial1.print("\r");
    readResponse(0);
}
String DigiFi::getAPNetwork()//LANN ip,subnet
{
    Serial1.print("AT+LANN\r");
    return readResponse(0);
}
void DigiFi::setAPNetwork(char *ip, char *subnet)
{
    Serial1.print("AT+LANN=");
    Serial1.print(ip);
    Serial1.print(",");
    Serial1.print(subnet);
    Serial1.print("\r");
    readResponse(0);
}
String DigiFi::getAPParams()//WAP (11B|11BG|11BGN),SSID,(AUTO|C1...C11)
{
    Serial1.print("AT+WAP\r");
    return readResponse(0);
}
void DigiFi::setAPParams(char *mode, char *ssid, char *channel)
{
    Serial1.print("AT+WAP=");
    Serial1.print(mode);
    Serial1.print(",");
    Serial1.print(ssid);
    Serial1.print(",");
    Serial1.print(channel);
    Serial1.print("\r");
    readResponse(0);
}
String DigiFi::getAPKey()//WAKEY (OPEN|WPA2PSK),(NONE|AES),key
{
    Serial1.print("AT+WAKEY\r");
    return readResponse(0);
}
void DigiFi::setAPKey(char* auth,char *encry,char *key)
{
    Serial1.print("AT+WAKEY=");
    Serial1.print(auth);
    Serial1.print(",");
    Serial1.print(encry);
    Serial1.print(",");
    Serial1.print(key);
    Serial1.print("\r");
    readResponse(0);
}
String DigiFi::getAPMac()//WAMAC returns MAC
{
    Serial1.print("AT+WAMAC\r");
    return readResponse(0);
}
String DigiFi::getAPDHCP()//WADHCP (on|off)
{
    Serial1.print("AT+WADHCP\r");
    return readResponse(0);
}
void DigiFi::setAPDHCP(char *status)
{
    Serial1.print("AT+WADHCP=");
    Serial1.print(status);
    Serial1.print("\r");
    readResponse(0);
}
String DigiFi::getAPPageDomain()//WADMN domain
{
    Serial1.print("AT+WADM\r");
    return readResponse(0);
}
void DigiFi::setAPPageDomain(char *domain)
{
    Serial1.print("AT+WADMN=");
    Serial1.print(domain);
    Serial1.print("\r");
    readResponse(0);
}
void DigiFi::setPageDisplayMode(char *mode)//WEBSWITCH (iw|ew)
{
    Serial1.print("AT+WEBSWITCH=");
    Serial1.print(mode);
    Serial1.print("\r");
    readResponse(0);
}
void DigiFi::setPageLanguage(char *lang)//PLANG CN|EN
{
    Serial1.print("AT+PLANG=");
    Serial1.print(lang);
    Serial1.print("\r");
    readResponse(0);
}
String DigiFi::getUpgradeUrl()//UPURL url !!!DANGEROUS!!!
{
    Serial1.print("AT+UPURL\r");
    return readResponse(0);
}
void DigiFi::setUpgradeUrl(char *url)//url,filename (filename is optional, if provided upgrade is auto started)
{
    Serial1.print("AT+UPURL=");
    Serial1.print(url);
    Serial1.print("\r");
    readResponse(0);
}
String DigiFi::getUpgradeFile()//UPFILE filename !!!DANGEROUS!!!
{
    Serial1.print("AT+UPFILE\r");
    return readResponse(0);
}
void DigiFi::setUpgradeFile(char *filename)
{
    Serial1.print("AT+UPFILE=");
    Serial1.print(filename);
    Serial1.print("\r");
    readResponse(0);
}
String DigiFi::startUpgrade()//UPST !!!DANGEROUS!!!
{
    Serial1.print("AT+UPST\r");
    return readResponse(0);
}
String DigiFi::getWebAuth()//WEBU user,pass
{
    Serial1.print("AT+WEBU\r");
    return readResponse(0);
}
void DigiFi::setWebAuth(char *user, char *pass)
{
    Serial1.print("AT+WEBU=");
    Serial1.print(user);
    Serial1.print(",");
    Serial1.print(pass);
    Serial1.print("\r");
    readResponse(0);
}
String DigiFi::getSleepMode()//MSLP normal|standby
{
    Serial1.print("AT+MSLP\r");
    return readResponse(0);
}
void DigiFi::setSleepMode(char *mode)
{
    Serial1.print("AT+MSLP=");
    Serial1.print(mode);
    Serial1.print("\r");
    readResponse(0);
}
void DigiFi::setModId(char *modid)//WRMID
{
    Serial1.print("AT+WRMID=");
    Serial1.print(modid);
    Serial1.print("\r");
    readResponse(0);
}
String DigiFi::getWifiCfgPassword()//ASWD aswd
{
    Serial1.print("AT+ASWD\r");
    return readResponse(0);
}
void DigiFi::setWifiCfgPassword(char *aswd)
{
    Serial1.print("AT+ASWD=");
    Serial1.print(aswd);
    Serial1.print("\r");
    readResponse(0);
}
