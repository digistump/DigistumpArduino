bool start = true;
String message = "";


#include <DigiFi.h>

DigiFi wifi;

//#define DEBUG
#include <Wire.h>                  // I2C Master lib for ATTinys which use USI - comment this out to use with standard arduinos
#include <LiquidCrystal_I2C.h>          // for LCD w/ GPIO MODIFIED for the ATtiny85

#define GPIO_ADDR     0x27             // (PCA8574A A0-A2 @5V) typ. A0-A3 Gnd 0x20 / 0x38 for A - 0x27 is the address of the Digispark LCD modules.


LiquidCrystal_I2C lcd(GPIO_ADDR,16,2);  // set address & 16 chars / 2 lines



// Require keyboard control library
#include <KeyboardController.h>

// Initialize USB Controller
USBHost usb;

// Attach keyboard controller to USB
KeyboardController keyboard(usb);

// This function intercepts key press
void keyPressed() {

}

// This function intercepts key release
void keyReleased() {

  printKey();
}

void printKey() {
  
  if(start){
    lcd.clear();
    lcd.home();
    start = false;
  }
  
  if(keyboard.getOemKey()==40){//enter pressed
    lcd.noAutoscroll();
    lcd.clear();
    lcd.home();
    lcd.print("Sending tweet...");
    if(wifi.post("api.thingspeak.com","/apps/thingtweet/1/statuses/update","api_key=[YOURTHINGTWEETAPIKEY]&status="+wifi.URLEncode(message))){
      lcd.clear();
      lcd.home();
      lcd.print("Tweet sent!");
    }
    else{
      lcd.clear();
      lcd.home();
      lcd.print("Error sending!");
    }
    delay(3000);
    lcd.clear();
    lcd.home();
    lcd.print("DigiX - Ready!");
    message ="";
    start=true;
    lcd.noAutoscroll();
  }
  else{
    // getKey() returns the ASCII translation of OEM key
    // combined with modifiers.
    
    char nextChar = keyboard.getKey();
    lcd.print(nextChar);
    message += nextChar;
    if(message.length()==16)
      lcd.autoscroll();
  }
  

}



void setup()
{
  Wire1.begin();                    // initialize I2C lib - comment this out to use with standard arduinos
  lcd.init();                           // initialize the lcd 
  lcd.backlight();                      // Print a message to the LCD.
  lcd.print("Starting....");
  delay(5000); //give wifi some time to warm up
  lcd.clear();
  lcd.print("WiFi Starting...");
  wifi.begin(9600);
  
  delay(200);
  
  while (wifi.ready() != 1)
  {
    lcd.home();
    lcd.print("WiFi not ready");
    delay(15000);
  }  
  lcd.clear();
  lcd.home();
  lcd.print("DigiX - Ready!");
  
  

}

void loop()
{
  // Process USB tasks
  usb.Task();
}
