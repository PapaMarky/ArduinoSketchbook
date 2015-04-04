// copyright 2015, Mark Dyer
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Component.h>

class Screen;

const int MAX_COMPONENTS = 10;

class Controller: public ButtonManager {
 public:
 Controller(): _n_components(0), _screen(0) {}
 ~Controller() {}

  void init();

  void setup();
  void loop(uint32_t now);

  void addComponent(Component* component);

  void onButtonEvent(int button_id, int event);
  void onMessage(uint16_t cmd, uint8_t len, byte* buffer);
  void onFoul(char* message);

  void setScreen(Screen* s);
  Screen* getScreen() { return _screen; }
  int nComponents() { return _n_components;}

 private:
  int _n_components;
  Screen* _screen;
  Component* _components[MAX_COMPONENTS];
};


#endif // CONTROLLER_H
