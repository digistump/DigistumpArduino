DigistumpArduino
================

Files to add Digistump support (Digispark, Pro, DigiX) to Arduino 1.6.X (1.6.4+)

**These files are designed for install via the Arduino Boards Manager:** 

Board manager URL: http://digistump.com/package_digistump_index.json

**Full Install Instructions:** 

Digispark: http://digistump.com/wiki/digispark/tutorials/connecting

Digispark Pro: http://digistump.com/wiki/digispark/tutorials/connectingpro

DigiX: http://digistump.com/wiki/digix/tutorials/software

**To compile:**

Micronucleus is the only executable in these packages that is pre-compiled:

Micronucleus: https://github.com/micronucleus/micronucleus/tree/80419704f68bf0783c5de63a6a4b9d89b45235c7
Dependencies: libusb and possibly lib32stdc on linux - (on ubuntu get it by issuing: apt-get install lib32stdc++6)