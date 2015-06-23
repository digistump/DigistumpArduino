jam F_CPU=8000000 "CCFLAGS=-include DuinodeV5.h" -dx UPLOAD_SPEED=57600 u0 && picocom -b 57600 /dev/ttyUSB0
