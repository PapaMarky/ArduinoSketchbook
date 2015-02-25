// copyright 2015, Mark Dyer

#include "Component.h"
#include <string.h>

void LcdComponent::loop() {
  if (check_for_message()) {
    if (strncmp(_buffer,"lcd:ack start", 13) == 0) {
      _is_ready = true;
      flush_message();
    }
  }
}

