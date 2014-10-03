#ifndef FLCP_STATE_H
#define FLCP_STATE_H

#include "LCD.h"
extern LCD lcd;

// States
#define ST_POWER_UP 0
#define ST_LASER_READY 1
#define ST_READY 2

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
