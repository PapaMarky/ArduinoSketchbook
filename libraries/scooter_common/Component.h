// Copyright 2015, Mark Dyer

#ifndef _COMPONENT_H_
#define _COMPONENT_H_

#include <arduino.h>
#include "debug.h"

class SoftwareSerial;

class Component {
  public:
  virtual void loop(uint32_t now){};
  virtual void setup(){};
  virtual char* name() {return "base";}
};

#define BUTTON_PRESSED  1000
#define BUTTON_RELEASED 1001

char* event_to_str(int event);

class ButtonManager {
 public:
  ButtonManager() {}
  virtual ~ButtonManager(){}
  virtual void onButtonEvent(int button_id, int event) = 0;
};

#define BUTTON_BOUNCE_DELAY 10
class ButtonComponent : public Component {
 public:
 ButtonComponent(int pin, int id, ButtonManager* manager)
   :  _closed(false), _state_start(0), _pin(pin), _id(id), _manager(manager) {
  }

  virtual char* name() {return "btn";}
  void setup() {
    pinMode(_pin, INPUT);
  }

  void loop(uint32_t now);

 private:
  bool _closed;
  uint32_t _state_start;
  int _pin;
  int _id;
  ButtonManager* _manager;
};

class AnalogOutComponent : public Component {
 public:
 AnalogOutComponent(int pin):_state(VALUE), _value(0), _pin(pin) {}
  void setup() {
    pinMode(_pin, OUTPUT);
  }
  virtual char* name() {return "aout";}
  void loop(uint32_t now);

  void setValue(uint8_t value);
  void setFadeUp(uint8_t value, uint32_t duration);
  void setFadeDown(uint8_t value, uint32_t duration);
 private:
  enum V_STATE {VALUE, FADE_UP, FADE_DOWN};

  V_STATE _state;
  uint8_t _value;
  uint8_t _fade_target;
  uint32_t _fade_duration;
  uint32_t _fade_start;
  int _pin;
};
#endif // _COMPONENT_H_

