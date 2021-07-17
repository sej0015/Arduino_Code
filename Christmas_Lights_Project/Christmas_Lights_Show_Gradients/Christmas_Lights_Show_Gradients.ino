#include "LEDColors.h"
#include <FastLED.h>
#define NUM_LEDS 100
#define DATA_PIN 3
CRGB leds[NUM_LEDS];
#define numFrames 120
#define s (numFrames * NUM_LEDS)
const byte PROGMEM c[s] = {colors};
//int place;
byte loadedBytes[NUM_LEDS];
CHSV color1 = {50, 255, 255};
CHSV color2 = {150, 255, 255};
float thisColor[3];
float thisByte;
bool done = false;
int selector;

void setup() {
  FastLED.addLeds<WS2811, DATA_PIN>(leds, NUM_LEDS);
  pinMode(4, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  for (byte i = 0; i < numFrames; i++) {

    if (digitalRead(4) == LOW) {
      if (selector == 1) {
        selector = 0;
      } else if (selector == 0) {
        selector = 1;
      }
    }
    
    while (digitalRead(4) == LOW) {
      if (selector == 1) {
        color1.hue += 1;
        for (byte k = 0; k < NUM_LEDS; k++) {
          leds[k] = color1;
        }
        FastLED.show();
        delay(100);
      } else {
        color2.hue += 1;
        for (byte k = 0; k < NUM_LEDS; k++) {
          leds[k] = color2;
        }
        FastLED.show();
        delay(100);
      }
    }


    CRGB color1RGB = rgb2hsv_approximate(color1);
    CRGB color2RGB = rgb2hsv_approximate(color2);
    memcpy_P(&loadedBytes, &(c[NUM_LEDS * i]), sizeof(loadedBytes));
    for (byte j = 0; j < NUM_LEDS; j++) {
      thisByte = loadedBytes[j];
      thisColor[0] = (color1RGB.r * thisByte / 255) + (color2RGB.r * (1 - (thisByte / 255)));
      thisColor[1] = (color1RGB.g * thisByte / 255) + (color2RGB.g * (1 - (thisByte / 255)));
      thisColor[2] = (color1RGB.b * thisByte / 255) + (color2RGB.b * (1 - (thisByte / 255)));
      leds[j] = CRGB(thisColor[0], thisColor[1], thisColor[2]);
    }
    FastLED.show();
    delay(70);
  }
}
