//----LCD Screen Stuff----//
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define BUTTON1_PIN 2
#define BUTTON2_PIN 4
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

//----LED Stuff----//
#include <FastLED.h>
#define NUM_LEDS 100
#define DATA_PIN 3               //The data pin for the LEDs
CRGB leds[NUM_LEDS];
//CHSV sunriseColor = {40, 200, 255};
//CHSV sunsetColor = {43, 70, 255};
CHSV twilightColor = {0, 150, 255};
CHSV sunriseColor = twilightColor;
CHSV sunsetColor = twilightColor;
CHSV color1 = {50, 210, 255};
CHSV color2 = {150, 255, 255};
unsigned long sunriseTime;
unsigned long sunsetTime;
unsigned long twilightEnd;
unsigned long durration;
byte daytimeStarts[NUM_LEDS];

//----Sunrise Sunset Time Stuff----//
#include <Dusk2Dawn.h>
#include <Chronos.h>
#include <Time.h>
#include <TimeLib.h>
Dusk2Dawn morgantown(39.3755, -79.5725, -4);
Chronos::DateTime currentDate(2020, 1, 1, 0, 0, 0);

//----Clock Module Stuff----//
#include <DS3231.h>
DS3231 Clock;
bool Century = false;
bool PM;
bool h12;


void setup()
{
  Wire.begin();
  lcd.begin(16, 2);              // initialize the lcd
  lcd.home ();                   // go home (first character on the LCD screen)
  pinMode(BUTTON1_PIN, INPUT_PULLUP);      //setup the button pins, they should be wired to ground
  pinMode(BUTTON2_PIN, INPUT_PULLUP);

  FastLED.addLeds<WS2811, DATA_PIN>(leds, NUM_LEDS);

  //default alarm times
  sunriseTime = 24300; //6:45 AM;
  sunsetTime = 70200;  //6:00 PM;
  twilightEnd = 79560;//10:06 PM;
  durration = 2700;

  Serial.begin(9600);

  for (int i = 0; i < NUM_LEDS; i++) {
    daytimeStarts[i] = random(0, 99);
  }
}







//----Setup Variables for the LCD----//
unsigned long timeCounter = 0;
unsigned long previousMillis = 0;
unsigned long lastRefresh = 0;
int pos = 0;
int hours = 0;
int minutes = 0;
int seconds = 0;
int yearsSince2000 = 20;
int months = 1;
int days = 1;
int setMode = 0;
char disp[8];
char disp2[8];
unsigned long t;
unsigned long buttonStart[] = {0, 0};
int buttonPin[] = {BUTTON1_PIN, BUTTON2_PIN};
int pressed[] = {0, 0};
int buttonDelay = 500;              //How long in millis between increments when a button is held
unsigned long refreshDelay = 1000;  //How often in millis the LCD screen refreshes
int presses = 0;
bool setThisFrame = false;
String AMPM = "AM";
int prevHrs = 0;
//---------------------------------//

//----Setup Variables for the LEDs----//
int state = 0;
unsigned long refreshDelayLED = 100;
unsigned long lastRefreshLED = 0;
float place = 0;
float maxBrightness = 255;
//------------------------------------//

//----This function controls the logic behind when a button press is considered true----//
int checkButton(int buttonNumber) {
  if (digitalRead(buttonPin[buttonNumber - 1]) == LOW) { //is the button down
    if (pressed[buttonNumber - 1] == 0) {                //was the button down the last time this was checked?
      pressed[buttonNumber - 1] = 1;
      buttonStart[buttonNumber - 1] = millis();
      return 1;
    } else {    //the button was down the last check
      if ((millis() - buttonStart[buttonNumber - 1]) >= buttonDelay) { //has it been longer than the button delay since the last increment?
        buttonStart[buttonNumber - 1] = millis();
        return 1;
      } else {
        return 0;
      }
    }
  } else {
    if (pressed[buttonNumber - 1] == 1) { //if it was pressed last time, set pressed to no
      pressed[buttonNumber - 1] = 0;
    }
    return 0;
  }
}


//----This function checks if the LCD screen should refresh----//
bool checkRefresh() {
  if ((millis() - lastRefresh) >= refreshDelay) {
    lastRefresh = millis();
    return true;
  } else {
    return false;
  }
}


//----This function controls the LED pattern----//
void ledPattern(unsigned long timeCounter, unsigned long sunriseTime, unsigned long sunsetTime, unsigned long twilightEnd, unsigned long  durration) {
  //state 0: night, state 1: morning, state 2: daytime, state 3: evening, state 4:twilight
  //sunriseTime and sunsetTime need to be in milliseconds since midnight
  //durration needs to be in seconds
  if (checkRefreshLED) {

    if (timeCounter < sunriseTime) { //determining what state we are in
      state = 0;
    } else if (timeCounter < sunriseTime + durration) {
      state = 1;
    } else if (timeCounter < sunsetTime) {
      state = 2;
    } else if (timeCounter < twilightEnd) {
      state = 3;
    } else {
      state = 0;
    }

    if (state == 0) { //at night, lights off
      for (byte i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB(0, 0, 0);
      }
      if (checkRefreshLED()) {
        FastLED.show();
      }
    } else if (state == 1) {
      //morning, gradual pulsing increase
      float tempdur = durration;
      float percentThrough = (timeCounter - sunriseTime) / tempdur;
      maxBrightness = 255 * percentThrough;
      ledPulse(sunriseColor);
    } else if (state == 2) {
      //daytime, dual color pulse
      daytimePattern();
    } else if (state == 3) {
      //sunset, gradual pulsing change from pinkish to purplish
      float percentThrough = float((timeCounter - sunsetTime)) / (twilightEnd - sunsetTime);
      CRGB twilightRGB = rgb2hsv_approximate(twilightColor);
      CRGB sunsetRGB = rgb2hsv_approximate(sunsetColor);
      byte thisColor[3];
      thisColor[0] = (twilightRGB.r * percentThrough) + (sunsetRGB.r * (1 - percentThrough));
      thisColor[1] = (twilightRGB.g * percentThrough) + (sunsetRGB.g * (1 - percentThrough));
      thisColor[2] = (twilightRGB.b * percentThrough) + (sunsetRGB.b * (1 - percentThrough));
      CRGB tempColor = CRGB(thisColor[0], thisColor[1], thisColor[2]);
      maxBrightness = 255;
      ledPulse(rgb2hsv_approximate(tempColor));
    }
  }
}


//----Morning, evening, and twilight pattern----//
void ledPulse(CHSV color) {
  if (checkRefreshLED()) {
    CHSV c;
    int value;
    for (byte i = 0; i < NUM_LEDS; i++) {
      if (i < 98 and (abs(daytimeStarts[i] - daytimeStarts[i + 1]) < 10 or random() < 0.1)) {
        daytimeStarts[i] = (daytimeStarts[i] + random(5, 10)) % 100;
      }
      int temp = (daytimeStarts[i] + round(place)) % 100;
      c = color;
      if (temp < 40) {
        value = map(temp, 0, 39, 75, 90);
      } else if (temp < 50) {
        value = map(temp, 39, 49, 91, 255);
      } else if (temp < 80) {
        value = map(temp, 50, 79, 255, 91);
      } else if (temp < 100) {
        value = map(temp, 80, 99, 90, 75);
      }
      c.value = map(value, 0, 255, 0, c.value);
      leds[i] = c;
    }
  }
  place = place + .125;
  if (place >= 100) {
    place = 0;
  }
  FastLED.show();
}


//----Daytime Pattern----//
void daytimePattern() {
  if (checkRefreshLED()) {
    CHSV c;
    int saturation;
    for (byte i = 0; i < NUM_LEDS; i++) {
      int temp = (daytimeStarts[i] + round(place)) % 100;
      if (temp < 25) {
        c = color1;
        saturation = map(temp, 0, 24, 0, 255);
      } else if (temp < 50) {
        c = color1;
        saturation = map(temp, 25, 49, 255, 0);
      } else if (temp < 75) {
        c = color2;
        saturation = map(temp, 50, 74, 0, 255);
      } else {
        c = color2;
        saturation = map(temp, 75, 99, 255, 0);
      }
      c.saturation = map(saturation, 0, 255, 0, c.saturation);
      leds[i] = c;
    }
    place++;
    if (place >= 100) {
      place = 0;
    }
    FastLED.show();
  }
}




//----This function checks if the LEDs should refresh----//
bool checkRefreshLED() {
  if ((millis() - lastRefreshLED) >= refreshDelayLED) {
    lastRefreshLED = millis();
    return true;
  } else {
    return false;
  }
}




byte lastHour = 0;

void loop() {
  bool but1 = checkButton(1); //only read the buttons once a frame
  bool but2 = checkButton(2);

  if (but1 && setMode == 0) {   //should the display be the clock or the set screen?
    setMode = 1;
    setThisFrame = true;
  } else {
    setThisFrame = false;
  }

  if (setMode == 1) {   //Setmode = 1 means the time is being set
    hours = Clock.getHour(h12,PM);
    minutes = Clock.getMinute();

    if (pos == 0) { //Pos 1: Hours, Pos 2: Minutes
      lcd.setCursor(0, 0);
      lcd.print("Hours");
      if (but2) {
        hours++;
        hours = (hours % 24);
        Clock.setHour(hours);
      }
      if (but1 && not(setThisFrame)) { //this makes sure we don't skip past setting the hours
        pos += 1;
      }
    } else if (pos == 1) {
      lcd.setCursor(0, 0);
      lcd.print("Mins ");
      if (but2) {
        minutes = Clock.getMinute()+1;
        minutes = minutes % 60;
        Clock.setMinute(minutes);
        Clock.setSecond(0);
      }
      if (but1 && not(setThisFrame)) {
        pos += 1;
        //setMode = 0;
      }
    } else if (pos == 2) {
      lcd.setCursor(0, 0);
      lcd.print("Years");
      if (but2) {
        currentDate.setYear(currentDate.year() + 1);
        if (currentDate.year() >= 2030) {
          currentDate.setYear(2020);
        }
      }
      if (but1 && not(setThisFrame)) {
        pos += 1;
        //setMode = 0;
      }
    } else if (pos == 4) {
      lcd.setCursor(0, 0);
      lcd.print("Days ");
      if (but2) {
        currentDate.setDay(currentDate.day() + 1);
      }
      if (but1) {
        pos = 0;
        setMode = 0;
        sunriseTime = long(60 * long(morgantown.sunrise(currentDate.year(), currentDate.month(), currentDate.day(), false)));
        if (sunriseTime > 24300) {
          sunriseTime = 24300;
        }
        sunsetTime = long(60 * long(morgantown.sunset(currentDate.year(), currentDate.month(), currentDate.day(), false))) - 3600;
      }
    } else if (pos == 3) {
      lcd.setCursor(0, 0);
      lcd.print("Month");
      if (but2) {
        currentDate.setMonth(currentDate.month() + 1);
      }
      if (but1 && not(setThisFrame)) {
        pos += 1;
        //setMode = 0;
      }
    }



    if (checkRefresh()) {

      lcd.setCursor(0, 1);
      sprintf(disp2, "%02d:%02d:%02d", Clock.getHour(h12,PM), Clock.getMinute(), Clock.getSecond());
      lcd.print("    " + String(disp2));
      sprintf(disp, "%02d/%02d/%02d", currentDate.month(), currentDate.day(), currentDate.year()-2000);
      lcd.setCursor(0, 0);
      lcd.print("        " + String(disp));
    }

  } else {
    //this area is what happens when the time isn't being set
    if (checkRefresh()) {
      lcd.setCursor(0, 0);
      sprintf(disp, "%02d/%02d/%02d", currentDate.month(), currentDate.day(), currentDate.year()-2000);
      lcd.print("Time    " + String(disp));
      lcd.setCursor(0, 1);
      sprintf(disp2, "%02d:%02d:%02d", Clock.getHour(h12,PM), Clock.getMinute(), Clock.getSecond());
      lcd.print("    " + String(disp2));
      if (lastHour == 23 && Serial.print(Clock.getHour(h12,PM))==0) {
        currentDate += Chronos::Span::Days(1);
        sunriseTime = long(60 * long(morgantown.sunrise(currentDate.year(), currentDate.month(), currentDate.day(), true)));
        if (sunriseTime > 24300) {
          sunriseTime = 24300;
        }
        sunsetTime = long(60 * long(morgantown.sunset(currentDate.year(), currentDate.month(), currentDate.day(), true))) - 3600;
      }
    }
  }
  timeCounter = long(Clock.getHour(h12,PM))*60*60 + long(Clock.getMinute())*60 + long(Clock.getSecond());
  ledPattern(timeCounter, sunriseTime, sunsetTime, twilightEnd, durration);
  lastHour = Clock.getHour(h12,PM);
  /*lcd.setCursor(0, 1);
  lcd.print(String(timeCounter)+"        ");
  lcd.setCursor(0,0);
  lcd.print(String(sunriseTime) + "        ");*/
}
