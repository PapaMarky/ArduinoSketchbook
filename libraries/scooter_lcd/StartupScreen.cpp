// Copyright 2015, Mark Dyer
#include <Arduino.h>
#include <LiquidCrystal.h>

#include "strings.h"
#include "screen.h"
#include "SerialComponent.h"

void StartupScreen::onEnter() {
  _lcd->clear();
  _lcd->setCursor(0, 0);
  red_led.off();
  yellow_led.off();
  green_led.off();
  go_button_led.off();
  _lcd->print(s(S_SCOOTERBOT));
  set_line(1, s(S_CONNECTING));
}

bool StartupScreen::onMessage(uint16_t cmd, uint8_t len, byte* buffer) {
  switch(cmd) {
  case SerialComponent::msg_base_hello:
    set_line(1, "Waiting for Laser...");
    red_led.on();
    base.send_message(SerialComponent::msg_lid_hello);
    // TODO : make this depend on disk state if that ever works
    base.send_message(SerialComponent::msg_lid_ready);
    return true;

  case SerialComponent::msg_laser_seen:
    yellow_led.on();
    set_line(1, "   Laser Detected   ");
    return true;

  case SerialComponent::msg_laser_ready:
    green_led.on();
    set_line(1, "   Laser Ready      ");
    delay(1000);
    return true;

  case SerialComponent::msg_base_ready:
    //    set_line(1, "Next Screen...");
    delay(500);
    g_controller.setScreen(g_ready_screen);
    return true;
  }
  return false;
}
