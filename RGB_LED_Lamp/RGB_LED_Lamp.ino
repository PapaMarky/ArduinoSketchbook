#include <Button.h>

#include <SPI.h>

#include <WS2801.h>

const int nPoles = 5;
const int nLayers = 4;

const int poles[nPoles][nLayers] = {
  {0, 5, 10, 15},
  {3, 8, 13, 18},
  {1, 6, 11, 16},
  {4, 9, 14, 19},
  {2, 7, 12, 17},
};

const int layers[nLayers][nPoles] = {
  { 0,  1,  2,  3,  4},
  { 5,  6,  7,  8,  9},
  {10, 11, 12, 13, 14},
  {15, 16, 17, 18, 19}, 
};

const int dataPin = 2; //(green)
const int clockPin = 3; //(blue)
const int buttonPin = 7;
const int nLEDs = 20;

WS2801 strip = WS2801(nLEDs, dataPin, clockPin);
Button button = Button();

void setup(){
  Serial.begin(9600);
  strip.begin();
  strip.show();
  button.setup(buttonPin);
  
}

void loop(){
  //Serial.println("Loop");
  button.update();
  
  /*
  static int p = nPoles;
  
  if(button.JustPressed()) {
    p++;
    if(p >= nPoles) { p = 0; }
    
    allOff();
    lightPole(p, Color(255,0,0)); 
    strip.show();
    
    Serial.print("POLE: ");
    Serial.println(p);
  }
  static int p = 0;
  static int l = 0;
  static int ldir = 1;
  
  static uint32_t bg = Color(0, 255, 0);
  static uint32_t pcolor = Color(255, 0, 0);
  static uint32_t lcolor = Color(0, 255, 255);
  
  setAll(bg);
  lightLayer(l, lcolor);
  lightPole(p, pcolor);
  strip.show();
  
  if(true || button.JustPressed()) {
    p++; if(p >= nPoles) p = 0;
    if(ldir == 1) {
      l++; 
      if(l >= nLayers) {
        l = nLayers - 2;
        ldir = 0;
      }
    }
    else {
      l--;
      if(l <= 0) {
        l = 0;
        ldir = 1;
      }
    }
    delay(100);
  }
  */

  for(int i = 0; i < 10; i++)
  {
    spinPoles(100, Color(255,0,0), Color(255,255,255));
  }
  colorWipe(Color(255, 0, 0), 50);
  colorWipe(Color(0, 255, 0), 50);
  colorWipe(Color(0, 0, 255), 50);
  rainbow(20);
  rainbowCycle(20);
  /* */
}

void setAll(uint32_t color)
{
  for(int i = 0; i < nLEDs; i++) {
    strip.setPixelColor(i, color);
  }
}

void allOff()
{
  setAll(0);
}

void spinPoles(uint8_t wait, uint32_t color, uint32_t bgcolor)
{
  const int nPoles = 5;
  
  for(int pole = 0; pole < nPoles; pole++) {
    setAll(bgcolor);
    lightPole(pole, color);
    strip.show();
    delay(wait);
  }
}

void lightPole(int pole, uint32_t color)
{
  for(int i = 0; i < nLayers; i++) {
    strip.setPixelColor(poles[pole][i], color);
  }
}

void lightLayer(int layer, uint32_t color)
{
  for(int i = 0; i < nPoles; i++) {
    strip.setPixelColor(layers[layer][i], color);
  }
}

void rainbow(uint8_t wait) {
  int i, j;
   
  for (j=0; j < 256; j++) {     // 3 cycles of all 256 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel( (i + j) % 255));
    }  
    strip.show();   // write all the pixels out
    delay(wait);
  }
}

// Slightly different, this one makes the rainbow wheel equally distributed 
// along the chain
void rainbowCycle(uint8_t wait) {
  int i, j;
  
  for (j=0; j < 256 * 5; j++) {     // 5 cycles of all 25 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      // tricky math! we use each pixel as a fraction of the full 96-color wheel
      // (thats the i / strip.numPixels() part)
      // Then add in j which makes the colors go around per pixel
      // the % 96 is to make the wheel cycle around
      strip.setPixelColor(i, Wheel( ((i * 256 / strip.numPixels()) + j) % 256) );
    }  
    strip.show();   // write all the pixels out
    delay(wait);
  }
}

// fill the dots one after the other with said color
// good for testing purposes
void colorWipe(uint32_t c, uint8_t wait) {
  int i;
  
  for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

/* Helper functions */

// Create a 24 bit color value from R,G,B
uint32_t Color(byte r, byte g, byte b)
{
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}

//Input a value 0 to 255 to get a color value.
//The colours are a transition r - g -b - back to r
uint32_t Wheel(byte WheelPos)
{
  if (WheelPos < 85) {
   return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
   WheelPos -= 85;
   return Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170; 
   return Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
