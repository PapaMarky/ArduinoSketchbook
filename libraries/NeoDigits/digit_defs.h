// Digit definitions
// Each digit is defined as an 8 bit bitmap.
// Bit zero is segment A, bit 7 is the DP
// DIG_DP can be or'ed into a digit to turn it on

#define DIG_DP    (byte)B10000000
#define DIG_SPACE (byte)B00000000
#define DIG_0     (byte)B00111111
#define DIG_1     (byte)B00000110
#define DIG_2     (byte)B01011011
#define DIG_3     (byte)B01001111
#define DIG_4     (byte)B01100110
#define DIG_5     (byte)B01101101
#define DIG_6     (byte)B01111101
#define DIG_7     (byte)B00000111
#define DIG_8     (byte)B01111111
#define DIG_9     (byte)B01100111
#define DIG_A     (byte)B01110111
#define DIG_b     (byte)B01111100
#define DIG_C     (byte)B00111001
#define DIG_c     (byte)B01011000
#define DIG_d     (byte)B01011110
#define DIG_E     (byte)B01111001
#define DIG_F     (byte)B01110001
// g
#define DIG_H     (byte)B01110110
// i (?)
#define DIG_J     (byte)B00001110
// k
#define DIG_L     (byte)B00111000
// m
#define DIG_n     (byte)B01010100
#define DIG_o     (byte)B01011100
#define DIG_O     DIG_0
#define DIG_P     (byte)B01110011
// q
#define DIG_r     (byte)B01010000
#define DIG_S     DIG_5
#define DIG_t     (byte)B01111000
#define DIG_U     (byte)B00111110
#define DIG_u     (byte)B00011100
// v
// w
// x
#define DIG_y     (byte)B01101110
// z
