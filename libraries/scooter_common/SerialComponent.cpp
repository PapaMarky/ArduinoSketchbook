#include <SoftwareSerial.h>
#include "SerialComponent.h"
#include "debug.h"

void SerialComponent::loop(uint32_t now) {

  if (check_for_message()) {
    gdbg->DEBUG("**got message");
    _is_connected = true;
    handleMessage(_msg_command, _msg_length, (byte*)_buffer);
    done_with_message();
  }
}

void SerialComponent::send_message(uint16_t cmd) {
  send_message(cmd, 0, 0);
}

void SerialComponent::send_message(uint16_t cmd, char* data, byte len) {
  char b[64];
  char dbg[32];
  snprintf(dbg,64,"send %02x", cmd);
  gdbg->DEBUG(dbg);
  int_to_str(cmd, &b[0]);
  int_to_str(len, &b[2]);
  int i = 0;
  for (i = 0; i < len && i + 4 < 64; i++)
    b[i+4] = data[i];

  const char* terminator = SCOOTER_MESSAGE_TERMINATOR;
  for (i = 0; i < SCOOTER_MESSAGE_TERMINATOR_LENGTH; i++)
    b[i+4+len] = terminator[i];
  
  b[len+4+SCOOTER_MESSAGE_TERMINATOR_LENGTH] = '\0';
  snprintf(dbg,32,"cmd: 0x%02x (%d) = '%s'",cmd, cmd, (char*)b);
  gdbg->DEBUG(dbg);
  int sent = _serial->print((char*)b);
  //  int sent = _serial->write(b, len + 4 + SCOOTER_MESSAGE_TERMINATOR_LENGTH+1);
  snprintf(dbg, 32, "sent: %d bytes", sent);
  gdbg->DEBUG(dbg);
}

void SerialComponent::done_with_message() {
  _message_length = 0;
  _position = 0;
  _msg_command = msg_none;
}

bool SerialComponent::check_for_message() {
  char dbg[32];
  snprintf(dbg,32,"chk_4_msg() - %s", have_message() ? "HAVE" : "NONE");
  gdbg->DEBUG(dbg);
  if (!have_message()) {
    int count = _serial->available();
    while(count > 0) {
      char bbb[20];
      snprintf(bbb, 20, "Serial Has %d", count);
      int i = _serial->read();
      if (i != -1) {
	char c = (char)i;
	_buffer[_position] = c;
	_position++;
	if (_position == 2) {
	  if (! str_to_int(&_buffer[0], _msg_command)) {
	    _position = 0;
	    return false;
	  }
	}
	else if (_position == 4) {
	  if (! str_to_int(&_buffer[0] + 2, _msg_length)) {
	    _position = 0;
	    return false;
	  }
	}
	if (check_for_terminator()) {
	  _position -= SCOOTER_MESSAGE_TERMINATOR_LENGTH;
	  if (_position - 4 != _msg_length) {
	    _position = 0;
	    return false;
	  }
	  _message_length = _position;
	  _buffer[_position] = '\0';
	}
      }
      else {
	_position = 0;
      }
      count = _serial->available();
    }
  }
  return have_message();
}

bool SerialComponent::to_hex(char c, uint8_t& v) {
  if (c >= '0' && c <= '9')
    v = c - '0';
  else if (c >= 'a' && c <= 'f')
    v = c - 'a' + 10;
  else if (c >= 'A' && c <= 'F')
    v = c - 'A' + 10;
  else
    return false;
  return true;
}

bool SerialComponent::str_to_int(char* data, uint8_t& value) {
  uint8_t d0, d1;
  if (to_hex(data[0], d0) && to_hex(data[1], d1)) {
    value = d0 * 16 + d1;
    return true;
  }
  return false;
}
void SerialComponent::TEST() {
  byte b = 0;
  char buf[4];
  while (b < 0x20) {
    Serial.print("Test: "); Serial.println(b,HEX);
    int_to_str(b, buf);
    buf[2] = 0;
    Serial.print(" -> "); Serial.println(buf);
    uint8_t v;
    str_to_int(buf, v);
    Serial.print(" ---> "); Serial.println(v,HEX);
    if (v == b) 
      Serial.println("SUCCESS");
    else
      Serial.println("FAIL");
    b++;
  }
}

bool SerialComponent::to_char(byte b, char& c) {
  char bb[32];
  snprintf(bb,32,"to_char: 0x%02x (%d)", b, b);
  if (b < 10)
    c = '0' + b;
  else if (b < 16)
    c = 'a' + (b - 10);
  else
    return false;
  snprintf(bb,32," - returning '%c'", (char)c);
  return true;
}

bool SerialComponent::int_to_str(uint16_t value, char* data) {
  byte v = (value & 0xf0) >> 4;
  to_char(v, data[0]);
  to_char(value & 0x0f, data[1]);
  return true;
}

const char* _terminator = SCOOTER_MESSAGE_TERMINATOR;
bool SerialComponent::check_for_terminator() {
  if (_position < SCOOTER_MESSAGE_TERMINATOR_LENGTH)
    return false;
  
  int n = _position - SCOOTER_MESSAGE_TERMINATOR_LENGTH;
  int i = 0;
  while(n < _position) {
    if (_terminator[i] != _buffer[n])
      return false;
    n++; i++;
  }
  return true;
}
