
#include <Arduino.h>

#include "flcp_state.h"

void StatePowerUp::OnEnter() {
  Serial.println("StatePowerUp::OnEnter()");
  _start = millis();
  lcd.setText("Scooter Madness", "v1.0");
}

void StatePowerUp::loop() {
  
}

void StatePowerUp::OnExit() {
}

bool StatePowerUp::OnEvent(int event) {
}

