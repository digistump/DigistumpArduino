#include <rtc_clock.h>

// Select the Slowclock source
//RTC_clock rtc_clock(RC);
RTC_clock rtc_clock(XTAL);

char* daynames[]={"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};

void setup() {
  Serial.begin(9600);
  rtc_clock.init();
  rtc_clock.set_time(10, 29, 49);
  
  rtc_clock.set_alarmtime(10, 30, 0);
  
  rtc_clock.attachalarm(announcement);
//  rtc_clock.disable_alarm();
}

void loop() {
  Serial.print("At the third stroke, it will be ");
  Serial.print(rtc_clock.get_hours());
  Serial.print(":");
  Serial.print(rtc_clock.get_minutes());
  Serial.print(":");
  Serial.println(rtc_clock.get_seconds());
}

void announcement() {
  Serial.println();
  Serial.println("Get up and buy an Arduino Due.");
}
