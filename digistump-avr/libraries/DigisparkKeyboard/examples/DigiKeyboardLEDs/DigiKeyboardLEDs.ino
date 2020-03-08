/***
 *      ___  _      _ _  __         _                      _ 
 *     |   \(_)__ _(_) |/ /___ _  _| |__  ___  __ _ _ _ __| |
 *     | |) | / _` | | ' </ -_) || | '_ \/ _ \/ _` | '_/ _` |
 *     |___/|_\__, |_|_|\_\___|\_, |_.__/\___/\__,_|_| \__,_|
 *     __     |___/_   _      _|__/___ ___                   
 *     \ \    / (_) |_| |_   | |  | __|   \ ___              
 *      \ \/\/ /| |  _| ' \  | |__| _|| |) (_-<              
 *       \_/\_/ |_|\__|_||_| |____|___|___//__/              
 *                                                           
 *   Danjovic, March 2020                                                        
 */

// Use modified version of DigiKeyboard library
// 
#include "DigiKeyboard.h"

void setup() {
  pinMode(1,OUTPUT);      // BuiltIn LED as output
}


void loop() {
  // Check the state of Caps Lock and change led accordingly
  if (DigiKeyboard.getLEDs() & CAPS_LOCK)
      digitalWrite(1, HIGH);
  else 
      digitalWrite(1, LOW);
  DigiKeyboard.delay(10);
}
