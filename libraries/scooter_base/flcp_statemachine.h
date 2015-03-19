#ifndef FLCP_STATEMACHINE_H
#define FLCP_STATEMACHINE_H
#include "flcp_state.h"
#include <Component.h>
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

// TODO : move buffer to globals
#define BUF_SIZE 64
extern char buffer[BUF_SIZE];

class StateMachine : public ButtonManager {
  public:

  virtual void onButtonEvent(int button_id, int event);

  void init() {_currentState = ST_POWER_UP; _nEvents = 0; }
  void setup() {
    gdbg->DEBUG("StateMachine::setup() - "); 
    AddState(&g_stPowerUp, ST_POWER_UP);
    gdbg->DEBUG(" -- StateMachine::setup() push EV_SETUP_COMPLETE");
    pushEvent(EV_SETUP_COMPLETE);
  }
  
  void AddState(State* s, int n) {
    snprintf(buffer,BUF_SIZE,"Adding state %d: %s",n, s->name());
    gdbg->DEBUG(buffer);
    _states[n] = s;
  }

  void loop();
 
  void transition(int transition);

  void pushEvent(int event);

  void handleMessage(uint8_t cmd, uint8_t len, byte* buffer);

  private:
  void processEvents();
  
  int popEvent();

  int _currentState;
  
  int _nEvents;
  static const int EVENT_QUEUE_SIZE = 16;
  int _eventQueue[EVENT_QUEUE_SIZE];
  
  State* _states[NUMBER_OF_STATES];
};

#endif // FLCP_STATEMACHINE_H
