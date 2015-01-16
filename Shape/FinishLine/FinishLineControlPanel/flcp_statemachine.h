#ifndef FLCP_STATEMACHINE_H
#define FLCP_STATEMACHINE_H
#include "flcp_state.h"
#include <Arduino.h>

// Transition Events
#define EV_STARTUP 0
#define EV_SELFCHECK_COMPLETE 1
#define EV_SETUP_COMPLETE 2


#define EV_GO_BUTTON 2
#define EV_LASER_AQUIRED 3
#define EV_LASER_LOST 4

struct Transition {
  int state;
  int event;
  int next_state;
};

#define N_TRANSITIONS 2
static Transition TRANSITIONS [N_TRANSITIONS] = {
  {ST_POWER_UP, EV_STARTUP, ST_SELFCHECK},
  {ST_SELFCHECK, EV_SELFCHECK_COMPLETE, ST_SETUPDB},
};

class StateMachine {
  public:
  StateMachine() : _currentState(ST_POWER_UP), _nEvents(0) {
  }
  
  void setup() {
    Serial.println("StateMachine::setup()");
    AddState(&g_stPowerUp, ST_POWER_UP);
    pushEvent(EV_SETUP_COMPLETE);
  }
  
  void AddState(State* s, int n) {
    Serial.print("Adding state "); 
    Serial.println(n);
    Serial.println(s->name());
    _states[n] = s;
  }

  void loop();
 
  void transition(int transition);

  void pushEvent(int event);
  
  private:
  void processEvents();
  
  int popEvent();

  int _currentState;
  
  int _nEvents;
  static const int EVENT_QUEUE_SIZE = 16;
  int _eventQueue[EVENT_QUEUE_SIZE];
  
  int _nStates;
  State* _states[NUMBER_OF_STATES];
};

extern StateMachine g_stateMachine;

#endif // FLCP_STATEMACHINE_H
