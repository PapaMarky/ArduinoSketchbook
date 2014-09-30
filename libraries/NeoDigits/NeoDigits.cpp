/**
 * NeoDigits - 7 Segment Display build from NeoPixel LEDs
 */
#include "Adafruit_NeoPixel.h"
#include "NeoDigits.h"

NeoDigits::NeoDigits(int nDigits, int nColons, int pinNumber) : _nDigits(nDigits), _nColons(nColons) {
  _pixels = Adafruit_NeoPixel(N_DIGITS * DIGIT_SIZE + N_COLONS * COLON_SIZE, pinNumber, NEO_GRB + NEO_KHZ800);
}

void NeoDigits::setup() {
}

void NeoDigits::loop() {
}
