// Copyright 2015, Mark Dyer
#include "screen.h"

#include "SerialComponent.h"

void ReadyScreen::onLaserOn() {
  set_line(2, "Move Up to the Line");
  set_line(3, " ");
  go_button_led.off();
}

void ReadyScreen::onLaserOff() {
  set_line(2, "Press Go Button to");
  set_line(3, "Start Countdown");
  go_button_led.on();
}

void ReadyScreen::onEnter() {
  set_line(1, "Ready to Rumble");
}

bool ReadyScreen::onMessage(uint8_t cmd, uint8_t len, byte* buffer) {
  switch(cmd) {
  case SerialComponent::msg_laser_on:
    onLaserOn();
    return true;
  case SerialComponent::msg_laser_off:
    onLaserOff();
    return true;
  case SerialComponent::msg_count_5:
    //    g_controller.setScreen(g_countdown_screen);
    return true;
  }
  return false;
}
