


#include <DS3231.h>
#include <Wire.h>

DS3231 Clock;

byte Year;
byte Month;
byte Date;
byte Hour;
byte Minute;
byte Second;


void setup() {
  Wire.begin();
  Year = 2021;
  Month = 1;
  Date = 23;
  Hour = 17;
  Minute = 15;
  Second = 0;
}

void loop() {

    Clock.setYear(Year);
    Clock.setMonth(Month);
    Clock.setDate(Date);
    Clock.setHour(Hour);
    Clock.setMinute(Minute);
    Clock.setSecond(Second);


}
