// copyright 2015, Mark Dyer
#include <SerialComponent.h>

#include "Controller.h"
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

extern ConsoleScreen startup_screen;

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

void Controller::onMessage(uint8_t cmd, uint8_t len, byte* buffer) {
  switch(cmd) {
  case SerialComponent::msg_heartbeat:
    _screen->onHeartbeat();
    break;

  case SerialComponent::msg_log:
    if (_screen->type() == Screen::scr_console) {
      ((ConsoleScreen*)_screen)->addLine((char*)buffer);
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
    //    snprintf(buff, 21, "setup %d (%s)", i, _components[i]->name());
    //    gdbg->DEBUG(buff);
    _components[i]->setup();
  }
}

void Controller::loop(uint32_t now) {
  char buff[22];
  //  snprintf(buff, 21, "c::loop(%d)", nComponents());
  //  gdbg->DEBUG(buff);
  for (int i = 0; i < nComponents(); i++) {
    //    snprintf(buff, 21, "%d %s", i, _components[i]->name());
    //    gdbg->DEBUG(buff);
    _components[i]->loop(now);
    //    gdbg->DEBUG("loop done");
  }
  if (_screen) {
    //    gdbg->DEBUG("Update screen");
    _screen->update(now);
    //    gdbg->DEBUG("draw screen");
    _screen->draw();
  }
}

void Controller::addComponent(Component* component) {
  char buff[22];
  //  snprintf(buff, 21, "add %d(%s:%p)", nComponents(), component->name(), component);
  //  gdbg->DEBUG(buff);
  if (nComponents() > 10) {
    gdbg->DEBUG("too many already");
    return;
  }

  _components[nComponents()] = component;
  //snprintf(buff, 21, "cmp: %p -> %p", _components[nComponents()], component);
  //gdbg->DEBUG(buff);
  _n_components++;
}


void Controller::setScreen(Screen* s) {
  if (_screen != 0) {
    _screen->onLeave();
  }

  _screen = s;
  _screen->onEnter();
}
