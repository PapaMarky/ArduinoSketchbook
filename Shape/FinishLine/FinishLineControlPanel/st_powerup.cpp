
#include <Arduino.h>

#include "flcp_state.h"
#include "DataBase.h"

extern DataBase data;

void StatePowerUp::OnEnter() {
  Serial.println("StatePowerUp::OnEnter()");
  _start = millis();
  //lcd.setText("Scooter Madness", "v1.0");
}

void StatePowerUp::loop() {
  if (data.isReady()) {
    // update LCD
    // fire power-up-done event
  }
}

void StatePowerUp::OnExit() {
}

bool StatePowerUp::OnEvent(int event) {
}

