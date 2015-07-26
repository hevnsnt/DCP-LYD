/*
The sketch is for use for the DefconParties 'FloorFlower' device only.
Please put more infor here someday, ok?

Lots of code stolen from: 
  -FastLED.io DemoReel
  -FastLED Fire2012
  -http://joergwende.wordpress.com

 ::FloorFlowers:: Now depricated, but it does work. You should be using FloorFlowering instead
-Warning it is pretty glitchy
-In order to change patters, send 0301, 0302, 0303, 0304
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
int scene = 1;

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

// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm, Fire2012 };

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

void loop() {
  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 

  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  #define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))   

     switch(scene) {
     case 1:
     confetti();
    /*colorWipe(255, 0, 0, del); // Red
    colorWipe(0, 255, 0, del); // Green
    colorWipe(0, 0, 255, del); // Blue */
      break;
      
    case 2:
    rainbowWithGlitter();
   /*for(int dot = 0; dot < NUM_LEDS; dot++) { 
       leds[dot].r = red;
       leds[dot].g = green;
       leds[dot].b = blue;
       FastLED.show();
      // clear this led for the next time around the loop
      leds[dot] = CRGB::Black;
      delay(del);
    } */
    break;
    case 3: 
    juggle();
  /*// Send a theater pixel chase in...
  theaterChase(127, 127, 127, del); // White
  theaterChase(127,   0,   0, del); // Red
  theaterChase(0,   0, 127, del); // Blue */
  break;
}  
}
  
void RFduinoBLE_onReceive(char *data, int len){
    switch (data[0]) {
    case 1:
      //red = data[1];
      //green = data[2];
      //blue = data[3];
      break;
    case 2:
      //del = data[1];
      break;
    case 3:
      scene = data[1];
      break;
    case 4:
      //BRIGHTNESS = data[1];
      FastLED.setBrightness( BRIGHTNESS );
      break;
  }
}

void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16(13,0,NUM_LEDS);
  leds[pos] += CHSV( gHue, 255, 192);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16(i+7,0,NUM_LEDS)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

void Fire2012()
{
// Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
        leds[j] = HeatColor( heat[j]);
    }
}

