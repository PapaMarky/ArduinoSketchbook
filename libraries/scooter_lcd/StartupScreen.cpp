// Copyright 2015, Mark Dyer
#include <Arduino.h>
#include <LiquidCrystal.h>

#include "screen.h"
#include "SerialComponent.h"

void StartupScreen::onEnter() {
  _lcd->clear();
  _lcd->setCursor(0, 0);
  _lcd->print("   ScooterBot 1.0   ");
  set_line(1, "Connecting...");
}

bool StartupScreen::onMessage(uint16_t cmd, uint8_t len, byte* buffer) {
  switch(cmd) {
  case SerialComponent::msg_base_hello:
    set_line(1, "Waiting for Laser...");
    red_led.setValue(255);
    base.send_message(SerialComponent::msg_lid_hello);
    base.send_message(SerialComponent::msg_lid_ready);
    return true;

  case SerialComponent::msg_laser_seen:
    yellow_led.setValue(255);
    set_line(1, "Laser Detected");
    return true;

  case SerialComponent::msg_laser_ready:
    green_led.setValue(255);
    set_line(1, "Laser Ready");
    delay(1000);
    return true;

  case SerialComponent::msg_base_ready:
    g_controller.setScreen(g_ready_screen);
    return true;
  }
  return false;
}
