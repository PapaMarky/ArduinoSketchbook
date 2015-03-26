// Copyright 2015, Mark Dyer
#include <Arduino.h>

#include "Component.h"
#include "flcp_state.h"
#include "flcp_statemachine.h"
#include "LcdComponent.h"
#include "LaserAssembly.h"

void StateReady::OnEnter() {
  Serial.println("StateReady::OnEnter()");
  g_lcd->send_message(SerialComponent::msg_base_ready);
  _laser_on = g_laser->isVisible();
}

void StateReady::OnExit() {
}

void StateReady::OnGoButton() {
  Serial.println("StateReady::OnGoButton()");
  if (! g_laser->isVisible()) {
    Serial.println(" -- Laser visible");
    g_stateMachine->set_state(&g_stCountdown);
  }
}

void StateReady::loop(uint32_t now) {
  if (_laser_on && ! g_laser->isVisible()) {
    g_lcd->send_message(SerialComponent::msg_laser_off);
    _laser_on = false;
  }
  else if ( ! _laser_on && g_laser->isVisible()) {
    g_lcd->send_message(SerialComponent::msg_laser_on);
    _laser_on = true;
  }
}
