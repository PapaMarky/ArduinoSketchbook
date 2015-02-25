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

class StatePowerUp : public State {
  public:
  void OnEnter();
  void OnExit();
  void loop();
  bool OnEvent(int event);
  char* name() { return "StatePowerUp";}
  
  private:
  unsigned long _start;
  bool _lcd_ready;
  uint16_t lcd_start_wait;
};

static StatePowerUp g_stPowerUp;

class StateCheckDisk : public State {
  public:
  void OnEnter();
  void OnExit();
  void loop();
  bool OnEvent(int event);
};

class StateAquireLaser : public State {
  public:
  void OnEnter();
  void OnExit();
  void loop();
  bool OnEvent(int event);
};

#endif // FLCP_STATE_H
