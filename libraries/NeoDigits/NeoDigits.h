/**
 * NeoDigits - 7 Segment Display built from NeoPixel LEDs
 */
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define N_DIGITS 1
#define N_COLONS 0
// Define offset of each segment from the begining of the digit
// In my case horizontal segments (A, D, G) are 3 pixels,
// vertical segments (B, C, E, F) are 2 and the decimal point
// is one.
#define SEG_A_OFF 0
#define SEG_A_LEN 4
#define SEG_B_OFF SEG_A_OFF + SEG_A_LEN
#define SEG_B_LEN 3
#define SEG_C_OFF SEG_B_OFF + SEG_B_LEN
#define SEG_C_LEN 3
#define SEG_D_OFF SEG_C_OFF + SEG_C_LEN
#define SEG_D_LEN 4
#define SEG_E_OFF SEG_D_OFF + SEG_D_LEN
#define SEG_E_LEN 3
#define SEG_F_OFF SEG_E_OFF + SEG_E_LEN
#define SEG_F_LEN 3
#define SEG_G_OFF SEG_F_OFF + SEG_F_LEN
#define SEG_G_LEN 4
#define SEG_DP_OFF SEG_G_OFF + 4
#define SEG_DP_LEN 1
// Number of pixels in a digit (including DP)
#define DIGIT_SIZE SEG_DP_OFF + 1
#define COLON_SIZE 2

#include "digit_defs.h"

class NeoDigitDisplay {
 public:
  NeoDigitDisplay(uint16_t nDigits, uint16_t nColons, uint8_t pinNumber);
  void setup();
  void loop();

  void SetColor(uint32_t c);
  void SetGradiant(uint32_t top_c, uint32_t bottom_c);

  void SetString(String str);
  void SetValue(char value[4]);
  void SetDigit(int digit, char value);

  void SetColon(int colon, bool on);
  void SetDecimal(int digit, bool on);

  void ClearAll();

  uint32_t color(uint8_t r, uint8_t g, uint8_t b) {return _pixels.Color(r, g, b);}
 private:
  
  Adafruit_NeoPixel _pixels;

  enum ColorScheme { SOLID, GRADIANT };
  enum TransitionType { INSTANT, FADE };
  uint16_t _nDigits;
  uint16_t _nColons;
  uint8_t _pin;
  char _current[N_DIGITS];
  char _next[N_DIGITS];
};

