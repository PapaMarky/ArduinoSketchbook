#include <Adafruit_NeoPixel.h>

#include <NeoDigits.h>

NeoDigitDisplay bdd(2, 0, 6); // BigDigitalDisplay

//static const int NCOLORS = 3;
//uint16_t colors[NCOLORS];
void setup() {
  Serial.begin(9600);
  bdd.setup();
  //colors[0] = bdd.color(255, 0, 0);
  //colors[0] = bdd.color(0, 255, 0);
  //colors[0] = bdd.color(0, 0, 255);
  
}
char count = 0;
void loop() {
  
  unsigned long start = millis();

  int n = count / 10;
  char c = n == 0 ? ' ' : '0' + n;
  bdd.SetDigit(0, c);
  
  n = count % 10;
  c = '0' + n;
  
  bdd.SetDigit(1, c);
  
  bdd.loop();
  count++;
  if (count >= 100) count = 0;
  
  unsigned long elapsed = millis() - start;
  if (elapsed < 1000) {
    delay(1000 - elapsed);
  }
}


