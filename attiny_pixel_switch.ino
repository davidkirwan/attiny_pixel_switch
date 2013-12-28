#include <Adafruit_NeoPixel.h> 
/*
 WS2811/Neopixel pattern switcher for ATtiny85 (and Arduino)
 Requires Adafruit NeoPixel Library
 WS2811 Signal, Digital Pin 4
 Button, Digital Pin 0
 GPL v3
*/

// Define
#define NUM_LEDS 15
#define DATA_PIN 4
#define BTN_PIN 0
#define BTN_DELAY 50
#define NUM_PATTERNS 5
#define CTR_THRESH 16

// Vars
uint8_t j = 0;
uint8_t pattern=1;
uint8_t buttonState=0;
uint8_t lastPix=0; 
uint8_t myPix=0;
uint8_t direction=1;
uint8_t counter=0;
uint32_t setColor=0;
unsigned long mark;

// Start Strip
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, DATA_PIN, NEO_RGB + NEO_KHZ800);

void setup() {
    pinMode(BTN_PIN, INPUT);     
    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
}

void loop() {
    // if button pressed, advance, set mark
    if(chkBtn(digitalRead(BTN_PIN))) {
       j = 0;
       mark = millis();
       pattern++;
    }
   
    // if pattern greater than #pattern reset
    if (pattern > NUM_PATTERNS) { pattern = 1; }
    
    // choose a pattern
    pickPattern(pattern);
    // set direction
    if (j > 254) { direction = 0; }
    if (j < 1) { direction = 1; }   
}

/* pick a pattern */
void pickPattern(uint8_t var) {
      switch (var) {
        case 1:
          bounceInOut(4,counter,15);
          counter++;
        break;
        case 2:
          rainbowCycle(10);
        break;
        case 3:
          colorFirefly(100);
          counter++;
        break;
        case 4:
          rainbow(10);
          counter++;
        break;
        case 5:
          colorWipe(60);
          counter++;
        break;
      }
      if (direction == 1) { j++;  } else {  j--; }
}

/* check button state */
boolean chkBtn(int buttonState) {
   if (buttonState == HIGH && (millis() - mark) > BTN_DELAY) {
       return true;
    } else {
       return false; 
    }
}

void colorFirefly(int wait) {
        if(myPix != lastPix) {
          if(counter<CTR_THRESH) {
            int colors[3];
            float colorV = sin((6.28/30)*(float)(counter)) *255;
            HSVtoRGB((359/CTR_THRESH)*counter, 255, colorV, colors);
            strip.setPixelColor(myPix, colors[0], colors[1], colors[2]);
           strip.show();
           delay(wait);
          } else {
            lastPix=myPix;
            counter=0;
            colorFast(0,0);
          }
        } else {
          myPix=random(0,strip.numPixels());
        }
	
}

// Fill the dots one after the other with a color
// Modified from Neopixel sketch to break on button press

void colorWipe(uint8_t wait) {
    if(counter < strip.numPixels()) {
        strip.setPixelColor(counter, setColor);
        strip.show();
        delay(wait);
    } else {
      counter=-1;
      // define random color
      setColor=strip.Color(random(255), random(255), random(255));
    }
}

// fast version 
void colorFast(uint32_t c, uint8_t wait) {
    for (uint16_t i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, c);
    }
    strip.show();
    delay(wait);
}

// Rainbow Cycle, modified from Neopixel sketch to break on button press
void rainbowCycle(uint8_t wait) {
    uint16_t i;

    //  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
    // }
}

void rainbow(uint8_t wait) {
    uint16_t i;

    //for(j=0; j<256; j++) {
    for (i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    delay(wait);
    // }
}

// larson scanner to midpoint
void bounceInOut(uint8_t num, uint8_t start,uint8_t wait) {
  colorFast(0,0);
  uint8_t color=200;
  for(int q=0; q < num; q++) {
    if(strip.numPixels()-start >= 0 && start < NUM_LEDS) {
          strip.setPixelColor(start+q, strip.Color(0,color,0));
          strip.setPixelColor(strip.numPixels()-start-q, strip.Color(0,color,0));
      }   
    color=round(color/2.0);
    strip.show();
    delay(wait);
  }
  if(counter > strip.numPixels()) { counter=0; }
}


// helpers 

uint32_t Wheel(byte WheelPos) {
    if (WheelPos < 85) {
        return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
    } else if (WheelPos < 170) {
        WheelPos -= 85;
        return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    } else {
        WheelPos -= 170;
        return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
}


// HSV to RGB colors
// hue: 0-359, sat: 0-255, val (lightness): 0-255
// adapted from http://funkboxing.com/wordpress/?p=1366
void HSVtoRGB(int hue, int sat, int val, int * colors) {
    int r, g, b, base;
    if (sat == 0) { // Achromatic color (gray).
        colors[0] = val;
        colors[1] = val;
        colors[2] = val;
    } else {
        base = ((255 - sat) * val) >> 8;
        switch (hue / 60) {
        case 0:
            colors[0] = val;
            colors[1] = (((val - base) * hue) / 60) + base;
            colors[2] = base;
            break;
        case 1:
            colors[0] = (((val - base) * (60 - (hue % 60))) / 60) + base;
            colors[1] = val;
            colors[2] = base;
            break;
        case 2:
            colors[0] = base;
            colors[1] = val;
            colors[2] = (((val - base) * (hue % 60)) / 60) + base;
            break;
        case 3:
            colors[0] = base;
            colors[1] = (((val - base) * (60 - (hue % 60))) / 60) + base;
            colors[2] = val;
            break;
        case 4:
            colors[0] = (((val - base) * (hue % 60)) / 60) + base;
            colors[1] = base;
            colors[2] = val;
            break;
        case 5:
            colors[0] = val;
            colors[1] = base;
            colors[2] = (((val - base) * (60 - (hue % 60))) / 60) + base;
            break;
        }

    }
}

