// copyright 2015, Mark Dyer
#include <SerialComponent.h>

#include "Controller.h"
#include "BaseComponent.h"
#include "screen.h"
#include "debug_lcd.h"
#include "button_id.h"


char* button_id_str(int id) {
  switch(id) {
  case SELECT_BUTTON:
    return "SELECT";
  case RIGHT_BUTTON:
    return "RIGHT";
  case LEFT_BUTTON:
    return "LEFT";
  case UP_BUTTON:
    return "UP";
  case DOWN_BUTTON:
    return "DOWN";
  }
}

extern StartupScreen startup_screen;

extern AnalogOutComponent red_led;
extern AnalogOutComponent yellow_led;
extern AnalogOutComponent green_led;
extern AnalogOutComponent go_button_led;
extern BaseComponent base;

void Controller::onButtonEvent(int button_id, int event) {
  char b[22];
  snprintf(b, 21, "evt: %s %s", button_id_str(button_id), event_to_str(event));
  gdbg->DEBUG(b);
  if (_screen->type() == Screen::scr_console) {
    char b[21];
    sprintf(b, "%s %s", button_id_str(button_id), event_to_str(event));
    ((ConsoleScreen*)_screen)->addLine(b);
  }
}

void Controller::onMessage(uint16_t cmd, uint8_t len, byte* buffer) {
  if (cmd == SerialComponent::msg_base_hello && _screen != g_startup_screen) {
    setScreen(g_startup_screen);
    g_startup_screen->set_line(1, "       RESET!       ");
      g_startup_screen->onMessage(cmd, len, buffer);

  }
  else {
    if(! _screen->onMessage(cmd, len, buffer)) {
      // ruh roh
    }
  }
}

void Controller::init() {
  _n_components = 0;
  _screen = 0;
}

void Controller::setup() {
  char buff[22];
  gdbg->DEBUG("Controller::setup");
  for (int i = 0; i < nComponents(); i++) {
    _components[i]->setup();
  }

  red_led.setValue(0);
  yellow_led.setValue(0);
  green_led.setValue(0);
}

void Controller::loop(uint32_t now) {
  char buff[22];
  for (int i = 0; i < nComponents(); i++) {
    _components[i]->loop(now);
  }
  if (_screen) {
    _screen->update(now);
    _screen->draw();
  }
}

void Controller::addComponent(Component* component) {
  if (nComponents() > 10) {
    gdbg->DEBUG("too many components");
    return;
  }

  _components[nComponents()] = component;
  _n_components++;
}


void Controller::setScreen(Screen* s) {
  if (_screen != 0) {
    _screen->onLeave();
  }

  _screen = s;
  _screen->onEnter();
}

