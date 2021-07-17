#include <FastLED.h>
#define NUM_LEDS 100
#define DATA_PIN 3
CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<WS2811, DATA_PIN>(leds, NUM_LEDS);
}

void loop() {
  for (int j = 0; j < 256; j++) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(map(i, 0, 99, 0, 255)+j*15, 255, 255);  
    FastLED.show();
  //leds[i] = CRGB::Black;
      
  }
  //delay(10);
  /*
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
    FastLED.show();
    delay(30);
  }
*/
}
}
