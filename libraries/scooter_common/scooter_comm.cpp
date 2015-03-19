#include "scooter_comm.h"

bool have_message = false;
char scooter_buffer[SCOOTER_BUFFER_LEN];
int pos = 0;
const char* terminator = SCOOTER_MESSAGE_TERMINATOR;

namespace scooter {

  uint8_t COMMAND = 0;
  uint8_t LENGTH = 0;

  void done_with_message() {
    have_message = false;
    pos = 0;
  }

  bool to_hex(char c, uint8_t& v) {
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
  bool str_to_int(char* data, uint8_t& value) {
    uint8_t d0, d1;
    if (to_hex(data[0], d0) && to_hex(data[1], d1)) {
      value = d0 * 16 + d1;
      return true;
    }
    return false;
  }

  int message_length() {
    if (!have_message)
      return 0;
    return pos;
  }

  bool check_for_terminator() {
    if (pos < SCOOTER_MESSAGE_TERMINATOR_LEN)
      return false;

    int n = pos - SCOOTER_MESSAGE_TERMINATOR_LEN;
    int i = 0;
    while(n < pos) {
      if (terminator[i] != scooter_buffer[n])
	return false;
      n++; i++;
    }
    return true;
  }

  bool check_for_messages(Stream* s) {
    if (!have_message) {
      while(s->available() > 0) {
	int i = s->read();
	if (i != -1) {
	  char c = (char)i;
	  scooter_buffer[pos] = c;
	  pos++;
	  if (pos == 2) {
	    if (! str_to_int(scooter_buffer, COMMAND)) {
	      pos = 0;
	      return false;
	    }
	  }
	  else if (pos == 4) {
	    if (! str_to_int(scooter_buffer + 2, LENGTH)) {
	      pos = 0;
	      return false;
	    }
	  }
	  if (check_for_terminator()) {
	    pos -= SCOOTER_MESSAGE_TERMINATOR_LEN;
	    if (pos - 4 != LENGTH) {
	      pos = 0;
	      return false;
	    }
	    have_message = true;
	    scooter_buffer[pos] = '\0';
	  }
	}
	else {
	  pos = 0;
	}
      }
    }
    return have_message;
  }

  bool consume_message(uint8_t& cmd, uint8_t& len, byte* buffer, int bufferlen) {
    len = 0;
    if(!have_message)
      return false;
    cmd = COMMAND;
    len = LENGTH;
    if (bufferlen < len)
      return false;
    for(int i = 0; i < len; i++)
      buffer[0] = scooter_buffer[4+i];
    
  }

}
