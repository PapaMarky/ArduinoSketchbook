// copyright 2015, Mark Dyer
#ifndef FLCP_STATE_H
#define FLCP_STATE_H

#include "globals.h"

// States
#define ST_POWER_UP 0
#define ST_SELFCHECK 1
#define ST_SETUPDB 2

class State {
  public:
  State() {}
  virtual void OnEnter() {}
  virtual void OnExit() {}
  virtual void loop() {}
  virtual bool OnEvent(int event) { return true; }
  virtual char* name() { return "State"; }
};

const int NUMBER_OF_STATES = 3;
const int LASER_READY_TIME = 5000;

class StatePowerUp : public State {
  public:
  StatePowerUp() {}

  void OnEnter();
  void OnExit();
  void loop();
  bool OnEvent(int event);
  char* name() { return "StatePowerUp";}

  private:
  bool checkLaserState(uint32_t now);

  unsigned long _start;
  bool _lcd_connected;
  uint32_t lcd_start_wait;

  bool _laser_seen;
  bool _laser_ready;
  uint32_t _laser_aquired_time;
};

static StatePowerUp g_stPowerUp;

#endif // FLCP_STATE_H

