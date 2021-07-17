#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define BUTTON1_PIN 2
#define BUTTON2_PIN 4
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

void setup()
{
  lcd.begin(16, 2);              // initialize the lcd
  lcd.home ();                   // go home
  pinMode(2, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
}








unsigned long timeCounter = 0;
unsigned long previousMillis = 0;
unsigned long lastRefresh = 0;
int pos = 0;
int hours = 0;
int minutes = 0;
int seconds = 0;
int setMode = 0;
char disp[8];
unsigned long t;
unsigned long buttonStart[] = {0, 0};
int buttonPin[] = {BUTTON1_PIN, BUTTON2_PIN};
int pressed[] = {0, 0};
int buttonDelay = 400;
unsigned long refreshDelay = 1000;
int presses = 0;
bool setThisFrame = false;
String AMPM = "AM";
int prevHrs = 0;


int checkButton(int buttonNumber) {
  if (digitalRead(buttonPin[buttonNumber - 1]) == LOW) {
    if (pressed[buttonNumber - 1] == 0) {
      pressed[buttonNumber - 1] = 1;
      buttonStart[buttonNumber - 1] = millis();

      return 1;
    } else {
      if ((millis() - buttonStart[buttonNumber - 1]) >= buttonDelay) {
        buttonStart[buttonNumber - 1] = millis();

        return 1;
      } else {
        return 0;
      }
    }
  } else {
    if (pressed[buttonNumber - 1] == 1) {
      pressed[buttonNumber - 1] = 0;
    }
    return 0;
  }
}



bool checkRefresh() {
  if ((millis() - lastRefresh) >= refreshDelay) {
    lastRefresh = millis();
    return true;
    } else {
    return false;
    }
  //return false;
}



void loop() {
  bool but1 = checkButton(1);
  bool but2 = checkButton(2);
  
  if (but1 && setMode == 0) {
    setMode = 1;
    setThisFrame = true;
  } else {
    setThisFrame = false;
  }
  
  if (setMode == 1) {
    if (pos == 0) {
      lcd.setCursor(0, 0);
      lcd.print("Setting Hours   ");
      if (but2) {
        hours++;
        hours = (hours % 12);
        if (hours == 0) {
          if (AMPM == "AM") {
            AMPM = "PM";
          } else {
            AMPM = "AM";
          }
        }
      }
      if (but1 && not(setThisFrame)) {
        pos += 1;
      }
    } else {
      lcd.setCursor(0, 0);
      lcd.print("Setting Mins    ");
      if (but2) {
        minutes += 1;
        minutes = minutes % 60;
        seconds = 0;
      }
      if (but1) {
        pos = 0;
        setMode = 0;
      }
    }

    if (checkRefresh()) {
      if (hours == 0) {
        hours = 12;
      }
      sprintf(disp, "%02d:%02d:%02d", hours, minutes, seconds);
      String disp2 = String(disp);
      lcd.setCursor(0, 1);
      lcd.print("    "+disp2+"  "+AMPM);
      timeCounter = (hours-1) * 3600000 + minutes * 60000 + seconds * 1000;
    }
    timeCounter = (hours-1) * 3600000 + minutes * 60000 + seconds * 1000;
    previousMillis = millis();

  } else {

    if (checkRefresh()) {
      timeCounter = timeCounter + millis() - previousMillis;
      previousMillis = millis();
      if (timeCounter >= 86400000) {
        timeCounter = timeCounter - 86400000;
      }
      t = timeCounter;
      hours = floor(t / 3600000);
      t = t - hours * 3600000;
      hours = hours % 12 + 1;
      if (prevHrs == 11 && hours == 12) {
        if (AMPM == "AM") {
            AMPM = "PM";
          } else {
            AMPM = "AM";
          }
      }
      prevHrs = hours;
      minutes = floor(t / 60000);
      t = t - minutes * 60000;
      seconds = floor(t / 1000);
      sprintf(disp, "%02d:%02d:%02d", hours, minutes, seconds);
      String disp2 = String(disp);
      lcd.setCursor(0, 0);
      lcd.print("  Current Time  ");
      lcd.setCursor(0, 1);
      lcd.print("    "+disp2+"  "+AMPM);
    }
  }
}
