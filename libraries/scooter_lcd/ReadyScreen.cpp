// Copyright 2015, Mark Dyer
#include "strings.h"
#include "screen.h"

#include "SerialComponent.h"

void ReadyScreen::onLaserOn() {
  set_line(1, s(S_MOVE_UP));
  set_line(2, " ");
  go_button_led.off();
}

void ReadyScreen::onLaserOff() {
  set_line(1, s(S_PRESS_GO));
  set_line(2, s(S_START_COUNT));
  go_button_led.on();
}

void ReadyScreen::onEnter() {
  set_line(1, s(S_RUMBLE));
  set_line(2, s(S_BLANK));
  set_line(3, s(S_BLANK));
  onLaserOn();
  red_led.off();
  yellow_led.off();
  green_led.off();
  go_button_led.off();
}

bool ReadyScreen::onMessage(uint16_t cmd, uint8_t len, byte* buffer) {
  switch(cmd) {
  case SerialComponent::msg_laser_on:
    onLaserOn();
    return true;
  case SerialComponent::msg_laser_off:
    onLaserOff();
    return true;
  case SerialComponent::msg_count_5:
    g_controller.setScreen(g_countdown_screen);
    return true;
  }
  return false;
}
