#include <SPI.h>

#include <WS2801.h>

const int analogPin = A0;
const int analogPin2 = A2;
const int dataPin = A4;
const int clkPin = A5;

const int nLeds = 20;

WS2801 leds(nLeds, dataPin, clkPin);

void setup()
{
  Serial.begin(9600);
  leds.begin();
  leds.show();
  
  Serial.print("myLog(1023) = ");
  Serial.println(myLog(1023));
}

int base = -1;

void loop()
{
  int mn = 1024;     // mn only decreases
  int mx = 0;        // mx only increases
  int val = 0;
  int mn2 = 1024;     // mn only decreases
  int mx2 = 0;        // mx only increases
  // Perform 10000 reads. Update mn and mx for each one.
  for (int i = 0; i < 250; ++i) {
    val = analogRead(analogPin);
    mn = min(mn, val);
    mx = max(mx, val);
    val = analogRead(analogPin2);
    mn2 = min(mn2, val);
    mx2 = max(mx2, val);
  }

  if(base == -1) {
    base = mx-mn;
  }
  else {
    base = min(mx-mn, base);
  }
  // Send min, max and delta over Serial
  clearLeds();
  setLeds(mx - mn, mx2 - mn2);
  leds.show();
  /*
  drawWaves2(mn, mx, "*", 2);
  drawWaves2(mn2, mx2, "#", 2);
  Serial.println("");
  */
  delay(10);
  /**/
  /**
  Serial.print("m=");
  Serial.print(mn);  
  Serial.print(" M=");
  Serial.print(mx);  
  Serial.print(" D=");
  Serial.print(mx-mn);  
  Serial.print(" C=");
  Serial.print(center);
  Serial.println();  
  **/
}

void clearLeds()
{
  for(int p = 0; p < nLeds; p++) {
    leds.setPixelColor(p, 0);
  }
}

int myLog(int n)
{
  /*
  int log = 0;
  while(n > 0) {
    log++;
    n = n/2;
  }
  return log;
  */
  return n;
}
// color: 0 = r, 1 = g, 2 = b
void setLeds(int d1, int d2)
{
  static int base1 = -1;
  static int base2 = -1;
  
  if(base1 == -1) base1 = d1;
  else base1 = min(d1, base1);
  
  if(base2 == -1) base2 = d2;
  else base2 = min(d2, base2);
  
  const int TOP = 75;
  const int HALF = nLeds/2;
  int v1 = map(myLog(d1 - base1), 0, TOP, 0, HALF-1);
  if(v1 >= HALF) v1 = HALF - 1;
  int v2 = map(myLog(d2 - base2), 0, TOP, 0, HALF-1);
  if(v2 >= HALF) v2 = HALF - 1;
  v2 = nLeds - 1 - v2;
  
  for(int led = 0; led < nLeds; led++) {
    uint8_t r = led <= v1 ? 255 : 0;
    uint8_t g = led >= v2 ? 255 : 0;
    uint8_t b = 0;
    leds.setPixelColor(led, r, g, b);
  }
  
}

void drawWaves2(int mn, int mx, const char* c, int scale)
{
  int d = (mx - mn)/scale;
  for(int i = base; i < d; i++)
  {
    Serial.print(i == base ? "|" : c);
  }
  Serial.println("");
}

void drawWaves(int mn, int mx)
{
  int i = 0;
  int step = 5;
  int center = mn + (mx - mn)/2;
  while(i < mn) {
    Serial.print(i == center ? "|" : " ");
    i += step;
  }
  bool c_shown = false;
  while(i < mx)
  {
    Serial.print(i == center ? "|" : "*");
    i += step;
  }
  Serial.print("C: "); Serial.print(center);
  Serial.println("");
}
