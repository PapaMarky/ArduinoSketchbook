#ifndef DEBUG_LCD_H
#define DEBUG_LCD_H

#include <debug.h>

class debug_lcd : public debug {
  void DEBUG(const char* msg);
};

#endif // DEBUG_LCD_H

