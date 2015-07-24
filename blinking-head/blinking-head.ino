#include "FastLED.h"
#include <RFduinoBLE.h>
// How many leds in your strip?
#define NUM_LEDS 16

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 1
// #define CLOCK_PIN 13
#define DATA_PIN2 2
// Define the array of leds
CRGB leds[NUM_LEDS];
int green = 0;
int red = 255;
int blue = 0;
int del = 30;
int scene = 1;
int BRIGHTNESS = 100;

void setup() { 
  RFduinoBLE.deviceName = "BlinkHead";
  RFduinoBLE.advertisementInterval = 675;
  RFduinoBLE.advertisementData = "LED";
  RFduinoBLE.begin();
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, DATA_PIN2>(leds, NUM_LEDS);
  FastLED.setBrightness( BRIGHTNESS );
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
