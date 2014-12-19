/*
Extensive TWI/I2C EEPROM Library - for 24LCxxx devices
version:        0.4.1
target device:  Microchip 24LC256 or similar
compatibility:  designed with Arduino Due
-> Ver. 0.4.1:  Successfully tested with Arduino Uno R3 and Arduino Micro!
author:         Dennis Schweer (Inglorious Engineer)
license:        CC BY-SA 3.0 (http://creativecommons.org/licenses/by-sa/3.0/deed.en)

Overview:
  -bytewise reading/writing
    void extEEPROMwrite(int chip_address, int address, byte value);
    byte extEEPROMread(int chip_address, int address);
  -pagewise reading/writing
    void extEEPROMwritePage(int chip_address, int startaddress, byte* data_origin_array, int amount_of_transfered_bytes)
      (!!!ATTENTION!!!: Limited to 30 Bytes only!)
    void extEEPROMreadPage(int chip_address, int startaddress, byte* data_target_array, int amount_of_transfered_bytes)
      (!!!ATTENTION!!!: Limited to 32 Bytes only) 
 Do not care about their size, just save them!
  -read/write complete 32bit integers [Requires four bytes of your EEPROM.]
    void extEEPROMwriteInt(int chip_address, int address, int data_to_be_stored)
    int extEEPROMreadInt(int chip_address, int address) 
  -read/write your 10/12 bit sensor values (e.g., ADC values) [Requires two bytes of your EEPROM.]
    void extEEPROMwriteSensor(int chip_address, int addresse, int data_to_be_stored)
    int extEEPROMreadSensor(int chip_address, int addresse)
    
NEW IN VERSION 0.4:
Now all functions include a device address parameter, allowing you to use two 
or more external EEPROM chips simultaneously on a single bus. Just choose the 
right device addresses and feet them into the library functions!

NEW IN VERSION 0.4.1:
My library is designed with an Arduino Due, but now everything is successfully
tested to work on Arduino Uno R3 and Arduino Micro as well! Since all current 
Arduinos are based on either ATmega328 (e.g., Uno), ATmega32U4 (e.g., Micro/
Leonardo/Esplora) or ATSAM3X8E (Due), my library should work with ALL official
or 1:1-compatible boards.
!!! Unfortunately, Arduino Due appears to be the only device with the ability
to handle 32 bit integers. Hence none of my "writeInt" / "readInt" functions 
run on 8-bit Arduinos !!!

    
Planned for future releases:
  -erase byte
  -erase page
  -erase complete EEPROM
  -read/write with autocorrection of "startaddress"-value
*/
 
#include "Arduino.h"
//========FUNCTIONS=========================  
  
/////////////////  WRITE  /////////////////////
  
void extEEPROMwrite(int EEPROM_addr, int addr, byte data)
{
  Wire.beginTransmission(EEPROM_addr);  //Start transmission to EEPROM
  Wire.write(highByte(addr));           // send high byte of address  
  Wire.write(lowByte(addr));            // send low byte of address
  Wire.write((byte) data);              // send data
  Wire.endTransmission(true);           // stop transmitting
  delay(6);                             // wait for a successful write
}

void extEEPROMwritePage(int EEPROM_addr, int addr, byte* data_origin, int amount)
{
  Wire.beginTransmission(EEPROM_addr);  //Start transmission to EEPROM
  Wire.write(highByte(addr));           // send high byte of address  
  Wire.write(lowByte(addr));            // send low byte of address  
  for(int i = 0; i<amount; i++)         //write array into EEPROM
  {
    Wire.write((byte) data_origin[i]);  
  }
  Wire.endTransmission(true);           // stop transmitting
  delay(6);                             // wait for a successful write
}

void extEEPROMwriteInt(int EEPROM_addr, int addr, int data)
{
  Wire.beginTransmission(EEPROM_addr);  //Start transmission to EEPROM
  Wire.write(highByte(addr));           // send high byte of address  
  Wire.write(lowByte(addr));            // send low byte of address
  Wire.write(lowByte(data));            // send lowest byte of 32 bit integer
  data = data >> 8;
  Wire.write(lowByte(data));            // send 2nd lowest byte of 32 bit integer
  data = data >> 8;
  Wire.write(lowByte(data));            // send 2nd highest byte of 32 bit integer
  data = data >> 8;
  Wire.write(lowByte(data));            // send highest byte of 32 bit integer
  Wire.endTransmission(true);           // stop transmitting
  delay(6);                             // wait for a successful write
}

void extEEPROMwriteSensor(int EEPROM_addr, int addr, int data)
{
  Wire.beginTransmission(EEPROM_addr);  //Start transmission to EEPROM
  Wire.write(highByte(addr));           // send high byte of address  
  Wire.write(lowByte(addr));            // send low byte of address
  Wire.write(lowByte(data));            // send low byte of 12 bit integer
  data = data >> 8;
  Wire.write(lowByte(data));            // send high byte of 12 bit integer
  Wire.endTransmission(true);           // stop transmitting
  delay(6);                             // wait for a successful write
}

/////////////////  READ  /////////////////////

byte extEEPROMread(int EEPROM_addr, int addr)
{
  Wire.beginTransmission(EEPROM_addr);           //Start transmission to EEPROM
  Wire.write(highByte(addr));                    // send high byte of address  
  Wire.write(lowByte(addr));                     // send low byte of address  
  Wire.endTransmission(true);                    // stop transmitting
  Wire.requestFrom(EEPROM_addr, 0x01, true);     // request 1 byte form the device attached to EEPROM_addr
  byte data_out = 64;
  // read that byte
  while(Wire.available() == 0) {}                // wait for data
  data_out = Wire.read();                        //read single byte
  return data_out;
}  

void extEEPROMreadPage(int EEPROM_addr, int addr, byte* data_target, int amount)
{
  Wire.beginTransmission(EEPROM_addr);            //Start transmission to EEPROM
  Wire.write(highByte(addr));                     // send high byte of address  
  Wire.write(lowByte(addr));                      // send low byte of address  
  Wire.endTransmission(true);                     // stop transmitting
  Wire.requestFrom(EEPROM_addr, amount, true);    // request 1 byte form the device attached to EEPROM_addr
  // read that byte
  while(Wire.available() == 0) {}                 // wait for data
  for(int i = 0; i<amount; i++)                   //write data into array
  {
    data_target[i] = Wire.read();
  }
}  

int extEEPROMreadInt(int EEPROM_addr, int addr)
{
  Wire.beginTransmission(EEPROM_addr);           //Start transmission to EEPROM
  Wire.write(highByte(addr));                    // send high byte of address  
  Wire.write(lowByte(addr));                     // send low byte of address  
  Wire.endTransmission(true);                    // stop transmitting
  Wire.requestFrom(EEPROM_addr, 0x04, true);     // request 1 byte form the device attached to EEPROM_addr
  int data_out = 0xDEADBEEF;
  int temp = 0xDEADBEEF;
  // read that byte
  while(Wire.available() == 0) {}                // wait for data
  data_out = Wire.read();                        //read single byte
  //reconstruct value
  temp = Wire.read();
  temp = temp << 8;
  data_out = data_out | temp;
  temp = Wire.read();
  temp = temp << 16;
  data_out = data_out | temp;
  temp = Wire.read();
  temp = temp << 24;
  data_out = data_out | temp;
  return data_out;
}

int extEEPROMreadSensor(int EEPROM_addr, int addr)
{
  Wire.beginTransmission(EEPROM_addr);           //Start transmission to EEPROM
  Wire.write(highByte(addr));                    // send high byte of address  
  Wire.write(lowByte(addr));                     // send low byte of address  
  Wire.endTransmission(true);                    // stop transmitting
  Wire.requestFrom(EEPROM_addr, 0x04, true);     // request 1 byte form the device attached to EEPROM_addr
  int data_out = 0xDEAD;
  int temp = 0xBEEF;
  // read that byte
  while(Wire.available() == 0) {}                // wait for data
  data_out = Wire.read();                        //read single byte
  //reconstruct value
  temp = Wire.read();
  temp = temp << 8;
  data_out = data_out | temp;
  return data_out;
}
