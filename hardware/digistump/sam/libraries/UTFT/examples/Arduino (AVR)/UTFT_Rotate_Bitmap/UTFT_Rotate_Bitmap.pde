// UTFT_Rotate_Bitmap (C)2012 Henning Karlsen
// web: http://www.henningkarlsen.com/electronics
//
// This program is a demo of the drawBitmap()-function.
//
// This program requires the UTFT library.
//

#include <UTFT.h>
#include <avr/pgmspace.h>

// Uncomment the next line for Arduino 2009/Uno
UTFT myGLCD(ITDB32S,19,18,17,16);   // Remember to change the model parameter to suit your display module!

// Uncomment the next line for Arduino Mega
//UTFT myGLCD(ITDB32S,38,39,40,41);   // Remember to change the model parameter to suit your display module!

extern unsigned int tux[0x400];

void setup()
{
  myGLCD.InitLCD(LANDSCAPE);
  myGLCD.fillScr(255, 255, 255);
  myGLCD.setColor(0, 0, 0);
}

void loop()
{
    for (int i=0; i<360; i+=5)
    {
      myGLCD.drawBitmap (10, 10, 32, 32, tux, i, 16, 16);
    }
}

