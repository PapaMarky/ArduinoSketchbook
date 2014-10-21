/**
 * NeoDigits - 7 Segment Display build from NeoPixel LEDs
 */
#include "Adafruit_NeoPixel.h"
#include "NeoDigits.h"

const int gSegLengths[8] = {
  SEG_A_LEN, SEG_B_LEN, SEG_C_LEN, SEG_D_LEN, SEG_E_LEN, SEG_F_LEN, SEG_G_LEN, SEG_DP_LEN
};


NeoDigitDisplay::NeoDigitDisplay(uint16_t nDigits, uint16_t nColons, uint8_t pinNumber)
  : _nDigits(nDigits), _nColons(nColons),
    _pixels(nDigits * DIGIT_SIZE + nColons * COLON_SIZE, pinNumber, (uint8_t)(NEO_GRB + NEO_KHZ800)){
}

void NeoDigitDisplay::setup() {
}

void NeoDigitDisplay::loop() {
}
