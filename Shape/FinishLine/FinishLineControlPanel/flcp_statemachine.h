
// States
#define ST_POWER_UP 1
#define ST_LASER_READY 2
#define ST_READY 3

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
  StateMachine() : currentState(0), nTransitions(N_TRANSITIONS), trans(TRANSITIONS), nEvents(0) { }

  void loop();
  void throwEvent(int event);
 
  void transition(int transition);
  
  private:
  int popEvent();
  int pushEvent(int event);

  int currentState;
  int nTransitions;
  Transition* trans;
  
  int nEvents;
  int eventQueue[16];
};

static StateMachine g_stateMachine;
