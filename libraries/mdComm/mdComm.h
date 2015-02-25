// Copyright 2015, Mark Dyer
#ifndef COMM_H
#define COME_H

#include <arduino.h>

class Stream;
namespace mdComm {
  /*
   * Start and end are each sent four times
   */
  const uint8_t START_BYTE = '['; // 0xfa;
  const uint8_t END_BYTE = ']';  // 0xab;
  const uint8_t START_COUNT = 2;

  void startDebug();
  bool writeMessage(char* data, uint16_t len, Stream& serial);
  bool checkForMessage(Stream& serial, char* buffer, int buffer_size);
  bool gotMessage();
  bool readMessage(byte* data, uint16_t& data_len, uint16_t buffer_size);

  enum comm_src {
    BASE = 0x01,
    LCD = 0x02,
    TREE = 0x03,
    DISP = 0x04
  };

  enum comm_result {
    SUCCESS = 0x00,
    IO_ERROR = 0x01,
    BUFFER_TOO_SMALL = 0x02,
    NO_DATA = 0x03,
    IO_ERROR_SRC = 0x04,
    IO_ERROR_DST = 0x05,
    IO_ERROR_ID = 0x06,
    IO_ERROR_LEN = 0x07,
    IO_ERROR_DATA = 0x08
  };

  /*
   * general communication messages
   * 0x01nn
   */
  const unsigned short MSG_HELLO = 0x0101;
  const unsigned short MSG_OLLEH = 0x0102;

  struct msg_header {
    uint8_t src;
    uint8_t dst;
    uint16_t msg_id;
    uint16_t msg_len;
  };

  struct message {
    struct msg_header header;
    uint8_t* data;
    int data_size;
  };
  

  char* get_raw_buffer();

  comm_result write_message(message& msg);
  comm_result write_message(message& msg, Stream& ser);
  
  comm_result read_message(message& msg);
  comm_result read_message(message& msg, Stream& ser);
}
#endif // COMM_H
