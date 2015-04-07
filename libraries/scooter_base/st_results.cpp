// Copyright 2015, Mark Dyer
#include <Arduino.h>

#include "Component.h"
#include "flcp_state.h"
#include "flcp_statemachine.h"
#include "LcdComponent.h"
#include "LaserAssembly.h"

void StateResults::OnEnter() {
}

void StateResults::OnExit() {
}

void StateResults::loop(uint32_t now) {
}

void StateResults::OnGoButton() {
  g_lcd->send_message(SerialComponent::msg_results_done);
  g_stateMachine->set_state(&g_stReady);
}
