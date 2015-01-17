DigistumpArduino
================

Files to add Digistump support (Digispark, Pro, DigiX) to Arduino 1.5.X (1.5.8+)


**Binary downloads of the bundled IDE can be found here:** https://github.com/digistump/DigistumpArduino/releases/tag/v1.5.8C

**To manually install:**

Place digistump folder inside the Arduino /hardware folder.

**To compile:**

To compile tools for other platforms or from scratch use:

Arduino 1.5.X (1.5.8+)

This repo.

Micronucleus: https://github.com/micronucleus/micronucleus/tree/80419704f68bf0783c5de63a6a4b9d89b45235c7
Dependencies: libusb and possibly lib32stdc on linux - (on ubuntu get it by issuing: apt-get install lib32stdc++6)

AVR-Dummy: https://github.com/digistump/avr-dummy

 - Download the lastest Arduino 1.5.X branch binaries or source (and compile per their instructions if necessary)
 - Download this repo
 - Download the Micronucleus source linked above 
 - Download the AVR-Dummy source above
 - Compile the micronucleus commandline tool (commandline directory) and avr-dummy ("make all" for both) - which will produce executables named micronucleus and avrdude
 - Copy the digistump folder from this repo into the hardware folder in your arduino directory.
 - Copy micronucleus and avrdude executables to the hard/digistump/avr/tools folder in the arduino directory
 - You're all set!


