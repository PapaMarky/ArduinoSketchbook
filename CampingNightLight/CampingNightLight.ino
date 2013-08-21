#include <Narcoleptic.h>

const int PWM_1 = 3;
const int MODE_SWITCH = 2;
const int PHOTOCELL = A0;
const int DIMMER = A1;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(PWM_1, OUTPUT);
  pinMode(MODE_SWITCH, INPUT);
  pinMode(PHOTOCELL, INPUT);
  pinMode(DIMMER, INPUT);
}
static float brightness = 255;
const int off_threshhold = 190;
const int on_threshhold = 130;
void loop() {
  static bool on = false;
  int light = analogRead(PHOTOCELL);
  Serial.print("light: ");
  Serial.println(light);
  int d = 1024 - analogRead(DIMMER);
  float dimmer = (float)d /1023.0;
  int mode = digitalRead(MODE_SWITCH);
  /*
  Serial.print("dimmer: ");
  Serial.print(d);
  Serial.print(" : ");
  Serial.print(dimmer, 2);
  Serial.println("");
  */
  on = (light < (on ? off_threshhold : on_threshhold));
  int brightness = (int)max(255.0 * dimmer, 1.0);
  int b = (mode || on) ? brightness : 0;
  //Serial.println(brightness);
  static int last_b = 0;
  if (b != last_b) {
    last_b = b;
    analogWrite(PWM_1, b);
  }
  if (on)
    delay(100);
  else
    Narcoleptic.delay(1000);
  
}
