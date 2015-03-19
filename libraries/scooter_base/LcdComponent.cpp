  // copyright 2015, Mark Dyer
#include <string.h>

#include "LcdComponent.h"
#include "flcp_statemachine.h"

#define WAIT_FOR_LCD 1000

extern StateMachine* g_stateMachine;

uint32_t lcd_wait_start = 0;
void LcdComponent::handleMessage(uint8_t cmd, uint8_t len, byte* buffer) {
  g_stateMachine->handleMessage(cmd, len, buffer);
}
