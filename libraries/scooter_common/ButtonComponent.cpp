// copyright 2015, Mark Dyer
#include "Component.h"

void ButtonComponent::loop(uint32_t now) {
  if (_state_start != 0 && now - _state_start > BUTTON_BOUNCE_DELAY) {
    _state_start = 0;
    if (_closed != (digitalRead(_pin) == HIGH)) {
      _closed = !_closed;
      if (_manager) {
	_manager->onButtonEvent(_id, _closed ? BUTTON_PRESSED : BUTTON_RELEASED);
      }
    }
    return;
  }

  if (_closed != (digitalRead(_pin) == HIGH)) {
    _state_start = now;
    return;
  }
}
