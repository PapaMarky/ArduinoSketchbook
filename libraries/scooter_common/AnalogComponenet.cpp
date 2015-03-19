// copyright 2015, Mark Dyer

#include "Component.h"

void AnalogOutComponent::loop(uint32_t now) {
  if (_state == VALUE)
    return;

  uint8_t v = _value;
  uint32_t elapsed = now - _fade_start;
  if (elapsed >= _fade_duration) {
    setValue((_state == FADE_UP) ? 255 : 0);
    return;
  }

  if (_state == FADE_UP) {
    v = map(elapsed, 0, _fade_duration, 0, 255);
  }

  if (_state == FADE_DOWN) {
    v = map(elapsed, 0, _fade_duration, 255, 0);
  }
  if (v != _value) {
    _value = v;
    analogWrite(_pin, _value);
  }
}

void AnalogOutComponent::setValue(uint8_t value) {
  if (value != _value) {
    _value = value;
    analogWrite(_pin, value);
  }
  _state = VALUE;
}

void AnalogOutComponent::setFadeUp(uint8_t value, uint32_t duration) {
  _state = FADE_UP;
  _fade_target = value;
  _fade_start = millis();
  _fade_duration = duration;
}

void AnalogOutComponent::setFadeDown(uint8_t value, uint32_t duration) {
  _state = FADE_DOWN;
  _fade_target = value;
  _fade_start = millis();
  _fade_duration = duration;
}
