// Copyright 2015, Mark Dyer
#include <Arduino.h>

#include "Component.h"
#include "flcp_state.h"
#include "flcp_statemachine.h"
#include "LcdComponent.h"
#include "LaserAssembly.h"

void StateCountdown::OnEnter() {
  _start_time = millis();
  _count = 5;
}

void StateCountdown::OnExit() {
}

void StateCountdown::loop(uint32_t now) {

  if (g_laser->isVisible()) {
    g_stateMachine->set_state(&g_stFoul);
    return;
  }
  uint32_t elapsed = now - _start_time;
  if (elapsed > 5000 && _count != 0) {
    g_lcd->send_message(SerialComponent::msg_go);
    _count = 0;
    // GO TO StateTiming
    return;
  }
  if (elapsed > 4000 && _count != 1) {
    g_lcd->send_message(SerialComponent::msg_count_1);
    _count = 1;
    return;
  }
  if (elapsed > 3000 && _count != 2) {
    g_lcd->send_message(SerialComponent::msg_count_2);
    _count = 2;
    return;
  }
  if (elapsed > 2000 && _count != 3) {
    g_lcd->send_message(SerialComponent::msg_count_3);
    _count = 3;
    return;
  }
  if (elapsed > 1000 && _count != 4) {
    g_lcd->send_message(SerialComponent::msg_count_4);
    _count = 4;
    return;
  }
}
