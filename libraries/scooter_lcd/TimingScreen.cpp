// Copyright 2015, Mark Dyer
#include <LiquidCrystal.h>

#include "screen.h"

#include "SerialComponent.h"

#define MS_HOUR   3600000L
#define MS_MINUTE 60000L
#define MS_SECOND 1000L

void TimingScreen::elapsedToString(uint32_t elapsed, char* buffer) {
  uint16_t hours = 0;
  if (elapsed > MS_HOUR) {
    hours = elapsed / MS_HOUR;
    elapsed = elapsed - (hours * MS_HOUR);
  }
  uint16_t minutes = 0;
  if (elapsed > MS_MINUTE) {
    minutes = elapsed / MS_MINUTE;
    elapsed = elapsed - (minutes * MS_MINUTE);
  }

  uint16_t seconds = 0;
  if (elapsed > MS_SECOND) {
    seconds = elapsed / MS_SECOND;
    elapsed = elapsed - (seconds * MS_SECOND);
  }
  uint16_t fraction = elapsed;

  // 00:00:00.000 (len = 12)
  // four leading spaces to center
  snprintf(buffer, 21, "    %02d:%02d:%02d.%03d    ", hours, minutes, seconds, fraction);
}

void TimingScreen::update(uint32_t now) {
  char buffer[22];
  elapsedToString(now - _start, buffer);
  _lcd->print(buffer);
}

void TimingScreen::onEnter() {
  go_button_led.off();
  _start = millis();
  char buffer[22];
  elapsedToString(0, buffer);
  _lcd->print(buffer);
}

bool TimingScreen::onMessage(uint16_t cmd, uint8_t len, byte* buffer) {
}

