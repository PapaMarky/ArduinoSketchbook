#include <Arduino.h>

#include "flcp_statemachine.h"


void StateMachine::onButtonEvent(int button_id, int event) {
}

void StateMachine::loop() {
  //Serial.println("StateMachine::loop()");
  // process events
  processEvents();
  
  _states[_currentState]->loop();
}

void StateMachine::processEvents() {
  while (_nEvents > 0) {
    int event = popEvent( );
    if (event < 0) return;
    Serial.print("Got Event: ");
    Serial.println(event);
    Serial.print("Current State: ");
    Serial.println(_states[_currentState]->name());
    if (_states[_currentState]->OnEvent(event)) {
      for (int i = 0; i < N_TRANSITIONS; i++) {
        if (TRANSITIONS[i].state == _currentState && TRANSITIONS[i].event == event) {
          _states[_currentState]->OnExit();
          _currentState = TRANSITIONS[i].next_state;
          _states[_currentState]->OnEnter();
        }
      }
    }
  }
}

int StateMachine::popEvent() {
  if (_nEvents <= 0) {
    return -1;
  }
  _nEvents--;
  return _eventQueue[_nEvents];
}

void StateMachine::pushEvent(int event) {
  Serial.print("pushEvent: ");
  Serial.println(event);
  Serial.print("nEvents: "); Serial.println(_nEvents);
  
  for (int i = _nEvents; i > 0 && i < EVENT_QUEUE_SIZE; i--) {
    Serial.print("i: "); Serial.print(i);
    _eventQueue[i] = _eventQueue[i - 1];
  }
  _eventQueue[0] = event;
  
  if (_nEvents < EVENT_QUEUE_SIZE - 1) {
    _nEvents++;
  }
  Serial.print(" - number of events: "); 
  Serial.println(_nEvents);
}

void StateMachine::handleMessage(uint8_t cmd, uint8_t len, byte* buffer) {
  char b[32];
  snprintf(b, 32, "MSG: cmd: %02d, len: %d, '%s'", cmd, len, (char*)buffer);
  gdbg->DEBUG(b);
}

