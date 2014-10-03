#ifndef FLCP_STATEMACHINE_H
#define FLCP_STATEMACHINE_H
#include "flcp_state.h"
#include <Arduino.h>

// Transition Events
#define EV_GO_BUTTON 1
#define EV_LASER_AQUIRED 2
#define EV_LASER_LOST 3

struct Transition {
  int state;
  int event;
  int next_state;
};

#define N_TRANSITIONS 3
static Transition TRANSITIONS [N_TRANSITIONS] = {
  {ST_POWER_UP, EV_LASER_AQUIRED, ST_LASER_READY},
  {ST_LASER_READY, EV_LASER_LOST, ST_POWER_UP},
  {ST_LASER_READY, EV_GO_BUTTON, ST_READY}
};

class StateMachine {
  public:
  StateMachine() : _currentState(0), _nEvents(0) {
    Serial.println("StateMachine CTOR");
    AddState(&g_stPowerUp, ST_POWER_UP);
    
    _states[_currentState]->OnEnter();
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

extern StateMachine* g_stateMachine;

#endif // FLCP_STATEMACHINE_H
