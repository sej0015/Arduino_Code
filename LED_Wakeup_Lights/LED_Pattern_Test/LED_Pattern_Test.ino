//----LED Stuff----//
#include <FastLED.h>
#define NUM_LEDS 100
#define DATA_PIN 3               //The data pin for the LEDs
CRGB leds[NUM_LEDS];
CHSV sunriseColor = {40, 200, 255};
CHSV sunsetColor = {10, 171, 255};
CHSV twilightColor = {192, 255, 255};
CHSV color1 = {50, 210, 255};
CHSV color2 = {150, 255, 255};
byte daytimeStarts[NUM_LEDS];



void setup() {
  FastLED.addLeds<WS2811, DATA_PIN>(leds, NUM_LEDS);
  Serial.begin(9600);

  for (int i = 0; i < NUM_LEDS; i++) {
    daytimeStarts[i] = random(0, 99);
  }
}

unsigned long refreshDelayLED = 100;
unsigned long lastRefreshLED = 0;
float maxBrightness = 255;
int place = 0;

void ledPattern(CHSV color) {
  if (checkRefreshLED()) {
    for (byte i = 0; i < NUM_LEDS; i++) {
      float currentBrightness = floor((maxBrightness / 2 * ((sin((place + i) / NUM_LEDS * 4 * PI) + 1) / 2)) + (maxBrightness / 2));
      CHSV tempColor = color;
      tempColor.value = currentBrightness;
      leds[i] = tempColor;
    }
    place++;
    if (place >= 100) {
      place = 0;
    }
    FastLED.show();
  }
}

void daytimePattern() {
  if (checkRefreshLED()) {
    CHSV c;
    int saturation;
    for (byte i = 0; i < NUM_LEDS; i++) {
      int temp = (daytimeStarts[i] + place) % 100;
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


bool checkRefreshLED() {
  if ((millis() - lastRefreshLED) >= refreshDelayLED) {
    lastRefreshLED = millis();
    return true;
  } else {
    return false;
  }
}



void loop() {
  //ledPattern(sunriseColor);
  ledPattern(sunsetColor);
  //ledPattern(twilightColor);
  //daytimePattern();
}
