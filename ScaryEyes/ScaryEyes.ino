#include <Tlc5940.h>
#include <tlc_animations.h>
#include <tlc_config.h>
#include <tlc_fades.h>
#include <tlc_progmem_utils.h>
#include <tlc_servos.h>
#include <tlc_shifts.h>

#include <ScaryEyes.h>

// Tlc5940 tlc; // USE THE PREDEFINED VARIABLE: Tlc;
ScaryEyes scary_eyes;
unsigned long last_motion = 0;
const int MOTION_SENSOR_PIN = 7;

void setup() {
  Serial.begin(9600);
  Serial.println("Here we go...");

  pinMode(MOTION_SENSOR_PIN, INPUT);
  Tlc.init(4095);
  scary_eyes.setup(&Tlc);
}

bool motion_detected() {
  return digitalRead(MOTION_SENSOR_PIN);
}

void loop() {
  unsigned long now = millis();
  
  if (motion_detected()) 
    last_motion = now;
    
  scary_eyes.update(now, last_motion);
  Tlc.update();
  delay(10);
}

