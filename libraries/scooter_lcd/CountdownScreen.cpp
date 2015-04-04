// Copyright 2015, Mark Dyer
#include <LiquidCrystal.h>

#include "screen.h"
#include "strings.h"

#include "SerialComponent.h"

void CountdownScreen::update(uint32_t now) {
  if (_ndots < 4 && (now - _dot_start) > 250) {
    _lcd->print(".");
    _ndots++;
    _dot_start = now;
  }
}

void CountdownScreen::onEnter() {
  _lcd->setCursor(0, 2);
  _lcd->print(s(S_BLANK));
  _lcd->setCursor(0, 3);
  _lcd->print(s(S_BLANK));
  _lcd->setCursor(0, 2);
  _lcd->print("5");
  _n = 0;
  _ndots = 0;
  _dot_start = millis();
}

int NUM = 0;
bool CountdownScreen::onMessage(uint16_t cmd, uint8_t len, byte* buffer) {
  char* n = "";
  NUM++;
  //char b[22];
  //snprintf(b,22,"0x%02x (%03d): %06d", cmd, cmd, NUM);
  //_lcd->setCursor(0, 3);
  //_lcd->print(b);
  switch(cmd) {
  case SerialComponent::msg_count_4:
    _n = 1;
    n = "4";
    break;
  case SerialComponent::msg_count_3:
    _n = 2;
    n = "3";
    break;
  case SerialComponent::msg_count_2:
    _n = 3;
    n = "2";
    break;
  case SerialComponent::msg_count_1:
    _n = 4;
    n = "1";
    break;
  case SerialComponent::msg_go:
    // move to timing screen
    set_line(3, s(S_GO));
    return true;
  case SerialComponent::msg_jump_start:
    g_controller.onFoul(s(S_FALSE_START));
    return true;
  default:
    return false;
  }
  // finish dots
  _lcd->setCursor((_n - 1) * 4 + _ndots, 2);
  while(_ndots < 4) {
    _ndots++;
    _lcd->print(".");
  }
  int pos = _n * 4;
  _ndots = 0;
  _dot_start = millis();
  _lcd->setCursor(pos, 2);
  _lcd->print(n);
  return true;
}
