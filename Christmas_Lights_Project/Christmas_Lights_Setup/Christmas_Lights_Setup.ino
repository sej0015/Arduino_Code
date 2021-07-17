#include <FastLED.h>
#define NUM_LEDS 100
#define DATA_PIN 3
CRGB leds[NUM_LEDS];
bool done = false;

void setup() {
  FastLED.addLeds<WS2811, DATA_PIN>(leds, NUM_LEDS);
  pinMode(4, INPUT_PULLUP);
}

void loop() {
  if (digitalRead(4) == LOW && done == false) {
    //done = true;
    for (int j = 0; j < 3; j++) {
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB::White;
      }
      FastLED.show();
      delay(500);
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB::Black;
      }
      FastLED.show();
      delay(500);
    }

    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::White;
      FastLED.show();
      delay(500);
      leds[i] = CRGB::Black;
    }
    FastLED.show();
  }
}
