#include <Narcoleptic.h>

const int PWM_1 = 3; // the transistor
const int LOW_BATTERY_LED = 4;

const int PHOTOCELL = A0;
const int DIMMER = A1;
const int BATTERY = A2;
const int SW_IN = 8;  // cols
const int SW_OUT = 12; // rows

const int TOUCH_SENSOR = 10;

const float BATTERY_THRESHOLD_ON = 3.5;

uint8_t readCapacitivePin(int pinToMeasure);

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
const int DELAY = 50; // 25
bool check_switch() {
  Serial.print("check_switch, on: ");
  Serial.println(switch_on ? "true" : "false");
  digitalWrite(SW_OUT, HIGH);
  delay(DELAY);
  if (!switch_on) {
    // better switch bounce checking?
    if (digitalRead(SW_IN) == LOW) {
      Serial.println(" -- first LOW");
      delay(DELAY);
    }
    if (digitalRead(SW_IN) == LOW) {
      Serial.println(" -- second LOW");
      switch_on = true; 
    }
  }
  else {
    if (digitalRead(SW_IN) == HIGH) {
      Serial.println(" -- first HIGH");
      delay(DELAY);
      if (digitalRead(SW_IN) == HIGH) {
        Serial.println(" -- second HIGH");
        switch_on = false;
      }
    }
    digitalWrite(SW_OUT, LOW);
    delay(DELAY*2);
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
  uint8_t cap = readCapacitivePin(TOUCH_SENSOR);
  bool swtch = cap > 5; //check_switch();
//  bool swtch = check_switch();
  Serial.print("CAP: ");
  Serial.println(cap);
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
  delay(DELAY*2);
  /*
  if (on)
    delay(10);
  else
    delay(1000); // Narcoleptic.delay(1000);
    */
  
}

// From:
// http://playground.arduino.cc/Code/CapacitiveSensor

// readCapacitivePin
//  Input: Arduino pin number
//  Output: A number, from 0 to 17 expressing
//  how much capacitance is on the pin
//  When you touch the pin, or whatever you have
//  attached to it, the number will get higher
#include "pins_arduino.h" // Arduino pre-1.0 needs this
uint8_t readCapacitivePin(int pinToMeasure) {
  // Variables used to translate from Arduino to AVR pin naming
  volatile uint8_t* port;
  volatile uint8_t* ddr;
  volatile uint8_t* pin;
  // Here we translate the input pin number from
  //  Arduino pin number to the AVR PORT, PIN, DDR,
  //  and which bit of those registers we care about.
  byte bitmask;
  port = portOutputRegister(digitalPinToPort(pinToMeasure));
  ddr = portModeRegister(digitalPinToPort(pinToMeasure));
  bitmask = digitalPinToBitMask(pinToMeasure);
  pin = portInputRegister(digitalPinToPort(pinToMeasure));
  // Discharge the pin first by setting it low and output
  *port &= ~(bitmask);
  *ddr  |= bitmask;
  delay(1);
  // Prevent the timer IRQ from disturbing our measurement
  noInterrupts();
  // Make the pin an input with the internal pull-up on
  *ddr &= ~(bitmask);
  *port |= bitmask;

  // Now see how long the pin to get pulled up. This manual unrolling of the loop
  // decreases the number of hardware cycles between each read of the pin,
  // thus increasing sensitivity.
  uint8_t cycles = 17;
       if (*pin & bitmask) { cycles =  0;}
  else if (*pin & bitmask) { cycles =  1;}
  else if (*pin & bitmask) { cycles =  2;}
  else if (*pin & bitmask) { cycles =  3;}
  else if (*pin & bitmask) { cycles =  4;}
  else if (*pin & bitmask) { cycles =  5;}
  else if (*pin & bitmask) { cycles =  6;}
  else if (*pin & bitmask) { cycles =  7;}
  else if (*pin & bitmask) { cycles =  8;}
  else if (*pin & bitmask) { cycles =  9;}
  else if (*pin & bitmask) { cycles = 10;}
  else if (*pin & bitmask) { cycles = 11;}
  else if (*pin & bitmask) { cycles = 12;}
  else if (*pin & bitmask) { cycles = 13;}
  else if (*pin & bitmask) { cycles = 14;}
  else if (*pin & bitmask) { cycles = 15;}
  else if (*pin & bitmask) { cycles = 16;}

  // End of timing-critical section
  interrupts();

  // Discharge the pin again by setting it low and output
  //  It's important to leave the pins low if you want to 
  //  be able to touch more than 1 sensor at a time - if
  //  the sensor is left pulled high, when you touch
  //  two sensors, your body will transfer the charge between
  //  sensors.
  *port &= ~(bitmask);
  *ddr  |= bitmask;

  return cycles;
}

