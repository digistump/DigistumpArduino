// UTFT_Rotate_Bitmap (C)2012 Henning Karlsen
// web: http://www.henningkarlsen.com/electronics
//
// This program is a demo of the drawBitmap()-function.
//
// This program requires the UTFT library.
//

#include <UTFT.h>

// Uncomment the next line for chipKit Uno32/uC32
UTFT myGLCD(ITDB24D,34,35,36,37);   // Remember to change the model parameter to suit your display module!

// Uncomment the next line for chipKit Max32
//UTFT myGLCD(ITDB24D,82,83,84,85);   // Remember to change the model parameter to suit your display module!

extern unsigned short biohazard[0x1000];

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
      myGLCD.drawBitmap (10, 10, 64, 64, biohazard, i, 32, 32);
    }
}

