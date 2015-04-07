// Copyright 2015, Mark Dyer
#include <LiquidCrystal.h>

#include "strings.h"
#include "screen.h"

#include "SerialComponent.h"

void FoulScreen::set_message(char* msg) {
  for(int i = 0; i < 20; i++) {
    _msg[i] = msg[i];
    if (msg[i] == '\0')
      break;
  }
}

const uint32_t BLINK_LENGTH = 500;
void FoulScreen::onEnter() {
  _red_on = true;
  red_led.on();
  yellow_led.off();
  green_led.off();
  _blink_start = millis();
  go_button_led.setFadeDown(255, BLINK_LENGTH);
  char* blank = s(S_BLANK);
  _lcd->setCursor(0,1);
  _lcd->print(blank);
  _lcd->setCursor(0,2);
  _lcd->print(blank);
  _lcd->setCursor(0,3);
  _lcd->print(blank);
  set_line(3, _msg);
}

void FoulScreen::update(uint32_t now) {
  if (now - _blink_start > BLINK_LENGTH) {
    _blink_start = now;
    go_button_led.setFadeDown(255, BLINK_LENGTH);
    if (_red_on) {
      red_led.off();
      yellow_led.on();
    } else {
      red_led.on();
      yellow_led.off();
    }
    _red_on = !_red_on;
  }
}

bool FoulScreen::onMessage(uint16_t cmd, uint8_t len, byte* buffer) {
  if (cmd == SerialComponent::msg_foul_done) {
    g_controller.setScreen(g_ready_screen);
    return true;
  }
  return false;
}
