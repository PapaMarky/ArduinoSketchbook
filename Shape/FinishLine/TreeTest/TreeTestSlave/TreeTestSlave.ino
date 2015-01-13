#include <TinyWireS.h>
#include <usiTwiSlave.h>


#include <Adafruit_NeoPixel.h>
const int tree_address = 4;
const int NeoPin = A3;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, NeoPin, NEO_GRB + NEO_KHZ800);

uint32_t RED;
uint32_t YELLOW;
uint32_t GREEN;
uint32_t BLACK;

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}

void setup()
{
  TinyWireS.begin(tree_address);
  strip.begin();
  strip.show();
  GREEN = strip.Color(255, 0, 0);
  YELLOW = Wheel(255/6);
  RED= strip.Color(0, 255, 0);
  BLACK = strip.Color(0, 0, 0);
}


void clear() {
  for (int i = 0; i < 8; i++) {
    strip.setPixelColor(i, BLACK);
  }
}

void stage1() {
  clear();
  strip.setPixelColor(7, RED);
  strip.setPixelColor(6, RED);
  strip.show();
}

void stage2() {
  clear();
  strip.setPixelColor(5, YELLOW);
  strip.setPixelColor(4, YELLOW);
  strip.show();
}

void stage3() {
  clear();
  strip.setPixelColor(3, YELLOW);
  strip.setPixelColor(2, YELLOW);
  strip.show();
}

void stage4() {
  clear();
  strip.setPixelColor(1, GREEN);
  strip.setPixelColor(0, GREEN);
  strip.show();
}

void loop()
{
  if (TinyWireS.available() > 0) 
  {
    byte cmd = TinyWireS.receive();
    switch (cmd) {
      case '1':
      stage1();
      break;
      case '2':
      stage2();
      break;
      case '3':
      stage3();
      break;
      case '4':
      stage4();
      break;
      break;
      default:
      break;
    }
  }
}

