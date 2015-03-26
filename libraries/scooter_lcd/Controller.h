// copyright 2015, Mark Dyer
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Component.h>

class Screen;

const int MAX_COMPONENTS = 10;

class Controller: public ButtonManager {
 public:
 Controller(): _n_components(0), _screen(0), _state(st_startup) {}
 ~Controller() {}

  void init();

  void setup();
  void loop(uint32_t now);

  void addComponent(Component* component);

  void onButtonEvent(int button_id, int event);
  void onMessage(uint16_t cmd, uint8_t len, byte* buffer);

  void setScreen(Screen* s);
  Screen* getScreen() { return _screen; }
  int nComponents() { return _n_components;}

 private:
  enum State {
    st_startup,
    st_ready,
    st_countdown,
    st_racing,
    st_showresults
  };
  int _n_components;
  Screen* _screen;
  Component* _components[10];
  State _state;
};


#endif // CONTROLLER_H
