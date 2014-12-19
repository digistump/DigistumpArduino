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
#include <Wire.h>
#include <Extensive_EEPROM.h>

const int EEPROM_addr = 0x50;

// Testbed variables
int test = 1;

void setup()
{
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(9600);
  test = 1;
}

void loop()
{
  //Testbed
  if(test == 1) //only run it once
  {
     //byte-wise writing/reading
     Serial.println("//byte-wise writing/reading");
     for(int i = 0; i < 32; i++)
     { 
        extEEPROMwrite(EEPROM_addr, i, i); //void extEEPROMwrite(int chip_address, int address, byte value);
     }
     int data_back;
     for(int i = 0; i < 32; i++)
     {
        data_back = extEEPROMread(EEPROM_addr, i); //byte extEEPROMread(int chip_address, int address); 
        Serial.print("original data= ");
        Serial.print(i);
        Serial.print(" read_back= ");
        Serial.println(data_back);
     }
     
     
     //page-wise writing/reading
     Serial.println("//page-wise writing/reading");
     byte data_to_be_written[30];
     for(int i=0; i<30; i++)
     {
       data_to_be_written[i] = (29-i);   //writes the numbers 29 downto 0 into cells 0-29
       Serial.print("Original Data = ");
       Serial.println((29-i));
     }
     //store array in EEPROM (max. 30 Bytes)
     extEEPROMwritePage(EEPROM_addr, 32, data_to_be_written, 30); //void extEEPROMwritePage(int chip_address, int startaddress, byte* data_origin_array, int amount_of_transfered_bytes)
     //read page into an array (max. 32 Bytes)
     byte data_output[30];
     extEEPROMreadPage(EEPROM_addr, 32, data_output, 30); // void extEEPROMreadPage(int chip_address, int startaddress, byte* data_target_array, int amountof_transfered_bytes)
     for(int j=0; j<30; j++)
     {
       Serial.print("Read Page= ");
       Serial.println(data_output[j]);  // Print array
     }
     
     //write/read 32 bit integer
     Serial.println("//write/read 32 bit integer");
     int original_int = 0x7FFFFFFF;
     extEEPROMwriteInt(EEPROM_addr, 70, original_int);
     int original_int_output;
     original_int_output = extEEPROMreadInt(EEPROM_addr, 70);
     Serial.print("Integer: in = ");
     Serial.print(original_int);
     Serial.print(" / out = ");
     Serial.println(original_int_output); 
     
     //write/read 10/12 bit sensor data
     Serial.println("//write/read 10/12 bit sensor data");
     int original_sensor = 0x7FFF;
     extEEPROMwriteInt(EEPROM_addr, 75, original_sensor);
     int original_sensor_output;
     original_sensor_output = extEEPROMreadInt(EEPROM_addr, 75);
     Serial.print("Sensordata: in = ");
     Serial.print(original_sensor);
     Serial.print(" / out = ");
     Serial.println(original_sensor_output);
     
  }
  test = 2; // only perform this procedure once
}
