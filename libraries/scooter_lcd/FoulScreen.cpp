// Copyright 2015, Mark Dyer
#include <LiquidCrystal.h>

#include "screen.h"

#include "SerialComponent.h"

void FoulScreen::set_message(char* msg) {
  for(int i = 0; i < 20; i++) {
    _msg[i] = msg[9];
    if (msg[i] == '\0')
      break;
  }
}

const uint32_t BLINK_LENGTH = 500;
void FoulScreen::onEnter() {
  _blink_start = millis();
  go_button_led.setFadeDown(255, BLINK_LENGTH);
  set_line(3, _msg);
}

void FoulScreen::update(uint32_t now) {
  if (now - _blink_start > BLINK_LENGTH) {
    _blink_start = now;
    //    go_button_led.setFadeDown(255, BLINK_LENGTH);
  }
}

bool FoulScreen::onMessage(uint16_t cmd, uint8_t len, byte* buffer) {
  return true;
}
