#include <rtc_clock.h>

#define START_VALUE    "Jan 01 2007"

// Select the Slowclock source
//RTC_clock rtc_clock(RC);
RTC_clock rtc_clock(XTAL);

int counter = 0;

void setup() {
  Serial.begin(9600);
  rtc_clock.init();
}

void loop() {
  if (rtc_clock.date_already_set() == 0) {
    Serial.print("no  ");
  } else {
    Serial.print("yes ");
  }
  digitprint(counter, 3, 1);
  Serial.println();
  delay(125);
  if (counter == 50) {
    rtc_clock.set_date(__DATE__);
  }
  if (counter == 100) {
    counter = 0;
    // technical all the same
    //rtc_clock.set_date( 1,  1, 2007);
    //rtc_clock.set_date("Jan 01 2007");
    rtc_clock.set_date(START_VALUE);
  }
  counter++;
}

void digitprint(int value, int lenght, int placeholder){
  for (int i = 0; i < (lenght - numdigits(value)); i++){
    if (placeholder == 0)
      Serial.print("0");
    else
      Serial.print(" ");
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
