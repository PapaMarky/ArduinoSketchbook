// Copyright 2015, Mark Dyer
#include <Arduino.h>

#include "Component.h"
#include "flcp_state.h"
#include "flcp_statemachine.h"
#include "LcdComponent.h"
#include "LaserAssembly.h"

void StateTiming::OnEnter() {
  _start_time = millis();
  _has_left = false;
}


void StateTiming::OnExit() {
}

void StateTiming::loop(uint32_t now) {
  if (!_has_left && g_laser->isVisible()) {
    _has_left = true;
  }
  if (_has_left && !g_laser->isVisible()) {
    uint32_t elapsed = now - _start_time;
    if (elapsed < 5000)
      return;
    char buffer[32];
    int len = snprintf(buffer, 32, "%lu", elapsed);
    if (len > 31) {
      Serial.print("ELAPSED TIME TOO BIG: ");
      Serial.print(elapsed); Serial.print(" (");
      Serial.print(len); Serial.println(" bytes)");
      len = 31;
    }
    g_lcd->send_message(SerialComponent::msg_finish, buffer, len);
    g_stateMachine->set_state(&g_stResults);
  }
  if (!_has_left && now - _start_time > 5000) {
    // foul?
  }
}
