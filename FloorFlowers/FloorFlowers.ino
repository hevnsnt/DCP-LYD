/*
The sketch is for use for the DefconParties 'FloorFlower' device only.
Please put more infor here someday, ok?

Lots of code stolen from: 
  -FastLED.io DemoReel
  -FastLED Fire2012
  -http://joergwende.wordpress.com
*/

#include "FastLED.h"
#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif
#include <RFduinoBLE.h>

// How many leds in your strip?
#define NUM_LEDS 144
#define DATA_PIN 5

// Define the array of leds
FASTLED_USING_NAMESPACE
#define DATA_PIN    5
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
#define NUM_LEDS    144
#define COOLING  55 // for fire animation
#define SPARKING 120 // for fire animation
CRGB leds[NUM_LEDS];
#define BRIGHTNESS          64
#define FRAMES_PER_SECOND  120

void setup() { 
  delay(3000); // 3 second delay for recovery
  RFduinoBLE.deviceName = "Defcon Lanyard";
  RFduinoBLE.advertisementInterval = 675;
  RFduinoBLE.advertisementData = "LED";
  RFduinoBLE.begin();
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS); // set master brightness control
}

 void loop() {
   
   switch(scene) {
     case 1:
    colorWipe(255, 0, 0, del); // Red
    colorWipe(0, 255, 0, del); // Green
    colorWipe(0, 0, 255, del); // Blue
      break;
    case 2:
   for(int dot = 0; dot < NUM_LEDS; dot++) { 
       leds[dot].r = red;
       leds[dot].g = green;
       leds[dot].b = blue;
       FastLED.show();
      // clear this led for the next time around the loop
      leds[dot] = CRGB::Black;
      delay(del);
    }
    break;
    case 3: 
  // Send a theater pixel chase in...
  theaterChase(127, 127, 127, del); // White
  theaterChase(127,   0,   0, del); // Red
  theaterChase(0,   0, 127, del); // Blue 
  break;
}  
}
    
void RFduinoBLE_onReceive(char *data, int len){
    switch (data[0]) {
    case 1:
      red = data[1];
      green = data[2];
      blue = data[3];
      break;
    case 2:
      del = data[1];
      break;
    case 3:
      scene = data[1];
      break;
    case 4:
      BRIGHTNESS = data[1];
      FastLED.setBrightness( BRIGHTNESS );
      break;
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t r,uint32_t g,uint32_t b, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < NUM_LEDS; i=i+3) {
       //turn every third pixel on
        leds[i+q].r=r;
        leds[i+q].g= g;
        leds[i+q].b= b;
      }
      FastLED.show();
      delay(wait);
      for (int i=0; i < NUM_LEDS; i=i+3) {
        //turn every third pixel off
         leds[i+q].r=0;
         leds[i+q].g= 0;
         leds[i+q].b= 0;
      }
    }
  }
}

void colorWipe(uint32_t r,uint32_t g,uint32_t b, uint8_t wait) {
  for(uint16_t i=0; i<NUM_LEDS; i++) {
   leds[i].r=r;
   leds[i].g= g;
   leds[i].b= b;
      FastLED.show();
      delay(wait);
  }
}
