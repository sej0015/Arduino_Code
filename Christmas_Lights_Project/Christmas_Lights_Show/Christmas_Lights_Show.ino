#include "LEDColors.h"
#include <FastLED.h>
#define NUM_LEDS 100
#define DATA_PIN 3
CRGB leds[NUM_LEDS];
#define numFrames 80
#define s (numFrames * NUM_LEDS * 3)
const byte PROGMEM c[s] = {colors};
int place;
byte loadedBytes[3 * NUM_LEDS];

void setup() {
  FastLED.addLeds<WS2811, DATA_PIN>(leds, NUM_LEDS);
}

void loop() {
  for (byte i = 0; i < numFrames; i++) {
    memcpy_P(&loadedBytes, &(c[NUM_LEDS * i * 3]),sizeof(loadedBytes));
    for (byte j = 0; j < NUM_LEDS; j++) {
      place = (j*3);
      leds[j].r = loadedBytes[place];
      leds[j].g = loadedBytes[place+1];
      leds[j].b = loadedBytes[place+2];
    }
    FastLED.show();  
    delay(30);
  }
}
