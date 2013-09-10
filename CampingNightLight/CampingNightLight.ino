#include <Narcoleptic.h>

const int PWM_1 = 3; // the transistor
const int LOW_BATTERY_LED = 4;

const int PHOTOCELL = A0;
const int DIMMER = A1;
const int BATTERY = A2;
const int SW_IN = A3;  // cols
const int SW_OUT = A4; // rows

const float BATTERY_THRESHOLD_ON = 3.5;
void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(PWM_1, OUTPUT);
  pinMode(LOW_BATTERY_LED, OUTPUT);
  pinMode(BATTERY, INPUT);
  pinMode(SW_IN, INPUT);
  pinMode(SW_OUT, OUTPUT);
  pinMode(PHOTOCELL, INPUT);
  //pinMode(DIMMER, INPUT);
  digitalWrite(SW_OUT, HIGH);
}
static float brightness = 255;
const int off_threshhold = 190;
const int on_threshhold = 130;
static bool battery_is_low = false;

static bool switch_on = false;
bool check_switch() {
  Serial.print("check_switch, on: ");
  Serial.println(switch_on ? "true" : "false");
  if (!switch_on) {
    //digitalWrite(SW_OUT, HIGH);
    delay(25);
    // better switch bounce checking?
    if (digitalRead(SW_IN) == LOW) {
      Serial.println(" -- first LOW");
      delay(25);
    }
    if (digitalRead(SW_IN) == LOW) {
      Serial.println(" -- second LOW");
      switch_on = true; 
    }
  }
  else {
    if (digitalRead(SW_IN) == HIGH) {
      delay(25);
      if (digitalRead(SW_IN) == HIGH) {
        switch_on = false;
      }
    }
    //digitalWrite(SW_OUT, LOW);
    delay(25);
  }
  
  return switch_on;
}

void loop() {
  static bool on = false;
  int light = analogRead(PHOTOCELL);
  //Serial.print("light: ");
  //Serial.println(light);
  int d = 1024 - analogRead(DIMMER);
  float dimmer = 1.0;(float)d /1023.0;
  float battery = 5.0 * (float)analogRead(BATTERY)/1024.0;
  bool swtch = check_switch();
  Serial.print("Switch: ");
  Serial.println(swtch ? "on" : "off");
  //Serial.print("battery level: ");
  //Serial.println(battery);
  //battery_is_low = swtch;
  battery = swtch ? 0 : BATTERY_THRESHOLD_ON;
  if (!battery_is_low && battery < BATTERY_THRESHOLD_ON) {
    digitalWrite(LOW_BATTERY_LED, HIGH);
    battery_is_low = true;
  } else if (battery >= BATTERY_THRESHOLD_ON) {
    battery_is_low = false;
    digitalWrite(LOW_BATTERY_LED, LOW);
  }
  
  //Serial.print("battery is low: ");
  //Serial.println(battery_is_low ? "true" : "false");
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
