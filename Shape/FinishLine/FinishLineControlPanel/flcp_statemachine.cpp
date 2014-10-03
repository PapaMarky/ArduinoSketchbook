#include <Arduino.h>

#include "flcp_statemachine.h"

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
  for (int i = _nEvents; i > 0; i--) {
    _eventQueue[i] = _eventQueue[i - 1];
  }
  _eventQueue[0] = event;
  
  if (_nEvents < EVENT_QUEUE_SIZE) {
    _nEvents++;
  }
}
