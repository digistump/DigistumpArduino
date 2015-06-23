/* ATtiny85 as an I2C Master   Ex2        BroHogan                           1/21/11
 * Modified for Digistump/DigiX - Digispark LCD Shield by Erik Kettenburg 11/2012
 */

//#define DEBUG
#include <Wire.h>                  // I2C Master lib for ATTinys which use USI - comment this out to use with standard arduinos
#include <LiquidCrystal_I2C.h>          // for LCD w/ GPIO MODIFIED for the ATtiny85

#define GPIO_ADDR     0x27             // (PCA8574A A0-A2 @5V) typ. A0-A3 Gnd 0x20 / 0x38 for A - 0x27 is the address of the Digispark LCD modules.


LiquidCrystal_I2C lcd(GPIO_ADDR,16,2);  // set address & 16 chars / 2 lines


void setup(){
  Wire1.begin();                    // initialize I2C lib - comment this out to use with standard arduinos
  lcd.init();                           // initialize the lcd 
  lcd.backlight();                      
  lcd.print("DigiX!"); // Print a message to the LCD.
}


void loop(){

}

