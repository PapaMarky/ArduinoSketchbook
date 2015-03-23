// copyright 2015, Mark Dyer
#ifndef FLCP_STATE_H
#define FLCP_STATE_H

#include "globals.h"

// States
#define ST_POWER_UP  0
#define ST_COUNTDOWN 1
#define ST_TIMING    2
#define ST_READY     3
#define ST_FOUL      4
#define ST_RESULTS   5
const int NUMBER_OF_STATES = 6;

class State {
  public:
  State() {}
  virtual void OnEnter() {}
  virtual void OnExit() {}
  virtual void loop(uint32_t now) {}

 protected:
};

const int LASER_READY_TIME = 5000;

class StatePowerUp : public State {
  public:
  StatePowerUp() {}

  void OnEnter();
  void OnExit();
  void loop(uint32_t now);

  private:
  bool checkLaserState(uint32_t now);

  unsigned long _start;
  bool _lcd_connected;
  bool _lcd_ready;
  uint32_t lcd_start_wait;

  bool _laser_seen;
  bool _laser_ready;
  uint32_t _laser_aquired_time;
};

class StateReady : public State {
 public:
  StateReady() {}

  void OnEnter();
  void OnExit();
  void loop(uint32_t now);

 private:
  bool _laser_on;
};

class StateCountdown : public State {
 public:
  StateCountdown() {}

  void OnEnter();
  void OnExit();
  void loop(uint32_t now);

 private:
  int _count;
  uint32_t _start_time;
};

class StateTiming : public State {
 public:
  StateTiming() {}

  void OnEnter();
  void OnExit();
  void loop(uint32_t now);

};

class StateResults : public State {
 public:
  StateResults() {}

  void OnEnter();
  void OnExit();
  void loop(uint32_t now);

};

class StateFoul : public State {
 public:
  StateFoul() {}

  void loop(uint32_t now);

  //  char* name() { return "StateFoul";}
};

static StatePowerUp g_stPowerUp;
static StateReady g_stReady;
static StateCountdown g_stCountdown;
static StateTiming g_stTiming;
static StateResults g_stResults;
static StateFoul g_stFoul;

#endif // FLCP_STATE_H

