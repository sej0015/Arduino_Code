
#include <DS3231.h>
#include <Wire.h>
DS3231 Clock;
bool Century = false;
bool PM;
bool h12;

void setup() {
  // Start the I2C interface
  Wire.begin();
  // Start the serial interface
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  Serial.print(Clock.getMonth(Century));
  Serial.print('/');
  Serial.print(Clock.getDate());
  Serial.print('/');
  Serial.print(Clock.getYear());  
  Serial.print(' ');    
  Serial.print(Clock.getHour(h12,PM));
  Serial.print(':');
  Serial.print(Clock.getMinute());
  Serial.print(':');
  Serial.print(Clock.getSecond());
  Serial.print('\n');
  delay(1000);
}
