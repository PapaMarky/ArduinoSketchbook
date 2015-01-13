#include <Arduino>
#include "Wire.h"
#include <RTClib.h>

#include <Adafruit_NeoPixel.h>

#define PIN 6
#define LED_COUNT 288
// I had to cut 18 leds off the inner strip and put them on the outer strip
// to make the circumfrences work out. 
// 144 + 18 = 162
// 144 - 18 = 126
#define LED_OUTER 162
#define LED_INNER 126
// both strips wind clockwise starting at 6 o'clock.

// RTC stuff
#define DS1307_I2C_ADDRESS 0x68  // This is the I2C address
// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val)
{
  return ( (val/10*16) + (val%10) );
}
 
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return ( (val/16*10) + (val%16) );
}
 
// Gets the date and time from the ds1307 and prints result
byte zero;
byte second = 0, minute = 0, hour = 0, dayOfWeek = 0, dayOfMonth = 0, month = 0, year = 0;
char  *Day[] = {"","Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
char  *Mon[] = {"","Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
void getDateDs1307()
{
   Serial.println("GET DATE");

  // Reset the register pointer
  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  Wire.write(zero);
  Wire.endTransmission();
 
  Wire.requestFrom(DS1307_I2C_ADDRESS, 7);
 
  // A few of these need masks because certain bits are control bits
  /*
  while(!Wire.available())
  ;
*/
  second     = bcdToDec(Wire.read() & 0x7f);
  minute     = bcdToDec(Wire.read());
  hour       = bcdToDec(Wire.read() & 0x3f);  // Need to change this if 12 hour am/pm
  dayOfWeek  = bcdToDec(Wire.read());
  dayOfMonth = bcdToDec(Wire.read());
  month      = bcdToDec(Wire.read());
  year       = bcdToDec(Wire.read());
 
  if (hour < 10)
    Serial.print("0");
  Serial.print(hour, DEC);
  Serial.print(":");
  if (minute < 10)
    Serial.print("0");
  Serial.print(minute, DEC);
  Serial.print(":");
  if (second < 10)
    Serial.print("0");
  Serial.print(second, DEC);
  /*
  Serial.print("  ");
  Serial.print(Day[dayOfWeek]);
  Serial.print(", ");
  Serial.print(dayOfMonth, DEC);
  Serial.print(" ");
  Serial.print(Mon[month]);
  Serial.print(" 20");
  if (year < 10)
    Serial.print("0");
  Serial.println(year, DEC);
 */
 Serial.println("");
}
 
void setDateDs1307(byte year, byte month, byte day, byte dayofweek, byte hour, byte minute, byte second) {
  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  Wire.write(zero);
  Wire.write(decToBcd(second) & 0x7f);
  Wire.write(decToBcd(minute));
  Wire.write(decToBcd(hour));
  Wire.write(decToBcd(dayofweek));
  Wire.write(decToBcd(day));
  Wire.write(decToBcd(month));
  Wire.write(decToBcd(year));
  Wire.endTransmission();
}



// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

float inner_pos[LED_INNER];

void setup() {
  // precalculate values
  for(int i = 0; i < LED_INNER; i++) {
    inner_pos[i] = (float)i / (float)LED_INNER;
  }
  /*
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  Wire.begin();
  zero=0x00;
  */
  Serial.begin(9600);
}

long lastUpdate = 0;

boolean timeSet = false;
byte lastSecond = -1;
long secondStart = 0;

void loop() {
  if (!timeSet) {
    //setDateDs1307(14, 7, 26, 7, 14, 35, 0);
    timeSet = true;
  }
  
  long now = millis();
  if (now - lastUpdate >= 1000) {
    lastUpdate = now;
    getDateDs1307();
    
    if(lastSecond != second) {
      lastSecond = second;
      secondStart = now;
    }
    float partSecond = (float)(now - secondStart)/1000.0;
    showTime(hour, minute, (float)second + partSecond);
  }
  // Some example procedures showing how to display to the pixels:
  /*
  colorWipe(strip.Color(255, 0, 0), 50); // Red
  colorWipe(strip.Color(0, 255, 0), 50); // Green
  colorWipe(strip.Color(0, 0, 255), 50); // Blue
  // Send a theater pixel chase in...
  theaterChase(strip.Color(127, 127, 127), 50); // White
  theaterChase(strip.Color(127,   0,   0), 50); // Red
  theaterChase(strip.Color(  0,   0, 127), 50); // Blue

  rainbow(20);
  rainbowCycle(20);
  theaterChaseRainbow(50);
  */
}

void showTime(int h, int m, float s) {
  uint16_t i = 0;
  
  Serial.print("showTime: h: ");
  Serial.print(h);
  Serial.print(", m: ");  Serial.print(m);
  Serial.print(", s: "); Serial.print(s);
  Serial.println("");

return;
  s = s - 30;
  while(s < 0) s += 60;
  float spos = ((float)s / 60.0) * (float)LED_OUTER;
  int S = floor(spos);
  
  uint32_t color;
  while(i < LED_OUTER) {
    //Serial.print(".");
    
    float delta = abs((float)spos - inner_pos[i]);
    // TODO handle edge cases
    /*
    if (delta <= 1.0/60.0) {
      int n = (delta * 60.0) * 255.0;
      color = strip.Color(n, n, n);
    }
    */
    if (S == i) {
      color = strip.Color(255, 255, 255);
    }
    else if (abs(i - S) < 2) {
      color = strip.Color(255, 255, 255);
    } else {
      color = strip.Color(32, 0, 32);
    }
    strip.setPixelColor(i + LED_INNER, color);
    i++;
  }
  //Serial.println("");
  
  
  // convert h and m to pixel positions
  m = m - 30;
  while(m < 0) m += 60;
  float mpos = ((float)m / 60.0) * (float)(LED_INNER);
  int M = LED_INNER - 1 - floor(mpos);
  
  while(h >= 12) h -= 12; // normalize to 12 hr clock
  h = h - 6; 
  while(h < 0) h += 12;
  float hpos = ((float)h / 12.0) * (float)(LED_INNER);
  int H = LED_INNER - 1 - floor(hpos);
  
  int lo = (M <= H) ? M : H;
  int hi = (M > H) ? M : H;
  
  int noon = LED_INNER - LED_INNER/2;
  int three = LED_INNER - floor(9.0/12.0 * (float)LED_INNER);
  i = 0;
  Serial.print("h: "); Serial.print(h);
  Serial.print(", m: "); Serial.print(m);
  Serial.print(", H: "); Serial.print(H);
  Serial.print(", M: "); Serial.print(M);
  Serial.print(", lo: "); Serial.print(lo);
  Serial.print(", hi: "); Serial.print(hi);
  Serial.println("");
  uint32_t black = strip.Color(0,0,0);
  
  while(i < LED_INNER) {
    /*
    if (i == noon) {
      color = strip.Color(255, 0, 255);
    } else if (i == three) {
      color = strip.Color(0, 255, 255);
    } else */ 
    
    if (i == M) {
      color = strip.Color(0, 0, 255);
    } else if (i == H) {
      color = strip.Color(255, 255, 0);
    } else if (i < lo) {
      color = strip.Color(32, 0, 0);
      //Serial.print("^");
    } else if (i < hi) {
      color = strip.Color(0, 32, 0);
      //Serial.print("*");
    } else {
      color = strip.Color(32, 0, 0);
      //Serial.print("-");
    }
    strip.setPixelColor(i, color);
    i++;
  }
  strip.show();
  //Serial.println("");
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();
     
      delay(wait);
     
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
        }
        strip.show();
       
        delay(wait);
       
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, 0);        //turn every third pixel off
        }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

