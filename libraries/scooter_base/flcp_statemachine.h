#ifndef FLCP_STATEMACHINE_H
#define FLCP_STATEMACHINE_H
#include "flcp_state.h"
#include <Component.h>
#include <Arduino.h>

// TODO : move buffer to globals
#define BUF_SIZE 64
extern char buffer[BUF_SIZE];

class StateMachine : public ButtonManager {
  public:

  virtual void onButtonEvent(int button_id, int event);

  void setup() {
    _state = 0;
    set_state(&g_stPowerUp);
  }
  void set_state(State* state);

  void loop();

  void handleMessage(uint8_t cmd, uint8_t len, byte* buffer);

 private:
  State* _state;
};

#endif // FLCP_STATEMACHINE_H
