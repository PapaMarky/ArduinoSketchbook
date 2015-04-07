// Copyright 2015, Mark Dyer
#include <LiquidCrystal.h>

#include "strings.h"
#include "screen.h"

#include "SerialComponent.h"

void ResultsScreen::setElapsedTime(uint32_t elapsed) {
  TimingScreen::elapsedToString(elapsed, buffer);
}

void ResultsScreen::update(uint32_t now) {
}

void ResultsScreen::onEnter() {
  go_button_led.on();
  _lcd->setCursor(0,2);
  _lcd->print("      FINISH!       ");
  _lcd->setCursor(0,3);
  _lcd->print(" Press 'Go' Button  ");
  green_led.off();
}

bool ResultsScreen::onMessage(uint16_t cmd, uint8_t len, byte* buffer) {
  switch(cmd) {
  case SerialComponent::msg_results_done:
    g_controller.setScreen(g_ready_screen);
    return true;
  }
  return false;
}
