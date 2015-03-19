// Copyright 2015, Mark Dyer
#include "Component.h"

char* event_to_str(int event) {
  switch(event) {
  case BUTTON_PRESSED:
    return "PRESSED";
  case BUTTON_RELEASED:
    return "RELEASED";
  default:
    return "BAD EVENT";
  }
}
