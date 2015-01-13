/**
 * NeoDigits - 7 Segment Display build from NeoPixel LEDs
 */
#include "Adafruit_NeoPixel.h"
#include "NeoDigits.h"

const int gSegLengths[8] = {
  SEG_A_LEN, SEG_B_LEN, SEG_C_LEN, SEG_D_LEN, SEG_E_LEN, SEG_F_LEN, SEG_G_LEN, SEG_DP_LEN
};

const int gSegOffsets[8] = {
  SEG_A_OFF, SEG_B_OFF, SEG_C_OFF, SEG_D_OFF, SEG_E_OFF, SEG_F_OFF, SEG_G_OFF, SEG_DP_OFF
};

struct DigitDef {
  char letter;
  byte map;
};

const DigitDef gDigitDefs[] = {
  {'.', DIG_DP},
  {' ', DIG_SPACE},
  {'0', DIG_0},
  {'1', DIG_1},
  {'2', DIG_2},
  {'3', DIG_3},
  {'4', DIG_4},
  {'5', DIG_5},
  {'6', DIG_6},
  {'7', DIG_7},
  {'8', DIG_8},
  {'9', DIG_9},
  {'A', DIG_A},
  {'b', DIG_b},
  {'C', DIG_C},
  {'c', DIG_c},
  {'d', DIG_d},
  {'E', DIG_E},
  {'F', DIG_F},
  {'H', DIG_H},
  {'J', DIG_J},
  {'L', DIG_L},
  {'n', DIG_n},
  {'o', DIG_o},
  {'O', DIG_O},
  {'P', DIG_P},
  {'r', DIG_r},
  {'S', DIG_S},
  {'t', DIG_t},
  {'U', DIG_U},
  {'u', DIG_u},
  {'y', DIG_y},
  {'\0', 0}
};

NeoDigitDisplay::NeoDigitDisplay(uint16_t nDigits, uint16_t nColons, uint8_t pinNumber)
  :
  _pixels(nDigits * ((uint16_t)DIGIT_SIZE) + nColons * COLON_SIZE, pinNumber, (uint8_t)(NEO_GRB + NEO_KHZ800)),
  _mode(SOLID),
  _nDigits(nDigits), _nColons(nColons), _dirty(false)
{
}

void NeoDigitDisplay::setup() {
  _pixels.begin();
  _pixels.show();
  Serial.println("");
  Serial.print("Digit Setup: nDigits: "); Serial.print(_nDigits); Serial.print(", pixels: ");
  Serial.println(_nDigits * DIGIT_SIZE + _nColons * COLON_SIZE);
  Serial.print("DIGIT_SIZE: "); Serial.println(DIGIT_SIZE);
  Serial.print("nColons: "); Serial.print(_nColons); Serial.print(", COLON_SIZE: "); Serial.println(COLON_SIZE);
  Serial.print("numPixels: "); Serial.println(_pixels.numPixels());
}

void NeoDigitDisplay::loop() {
  if (_dirty) {
    for(int i = 0; i < _nDigits; i++) {
      lightDigit(i);
    }
    Serial.println("SHOW");
    _pixels.show();
    _dirty = false;
  }
}

void NeoDigitDisplay::SetDigit(int digit, char value) {
  bool valid = false;

  int i = 0;
  Serial.print("look for '");Serial.print(value);Serial.println("'");
  for(i = 0; gDigitDefs[i].letter != '\0'; i++) {
    if (gDigitDefs[i].letter == value) {
      Serial.println("  FOUND IT");
      valid = true;
      break;
    }
  }

  if (valid) {
    // preserve status of DP segment
    bool dp_on = bitRead(_current[digit], 7);
    _current[digit] = gDigitDefs[i].map;
    if (dp_on)
      bitSet(_current[digit], 7);
    else
      bitClear(_current[digit], 7);

    _dirty = true;
  }
}

void NeoDigitDisplay::lightDigit(uint8_t digit) {
  if (digit < 0 || digit >= _nDigits) {
    return;
  }
  byte def = _current[digit];

  // light each segment
  for (int i = 0; i < 8; i++) {
    bool on = bitRead(def, i);
    lightSegment(digit, i, on);
  }
}

void NeoDigitDisplay::lightSegment(int digit, int segment, bool on) {
  int ds = DIGIT_SIZE;
  int offset = (digit * ds) + gSegOffsets[segment];

  Serial.print("lightSegment("); Serial.print(digit);
  Serial.print(", "); Serial.print(segment);
  Serial.print(", "); Serial.print(on);
  Serial.println(")");
  int len = gSegLengths[segment];

  Serial.print("Seg len: ");Serial.println(len);
  Serial.print("digit: "); Serial.println(digit);
  Serial.print("DIGIT_SIZE: ");Serial.println(ds);
  Serial.print("digit * DIGIT_SIZE: "); Serial.println(ds * digit);
  Serial.print("gSegOffsets[");Serial.print(segment);Serial.print("]: ");Serial.println(gSegOffsets[segment]);
  Serial.print("offset: ");Serial.println(offset);
  uint32_t color = 0;
  if (on) {
    color = _pixels.Color(255, 0, 0);
  }
  
  for (int i = 0; i < len; i++) {
    //Serial.print("offset: ");Serial.println(offset + i);
    //    Serial.print("color: 0x");Serial.println(color,HEX);
    _pixels.setPixelColor(offset + i, color);
  }
}

