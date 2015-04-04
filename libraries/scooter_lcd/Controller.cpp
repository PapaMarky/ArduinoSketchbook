// copyright 2015, Mark Dyer
#include <SerialComponent.h>
//#include <LiquidCrystal.h>

#include "Controller.h"
#include "BaseComponent.h"
#include "screen.h"
#include "debug_lcd.h"
#include "button_id.h"
#include "strings.h"

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
  if (_screen->type() == Screen::scr_console) {
    char b[21];
    sprintf(b, "%s %s", button_id_str(button_id), event_to_str(event));
    ((ConsoleScreen*)_screen)->addLine(b);
  }
}

void Controller::onFoul(char* msg) {
  g_foul_screen->set_message(msg);
  setScreen(g_foul_screen);
}

// extern LiquidCrystal lcd;

void Controller::onMessage(uint16_t cmd, uint8_t len, byte* buffer) {
  /*
  char buf[21];
  snprintf(buf, 21, "CMD: %02x, len: %d", cmd, len);
  lcd.setCursor(0,2);
  lcd.print("                    ");
  lcd.setCursor(0,2);
  lcd.print(buf);
  delay(1000);
  */
  if (cmd == SerialComponent::msg_base_hello && _screen != g_startup_screen) {
    setScreen(g_startup_screen);
    g_startup_screen->set_line(1, s(S_RESET));
    delay(1000);
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
  for (int i = 0; i < nComponents(); i++) {
    _components[i]->setup();
  }

  red_led.off();
  yellow_led.off();
  green_led.off();
  go_button_led.off();
}

void Controller::loop(uint32_t now) {
  for (int i = 0; i < nComponents(); i++) {
    _components[i]->loop(now);
  }
  if (_screen) {
    _screen->update(now);
  }
  //  lcd.setCursor(0,2);
  //  lcd.print("controller loop done");
  //  delay(1000);
}

void Controller::addComponent(Component* component) {
  if (nComponents() >= MAX_COMPONENTS) {
    //    gdbg->DEBUG("too many components");
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

