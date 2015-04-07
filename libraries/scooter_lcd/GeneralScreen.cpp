// Copyright 2015, Mark Dyer
#include <LiquidCrystal.h>
#include "strings.h"
#include "screen.h"

static byte original_shape_logo_data[8][8] = {
  { // 0
    B00001111,B00011111,B00011000,B00011111,
    B00001111,B00000000,B00011111,B00011111},
  { // 1
    B00011011,B00011011,B00000011,B00011011,
    B00011111,B00001111,B00011111,B00011011},
  { // 2
    B00000001,B00000001,B00000001,B00011111,
    B00011111,B00000001,B00000001,B00000001},
  { // 3
    B01010000,B01010000,B00011111,B00011111,
    B01010000,B00010011,B00010001,B00011000},
  { // 4
    B00000011,B00000001,B00011001,B00011001,
    B00000001,B00011001,B00011001,B00000001},
  { // 5
    B00011111, B00011111, B01010000, B01010000,
    B00011111, B00011111, B01010000, B01010000},
  { // 6
    B00010011, B00011111, B00011110, B00011111,
    B00011111, B00010110, B00000111, B00000011},
  { // 7
    B00011110, B00011111, B00000011, B00011111,
    B00011110, B00000000, B00011111, B00011111},
};

static byte shape_logo_data[8][8] = {
  { // 0
    B00001111,B00011111,B00011000,B00011111,
    B00001111,B00000000,B00011111,B00011111},
  { // 1
    B00011011,B00011011,B00000011,B00011011,
    B00011111,B00001111,B00011111,B00011011},
  { // 2
    B00000001,B00000001,B00000001,B00011111,
    B00011111,B00000001,B00000001,B00000001},
  { // 3
    B00010000,B00000000,B00011111,B00011111,
    B00010000,B00000011,B00010001,B00011000},
  { // 4
    B00000011,B00000001,B00011001,B00011001,
    B00000001,B00001001,B00011001,B00000001},
  { // 5
    B00011111, B00011111, B00010000, B00010000,
    B00011111, B00011111, B00010000, B00010000},
  { // 6
    B00010011, B00011111, B00011110, B00011111,
    B00011111, B00010110, B00000111, B00000011},
  { // 7
    B00011110, B00011111, B00000011, B00011111,
    B00011110, B00000000, B00011111, B00011111},
};

void GeneralScreen::setup() {
  //  load_shape_logo();
}

void GeneralScreen::load_shape_logo(LiquidCrystal* lcd) {
  for (byte i = 0; i < 8; i++) {
    lcd->createChar(i,shape_logo_data[i]);
    delay(10);
  }
}

void GeneralScreen::display_shape_logo() {
  _lcd->setCursor(0, 0);
  _lcd->print(s(S_SHAPE));
  /*
  _lcd->print("      ");
  for (byte i = 0; i < 8; i++)
    _lcd->write(i);
  _lcd->print("      ");
  */
}

void GeneralScreen::onEnter() {
  uint32_t now = millis();
  _logo_rotate_start = now;
  _showing_logo = true;
  display_shape_logo();
}

#define LOGO_ROTATE_TIME 5000

void GeneralScreen::update(uint32_t now) {
  if (now - _logo_rotate_start > LOGO_ROTATE_TIME) {
    _logo_rotate_start = now;
    _showing_logo = !_showing_logo;
    if (_showing_logo) {
      //display_shape_logo();
      _lcd->setCursor(0, 0);
      _lcd->print(s(S_SCOOTERBOT));
    } else {
      _lcd->setCursor(0, 0);
      _lcd->print(s(S_SCOOTERBOT));
    }
  }
}
