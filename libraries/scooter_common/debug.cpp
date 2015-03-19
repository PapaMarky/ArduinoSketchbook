
#include "debug.h"
debug* gdbg = 0;

int debug::freeRam () 
{
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

debug::debug(){}
debug::~debug(){}
