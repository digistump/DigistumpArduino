#include <stdio.h>

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#define _CXXTEST_HAVE_STD
#define _CXXTEST_LONGLONG long long
#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>
#include <cxxtest/TestMain.h>
#include <cxxtest/ErrorPrinter.h>
#include <cxxtest/Root.cpp>

#include <RF24Network_config.h>
#include <RF24Network.h>
#include <RF24.h>

extern void board_start(const char*);
extern RF24Network network;
extern RF24 radio;

int main( void ) 
{
  CxxTest::ErrorPrinter tmp;

  while(1)
  {
    board_start(__FILE__);
    
    SPI.begin();
    radio.begin();
    network.begin(/* channel */100,/* this node */0);

    CxxTest::Main<CxxTest::ErrorPrinter>( tmp, 0, NULL );

    printf("Tests complete.  Restarting...\r\n");
  }
}
const char* CxxTest::RealWorldDescription::_worldName = "cxxtest";
// vim:cin:ai:sts=2 sw=2 ft=cpp
