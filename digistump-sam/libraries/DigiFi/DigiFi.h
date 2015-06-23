// DigiX WiFi module example - released by Digistump LLC/Erik Kettenburg under CC-BY-SA 3.0


#ifndef DigiFi_h
#define DigiFi_h

#include "Arduino.h"    
#include "Print.h"
#include <string.h>
#include "Client.h"
#include "IPAddress.h"

#define DIGIFI_RTS  105
#define DIGIFI_CTS  104
#define TCP  1
#define UDP  0

class DigiFi : public Client
{
    public:
        static const int requestTimeout = 15;
        String serverRequestPathString;
        DigiFi();
        
        void begin(int aBaud = 9600, bool en = false);
        bool ready();
        void setDebug(bool debugStateVar);
        void setTCPTimeout(uint16_t timeout);
        bool serverRequest();
        void serverResponse(String response, int code = 200);
        String server(uint16_t port);
        String serverRequestPath();
        virtual int connect(IPAddress ip, uint16_t port);
        virtual int connect(const char *host, uint16_t port);
		virtual int disconnect();
        int get(char *aHost, char *aPath);
        int post(char *aHost, char *aPath, String postData);
        void startATMode();
        void endATMode();
        void close();
        void closeChunk();
        void printChunk(const char *str);
        void printChunk(int str);
        void printChunk(long str);
        void printChunk(String str);
        void setMode(uint8_t protocol = TCP);
        String header();
        String body();
        int lastError();
        void debug(String output);
        void debugWrite(char output);
        String URLEncode(String smsg);
        void setFlowControl(boolean);

        //Ethernet implimentation
        IPAddress localIP();
        IPAddress subnetMask();
        IPAddress gatewayIP();
        IPAddress dnsServerIP();
        uint8_t maintain();
        
        /* Client Implementation */
        virtual uint8_t connected();
        //uint8_t status();
        virtual operator bool();

        virtual int available( void ) ;
        virtual int peek( void ) ;
        virtual int read( void ) ;
        virtual int read(uint8_t *buf, size_t size);
        virtual void flush( void ) ;
        virtual void stop( void ) ;
        virtual size_t write( const uint8_t c ) ;
        virtual size_t write(const uint8_t *buf, size_t size);
        using Print::write ; // pull in write(str) and write(buf, size) from Print
        
        /* AT Wrappers */
        String AT(char *cmd, char *params);
        void toggleEcho(); //E
        String getWifiMode(); //WMODE AP STA APSTA
        void setWifiMode(char *mode);
        void setTransparent(); //ENTM
        String getTMode(); //TMODE throughput cmd
        void setTMode(char *mode);
        String getModId(); //MID
        String version(); //VER
        void factoryRestore(); //RELD rebooting...
        void reset(); //Z (No return)
        String help();//H
        int readConfig(byte* buffer);//CFGRD
        void writeConfig(byte* config, int len);//CFGWR
        int readFactoryDef(byte* buffer);//CFGFR
        void makeFactory(); //CFGTF
        String getUart();//UART baudrate,data_bits,stop_bit,parity
        void setUart(int baudrate,int data_bits,int stop_bit,char *parity);
        /* These are commented out as I'm unsure how they should be named
        String getAutoFrame(); //UARTF
        void setAutoFrame(char *para);
        int getAutoFrmTrigTime(); //UARTFT
        void setAutoFrmTrigTime(int ms);
        int getAutoFrmTrigLength(); //UARTFL
        void setAutoFrmTrigLength(int v);
        */
        void sendData(int len, char *data);//SEND
        String recvData(int len);//RECV len,data (+ok=0 if timeout (3sec))
        String ping(char *ip);//PING Success Timeout Unknown host
        String getNetParams();//NETP (TCP|UDP),(SERVER|CLIENT),port,IP 
        void setNetParams(char *proto, char *cs, int port, const char *ip);
        String getTCPLnk();//TCPLK on|off 
        String getTCPTimeout();//TCPTO 0 <= int <= 600 (Def 300)
        String getTCPConn();//TCPDIS On|off
        void setTCPConn(char *sta);
        String getWSSSID();//WSSSID
        void setWSSSID(char *ssid);
        String getSTAKey();//WSKEY (OPEN|SHARED|WPAPSK|WPA2PSK),(NONE|WEP|TKIP|AES),key
        void setSTAKey(char* auth,char *encry,char *key);
        String getSTANetwork();//WANN (static|DHCP),ip,subnet,gateway
        void setSTANetwork(char *mode, char *ip, char *subnet, char *gateway);
        String getSTAMac();//WSMAC returns MAC
        void setSTAMac(int code, char *mac);//Code default is 8888, no idea what its for
        String STALinkStatus();//WSLK (Disconnected|AP SSID (AP MAC)|RF Off)
        String STASignalStrength();//WSLQ (Disconnected|Value)
        String scan();//WSCAN returns list
        String getSTADNS();//WSDNS address
        void setSTADNS(char *dns);
        String getAPNetwork();//LANN ip,subnet
        void setAPNetwork(char *ip, char *subnet);
        String getAPParams();//WAP (11B|11BG|11BGN),SSID,(AUTO|C1...C11)
        void setAPParams(char *mode, char *ssid, char *channel);
        String getAPKey();//WAKEY (OPEN|WPA2PSK),(NONE|AES),key
        void setAPKey(char* auth,char *encry,char *key);
        String getAPMac();//WAMAC returns MAC
        String getAPDHCP();//WADHCP (on|off)
        void setAPDHCP(char *status);
        String getAPPageDomain();//WADMN domain
        void setAPPageDomain(char *domain);
        void setPageDisplayMode(char *mode);//WEBSWITCH (iw|ew)
        void setPageLanguage(char *lang);//PLANG CN|EN
        String getUpgradeUrl();//UPURL url !!!DANGEROUS!!!
        void setUpgradeUrl(char *url);//url,filename (filename is optional, if provided upgrade is auto started)
        String getUpgradeFile();//UPFILE filename !!!DANGEROUS!!!
        void setUpgradeFile(char *filename);
        String startUpgrade();//UPST !!!DANGEROUS!!!
        String getWebAuth();//WEBU user,pass
        void setWebAuth(char *user, char *pass);
        String getSleepMode();//MSLP normal|standby
        void setSleepMode(char *mode);
        void setModId(char *modid);//WRMID
        String getWifiCfgPassword();//ASWD aswd
        void setWifiCfgPassword(char *aswd);
    private:
        String readResponse(int contentLength);
		bool startATSequence();
        String aHeader;
        String aBody;
        //String lastHost;
        int lastErr;
        bool debugState;
		
};

#endif