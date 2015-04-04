// Copyright 2015, Mark Dyer
#include <avr/pgmspace.h>

#include "strings.h"

static char sbuffer[32];


const char reset[]       PROGMEM = "       RESET!       "; // 0
const char blank[]       PROGMEM = "                    "; // 1
const char go[]          PROGMEM = "        GO!!        "; // 2
const char false_start[] PROGMEM = "    FALSE START!!   "; // 3
const char scooter_bot[] PROGMEM = "   ScooterBot 1.0   "; // 4
const char connecting[]  PROGMEM = "Connecting...       "; // 5
const char shape[]       PROGMEM = "Shape Security, Inc."; // 6
const char moveup[]      PROGMEM = "Move Up to the Line "; // 7
const char pressgo[]     PROGMEM = "Press Go Button to  "; // 8
const char startcount[]  PROGMEM = "Start Countdown     "; // 9
const char rumble[]      PROGMEM = "Ready to Rumble     "; // 10

const char* const string_table[] PROGMEM =
  {
    reset,        //  0
    blank,        //  1
    go,           //  2
    false_start,  //  3
    scooter_bot,  //  4
    connecting,   //  5
    shape,        //  6
    moveup,       //  7
    pressgo,      //  8
    startcount,   //  9
    rumble        // 10
  };

char* s(int id) {
  strcpy_P(sbuffer, (char*)pgm_read_word(&(string_table[id])));
  return sbuffer;
}
