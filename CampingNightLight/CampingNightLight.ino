#include <Narcoleptic.h>

const int PWM_1 = 3; // the transistor
const int LOW_BATTERY_LED = 4;

const int PHOTOCELL = A0;
const int DIMMER = A1;
const int BATTERY = A2;

const float BATTERY_THRESHOLD_ON = 3.5;
void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(PWM_1, OUTPUT);
  pinMode(LOW_BATTERY_LED, OUTPUT);
  pinMode(BATTERY, INPUT);
  pinMode(PHOTOCELL, INPUT);
  //pinMode(DIMMER, INPUT);
}
static float brightness = 255;
const int off_threshhold = 190;
const int on_threshhold = 130;
static bool battery_is_low = false;
void loop() {
  static bool on = false;
  int light = analogRead(PHOTOCELL);
  //Serial.print("light: ");
  //Serial.println(light);
  int d = 1024 - analogRead(DIMMER);
  float dimmer = 1.0;(float)d /1023.0;
  float battery = 5.0 * (float)analogRead(BATTERY)/1024.0;
  Serial.print("battery level: ");
  Serial.println(battery);
  if (!battery_is_low && battery < BATTERY_THRESHOLD_ON) {
    digitalWrite(LOW_BATTERY_LED, HIGH);
    battery_is_low = true;
  } else if (battery >= BATTERY_THRESHOLD_ON) {
    battery_is_low = false;
    digitalWrite(LOW_BATTERY_LED, LOW);
  }
  Serial.print("battery is low: ");
  Serial.println(battery_is_low ? "true" : "false");
  /*
  Serial.print("dimmer: ");
  Serial.print(d);
  Serial.print(" : ");
  Serial.print(dimmer, 2);
  Serial.println("");
  */
  on = (light < (on ? off_threshhold : on_threshhold));
  int brightness = (int)max(255.0 * dimmer, 1.0);
  int b = on ? brightness : 0;
  //Serial.println(brightness);
  static int last_b = 0;
  if (b != last_b) {
    last_b = b;
    analogWrite(PWM_1, b);
  }
  if (on)
    delay(100);
  else
    delay(1000); // Narcoleptic.delay(1000);
  
}
