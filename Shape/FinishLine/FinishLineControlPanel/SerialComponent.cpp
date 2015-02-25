#include "Component.h"

#include <SoftwareSerial.h>

void SerialComponent::loop() {
  check_for_message();
}

bool SerialComponent::send_message(char* buffer) {
  _serial->println(buffer);
}

int SerialComponent::check_for_message() {
  if (!have_message() && _serial->available()) {
    _message_length = _serial->readBytesUntil('\n', _buffer, BUFFER_LENGTH);
  }
  return _message_length;
}
void SerialComponent::setup() {
  _serial->begin(_baud);
}
