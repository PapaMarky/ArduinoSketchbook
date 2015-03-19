// copyright 2015, Mark Dyer
#ifndef BASE_COMPONENT_H
#define BASE_COMPONENT_H

#include <SerialComponent.h>

class Controller;

class BaseComponent : public SerialComponent {
 public:
  void handleMessage(uint8_t cmd, uint8_t len, byte* buffer);
  void setController(Controller* c) { _controller = c; }
 private:
  Controller* _controller;
};


#endif // BASE_COMPONENT_H
