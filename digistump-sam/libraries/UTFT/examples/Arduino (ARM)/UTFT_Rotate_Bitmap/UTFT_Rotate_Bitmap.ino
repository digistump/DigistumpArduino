// UTFT_Rotate_Bitmap (C)2013 Henning Karlsen
// web: http://www.henningkarlsen.com/electronics
//
// This program is a demo of the drawBitmap()-function.
//
// This program requires the UTFT library.
//

#include <UTFT.h>

UTFT myGLCD(ITDB24E_16,38,39,40,41);   // Remember to change the model parameter to suit your display module!

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

