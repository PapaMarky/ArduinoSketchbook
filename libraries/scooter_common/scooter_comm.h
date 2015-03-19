#ifndef SCOOTER_COMM_H
#define SCOOTER_COMM_H

#define SCOOTER_BUFFER_LEN 64

#define SCOOTER_VERB_MAX_LEN 10
#define SCOOTER_DETAIL_MAX_LEN 20

#define SCOOTER_MESSAGE_TERMINATOR "\n"
#define SCOOTER_MESSAGE_TERMINATOR_LEN 1

// message format:
// two hex digits: command
// two hex digits: length of message
// n bytes: message
// newline
#include <arduino.h>
class Stream;

namespace scooter {
  enum Message {
    heartbeat = 0,
    log = 1
  };
  bool consume_message(uint8_t& cmd, uint8_t& len, byte* buffer, int bufferlen);

  bool check_for_messages(Stream* s);
}

#endif // SCOOTER_COMM_H
