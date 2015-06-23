AVR_BIN=/usr/local/avr/bin AVR_ETC=/usr/local/avr/etc jam F_CPU=8000000 sensornet.elf
touch sensornet.pde
AVR_BIN=/usr/local/avr/bin AVR_ETC=/usr/local/avr/etc jam F_CPU=8000000 "CCFLAGS=-include DuinodeV3.h" -dx UPLOAD_SPEED=57600 u1 && screen /dev/ttyUSB1 57600
