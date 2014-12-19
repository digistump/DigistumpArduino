#include <rtc_clock.h>

// Select the Slowclock source
//RTC_clock rtc_clock(RC);
RTC_clock rtc_clock(XTAL);

char* daynames[]={"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};

boolean toggle = false;
int old_unixtime;

void setup() {
  Serial.begin(9600);
  rtc_clock.init();
  
  //Summertimebegin Germany in 2013
  rtc_clock.set_time(1,59,50);
  rtc_clock.set_date(31,3,2013);
}

void loop() {
  if ( rtc_clock.unixtime() != old_unixtime) {
    old_unixtime = rtc_clock.unixtime();
    output();
  }
  
  if ( rtc_clock.get_seconds() == 10 ) {
    Serial.print("Timeleap ");
    toggle = !toggle;
  }  
  if ( rtc_clock.get_seconds() == 10 && toggle == false ) {
    //Summertimebegin Germany in 2013
    Serial.println("back to summer");
    rtc_clock.set_time(1,59,50);
    rtc_clock.set_date(31,3,2013);
  } else if (rtc_clock.get_seconds() == 10 && toggle == true ) {
    //Wintertimebegin Germany in 2013
    Serial.println("forward to the end of summer");
    rtc_clock.set_time(1,59,50);
    rtc_clock.set_date(27,10,2013);
  }
}

void output() {
  Serial.print("Unixtime: ");
  Serial.println(rtc_clock.unixtime(Germany));
  
  Serial.println("And in plain for everyone");
  Serial.print("Time: ");
  digitprint(rtc_clock.get_hours() + rtc_clock.summertime(), 2);
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
  
  Serial.print("Is Summertime?: ");
  if ( rtc_clock.summertime() == 1 ) {
    Serial.println("Yes!");
  } else {
    Serial.println("No!");
  }
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
