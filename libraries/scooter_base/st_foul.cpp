// Copyright 2015, Mark Dyer
#include <Arduino.h>

#include "Component.h"
#include "flcp_state.h"
#include "flcp_statemachine.h"
#include "LcdComponent.h"
#include "LaserAssembly.h"

void StateFoul::OnEnter() {
}

void StateFoul::OnGoButton() {
  g_lcd->send_message(SerialComponent::msg_foul_done);
  g_stateMachine->set_state(&g_stReady);
}

void StateFoul::loop(uint32_t now) {
}
