/**
 * NeoDigits - 7 Segment Display built from NeoPixel LEDs
 */

#define N_DIGITS 6
#define N_COLONS 2
// Define offset of each segment from the begining of the digit
// In my case horizontal segments (A, D, G) are 3 pixels,
// vertical segments (B, C, E, F) are 2 and the decimal point
// is one.
#define SEG_A_OFF 0
#define SEG_B_OFF SEG_A_OFF + 3
#define SEG_C_OFF SEG_B_OFF + 2
#define SEG_D_OFF SEG_C_OFF + 2
#define SEG_E_OFF SEG_D_OFF + 3
#define SEG_F_OFF SEG_E_OFF + 2
#define SEG_G_OFF SEG_F_OFF + 2
#define SEG_DP_OFF SEG_G_OFF + 3

// Number of pixels in a digit (including DP)
#define SEG_SIZE SEG_DP_OFF + 1

#include "digit_defs.h"

class NeoDigitDisplay {
 public:
 NeoDigits(int nDigits, int nColons) : _nDigits(nDigits), _nColons(nColons) {}
  setup();
  loop();

  void SetColor(color c);
  void SetGradiant(color top_c, color bottom_c);

 private:
  void light_digit(int digit);
  enum ColorScheme { SOLID, GRADIANT };
  enum TransitionType { INSTANT, FADE }
  int _nDigits;
  int _nColons;
  char _current[N_DIGITS];
  char _next[N_DIGITS];
};

