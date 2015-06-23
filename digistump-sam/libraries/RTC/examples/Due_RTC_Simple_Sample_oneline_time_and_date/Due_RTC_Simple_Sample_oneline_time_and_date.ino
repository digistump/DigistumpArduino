#include <rtc_clock.h>

// Select the Slowclock source
//RTC_clock rtc_clock(RC);
RTC_clock rtc_clock(XTAL);

char* daynames[]={"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
int hh,mm,ss,dow,dd,mon,yyyy;

void setup() {
  Serial.begin(9600);
  rtc_clock.init();
  rtc_clock.set_time(__TIME__);
  rtc_clock.set_date(__DATE__);
}

void loop() {
  Serial.print("Time: ");
  rtc_clock.get_time(&hh,&mm,&ss);
  rtc_clock.get_date(&dow,&dd,&mon,&yyyy);
  digitprint(hh, 2);
  Serial.print(":");
  digitprint(mm, 2);
  Serial.print(":");
  digitprint(ss, 2);
  Serial.println("");
  Serial.print("Date: ");
  Serial.print(daynames[dow-1]);
  Serial.print(" ");
  digitprint(dd, 2);
  Serial.print(".");
  digitprint(mon, 2);
  Serial.print(".");
  Serial.println(yyyy);
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
