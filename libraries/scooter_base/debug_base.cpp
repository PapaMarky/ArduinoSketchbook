#include <arduino.h>
#include "debug_base.h"

void debug_base::DEBUG(const char* msg) {
  char BB[100];
  snprintf(BB,100,"DBG|%4d|%s",freeRam(),msg);
  Serial.println(BB);
}

