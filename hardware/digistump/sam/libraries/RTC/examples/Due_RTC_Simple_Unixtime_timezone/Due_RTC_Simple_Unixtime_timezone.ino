#include <rtc_clock.h>

// Select the Slowclock source
//RTC_clock rtc_clock(RC);
RTC_clock rtc_clock(XTAL);

char* daynames[]={"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};

void setup() {
  Serial.begin(9600);
  rtc_clock.init();
  rtc_clock.set_time(__TIME__);
  rtc_clock.set_date(__DATE__);
}

void loop() {
  Serial.print("Unixtime: ");
  //All known Timezones are supported set in this style "UTC+1" or "UTC-930" without colon
  Serial.println(rtc_clock.unixtime(UTC-5));
  Serial.println("And in plain for everyone");
  Serial.print("Time: ");
  digitprint(rtc_clock.get_hours(), 2);
  Serial.print(":");
  digitprint(rtc_clock.get_minutes(), 2);
  Serial.print(":");
  digitprint(rtc_clock.get_seconds(), 2);
  Serial.println("");
  Serial.print("Date: ");
  Serial.print(daynames[rtc_clock.get_day_of_week()-1]);
  Serial.print(" ");
  digitprint(rtc_clock.get_days(), 2);
  Serial.print(".");
  digitprint(rtc_clock.get_months(), 2);
  Serial.print(".");
  Serial.println(rtc_clock.get_years());
  Serial.println("");
}

void digitprint(int value, int lenght){
  for (int i = 0; i < (lenght - numdigits(value)); i++){
    Serial.print("0");
  }
  Serial.print(value);
}

int numdigits(int i){
  int digits;
  if (i < 10)
    digits = 1;
  else
    digits = (int)(log10((double)i)) + 1;
  return digits;
}
