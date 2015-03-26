#include <Arduino.h>

#include "button_id.h"
#include "flcp_statemachine.h"


void StateMachine::onButtonEvent(int button_id, int event) {
  Serial.println("GOT BUTTON EVENT");
  if (button_id == GO_BUTTON) {
    // send a message
    Serial.println(" -- GO_BUTTON");
    if (event == BUTTON_RELEASED) {
      Serial.println("   -- RELEASED");
      _state->OnGoButton();
    }
  }
}

void StateMachine::loop() {
  uint32_t now = millis();
  _state->loop(now);
}

// Sketch uses 12,568 bytes (38%) of program storage space. Maximum is 32,256 bytes.
// Global variables use 1,285 bytes (62%) of dynamic memory,
// leaving 763 bytes for local variables. Maximum is 2,048 bytes.

void StateMachine::set_state(State* state) {
  char b[32];
  snprintf(b,32,"set_state(%p)",state);
  Serial.println(b);
  if (_state != 0) {
    _state->OnExit();
  }
  _state = state;
  _state->OnEnter();
}

void StateMachine::handleMessage(uint16_t cmd, uint8_t len, byte* buffer) {
  //char b[32];
  //snprintf(b, 32, "MSG: cmd: %02d, len: %d, '%s'", cmd, len, (char*)buffer);
  //  gdbg->DEBUG(b);
}

