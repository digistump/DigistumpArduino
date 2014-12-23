#ifndef SoftRcPulseOut_h
#define SoftRcPulseOut_h

/*
 Update 01/03/2013: add support for DigiSpark (http://digistump.com): automatic Timer selection (RC Navy: p.loussouarn.free.fr)
 
 English: by RC Navy (2012)
 =======
 <SoftRcPulseOut>: a library mainly based on the <SoftwareServo> library, but with a better pulse generation to limit jitter.
 It supports the same methods as <SoftwareServo>.
 It also support Pulse Width order given in microseconds. The current Pulse Width can also be read in microseconds.
 The refresh method can admit an optionnal argument (force). If SoftRcPulseOut::refresh(1) is called, the refresh is forced even if 20 ms are not elapsed.
 http://p.loussouarn.free.fr

 Francais: par RC Navy (2012)
 ========
 <SoftRcPulseOut>: une librairie majoritairement basee sur la librairie <SoftwareServo>, mais avec une meilleure generation des impulsions pour limiter la gigue.
 Elle supporte les memes methodes que <SoftwareServo>.
 Elle supporte egalement une consigne de largeur d'impulsion passee en microseconde. La largeur de l'impulsion courante peut egalement etre lue en microseconde.
 La methode refresh peut admettre un parametre optionnel (force). Si SoftRcPulseOut::resfresh(1) est appelee, le refresh est force meme si 20 ms ne se sont pas ecoulee.
 http://p.loussouarn.free.fr
*/

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <inttypes.h>

class SoftRcPulseOut
{
  private:
	boolean ItMasked;
    uint8_t pin;
    uint8_t angle;       // in degrees
    uint16_t pulse0;     // pulse width in TCNT0 counts
    uint8_t min16;       // minimum pulse, 16uS units  (default is 34)
    uint8_t max16;       // maximum pulse, 16uS units, 0-4ms range (default is 150)
    class SoftRcPulseOut *next;
    static SoftRcPulseOut* first;
  public:
    SoftRcPulseOut();
    uint8_t attach(int);     // attach to a pin, sets pinMode, returns 0 on failure, won't
                             // position the servo until a subsequent write() happens
    void detach();
    void write(int);         // specify the angle in degrees, 0 to 180
    void write_us(int);      // specify the angle in microseconds, 500 to 2500
    uint8_t read();          // return the current angle
    uint8_t read_us();       // return the current pulse with in microseconds
    uint8_t attached();
    void setMinimumPulse(uint16_t);  // pulse length for 0 degrees in microseconds, 540uS default
    void setMaximumPulse(uint16_t);  // pulse length for 180 degrees in microseconds, 2400uS default
    static uint8_t refresh(bool force = false);    // must be called at least every 50ms or so to keep servo alive
                              // you can call more often, it won't happen more than once every 20ms
};

#endif
