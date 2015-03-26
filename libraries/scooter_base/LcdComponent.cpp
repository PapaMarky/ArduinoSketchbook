  // copyright 2015, Mark Dyer
#include <string.h>

#include "LcdComponent.h"
#include "flcp_statemachine.h"

extern StateMachine* g_stateMachine;

void LcdComponent::handleMessage(uint16_t cmd, uint8_t len, byte* buffer) {
  _ready = true;
  g_stateMachine->handleMessage(cmd, len, buffer);
}
