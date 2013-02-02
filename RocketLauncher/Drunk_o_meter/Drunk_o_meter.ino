#include <WS2801.h>

#include <RGB_Spiral.h>

#include <SPI.h>

#include <Button.h>

#include <SoftwareSerial.h>

const int rxpin = 2; // unused
const int txpin = 3;

const int rx2pin = 4;
const int tx2pin = 5;

const int LedData = 10;
const int LedClock = 12;

const int button_pin = 8;
Button button;

const int sensor_pin = A0;

const int HiPotPin = A1;
const int LoPotPin = A2;
const int modePin = A5;

Button modeButton;


SoftwareSerial myDisplay(rxpin, txpin);
SoftwareSerial myDisplay2(rx2pin, tx2pin);

RGB_Spiral lamp(LedData, LedClock);

void setup()
{
  Serial.begin(9600);
  myDisplay.begin(9600);
  myDisplay2.begin(9600);
  pinMode(sensor_pin, INPUT);
  pinMode(LoPotPin, INPUT);
  pinMode(HiPotPin, INPUT);
  button.setup(button_pin);
  modeButton.setup(modePin);

  lamp.begin();
  //lamp.setAll(Color(0,0,255));
  lamp.show();
  
  randomSeed(analogRead(0));
}

long count = 0;

long max_count = 0;

int incr = 1;
  
char s[5];

bool decimal_set = false;
uint32_t hi = 5000;
uint32_t lo = 0;

// displayMode
// 0 = Breathalyzer
// 1 = pretty blinking lights

int displayMode = 0;
int pattern = 0;
void loop()
{
  static unsigned long lastLoop = 0;
  static unsigned int elapsed = 0;
  elapsed += millis() - lastLoop;
  lastLoop = millis();
  
  modeButton.update();
    
  if(elapsed >= 100)
  {
    lo = analogRead(LoPotPin);
    hi = analogRead(HiPotPin);
    count = analogRead(sensor_pin);
    if(count > max_count) { max_count = count; }
    elapsed = 0;
    if(modeButton.State() == Button::PRESSED) {
      DisplayNumber(myDisplay, lo);
      DisplayNumber(myDisplay2, hi);
    }
    else {
      DisplayNumber(myDisplay, count);
      DisplayNumber(myDisplay2, max_count);
    }
  }
  
  button.update();
  
  if(button.JustPressed()) {
    max_count = count;
    //Serial.println("PRESTO!");
  }

  if(modeButton.JustReleased()) {
    // Serial.println("End SetHiLo");
    displayMode++;
    if(displayMode > 1) displayMode = 0;
  }
  if(hi == 5000 || modeButton.State() == Button::PRESSED) {
    if(modeButton.JustPressed()) {
      lamp.endMode();
    }
    
    SetHiLo();
  }
  else {
    //Serial.println(count);
    
    if(displayMode == 0)
    {
      SetLamp();
      delay(100);
    }
    else if(lamp.Mode() == RGB_Spiral::NONE) {
      //Serial.print("Changing Pattern: ");
      //Serial.print(pattern);
      //Serial.println("");

      switch(pattern)
      {
        case 0:
          lamp.colorWipe(lamp.Wheel(random(0,256)), 50);
          break;
        case 1:
          lamp.colorWipe(Color(128, 128, 128), 50);
          break;
        case 2:
          lamp.rainbow(30);
          break;
        case 3:
          lamp.rainbowCycle(10);
          break;
        default:
          break;
      }
      int p = pattern;
      while(p == pattern) pattern = random(4);
    }
  }
  lamp.update();
}

//float PI = 3.1415;

void SetLamp()
{
  // figure out how high the red should climb
  //
  if(lo > hi) {
    int t = lo;
    lo = hi;
    hi = t;
  }

  int h = 0;
  if(count < lo) {
    h = 0;
  }
  else if (count > hi) {
    h = 19;
  }
  else {
    h = map(constrain(count, lo, hi), lo, hi, 0, 19);
  }
  
  for(int i = 0; i < 20; i++) {
    uint8_t r, b;
    
    /*
    float r = 0.0;
    float g = 1.0;
    float b = 0.0;
    
    float theta = 0;
    if(i <= h) {
      theta = (float)i/(float)h * PI/2.0;
      //map((float)i, 0.0, (float)h, 0.0, PI/2.0);
    }
    else {
      theta = PI/2.0 + ((float)(i - h)/(19.0 - h)) * PI/2.0;
      //map((float)i, (float)h, 19.0, PI/2.0, PI);
    }
    float scale = 1.0 - ((cos(theta) + 1.0) / 2.0);
    b = scale * 255.0;
    //map(scale, 0.0, 1.0, 0, 255);
    //g = 0;
    r = (1.0 - scale) * 255.0;
    //map(scale, 0.0, 1.0, 255, 0);
    */
    if(i < h -1) {
      r = 255;
      b = 0;
    }
    else if(i == h - 1){
      r = (255/3)*2;
      b = 255/3;
    }
    else if(i == h) {
      r= 255;
      b = 255;
    }
    else if(i == h+1) {
      r = 255/3;
      b = (255/3)*2;
    }
    else {
      r = 0;
      b = 255;
    }
    lamp.setPixelColor(i, Color((uint8_t)r,0,(uint8_t)b));
/*
    Serial.print("lo: "); Serial.print(lo);
    Serial.print(", hi: "); Serial.print(hi);
    Serial.print(", count: "); Serial.print(count);
    Serial.print(", h: "); Serial.print(h);
    Serial.print(", theta: "); Serial.print(theta);
    Serial.print(", scale: "); Serial.print(scale);
    Serial.print(", r: "); Serial.print(r);
    Serial.print(", b: "); Serial.print(b);
    Serial.println("");
    */
  }
  //Serial.println("--------------------------------------------------\n");
  lamp.show();
  delay(10);
}

void SetHiLo()
{
  lamp.setAll(Color(128, 128, 128));
  lamp.show();
  
  //lo = analogRead(LoPotPin);
  //hi = analogRead(HiPotPin);
}

void DisplayNumber(SoftwareSerial& display, long count)
{
  long  a,b,c,d;
  
  // use itoa?
  
  a = count/1000;
  b = (count - (a * 1000))/100;
  c = (count - (a * 1000) - (b * 100))/10;
  d = count - (a * 1000) - (b * 100) - (c * 10);
  
  s[0] = a == 0 ? ' ' : (unsigned char)a + '0';
  s[1] = b == 0 && a == 0 ? ' ' : (unsigned char)b + '0';
  s[2] = c == 0 && b == 0 && a == 0 ? ' ' : (unsigned char)c + '0';
  s[3] = (unsigned char) d + '0';
  s[4] = 0;
  display.print(s);
}

