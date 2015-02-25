// Copyright 2015, Mark Dyer

#include "mdComm.h"
#include <Stream.h>

namespace mdComm {

  const int _buffer_size = 64;
  static char _buffer[_buffer_size];
  char inBuffer[512];
  bool have_message = false;
  bool have_header = false;
  bool have_length = false;
  bool have_data = false;
  bool have_trailer = false;
  int start_bytes = 0;
  int end_bytes = 0;
  int data_length = 0;
  int data_count = 0;

  bool DBG = false;
  char dbg_buf[_buffer_size];

  char* get_raw_buffer() { return _buffer; }
  void startDebug() { DBG = true; }

  bool resetMessagePump() {
    have_message = false;
    have_header = false;
    have_length = false;
    have_data = false;
    have_trailer = false;
    start_bytes = 0;
    end_bytes = 0;
    data_length = 0;
    data_count = 0;
    if (DBG) 
      Serial.println("resetMessagePump");
  }

  bool isPrintable(char c) { return (c >= ' ' && c <= '~'); }

  bool findHeader(Stream& serial) {
    if (have_header)
      return true;

    int b = 0;
    if (serial.available()) {
      if (start_bytes == 0) {
	b = serial.peek();
	while(b != START_BYTE) {
	  int toss = serial.read();
	  if (DBG) {
	    if (isPrintable(toss))
	      sprintf(dbg_buf, "Looking for header, skipping 0x%02x '%c'", toss, toss);
	    else
	      sprintf(dbg_buf, "Looking for header, skipping 0x%02x", toss);
	    Serial.println(dbg_buf);
	  }
	  if (! serial.available())
	    return false;
	  b = serial.peek();
	}
      }  // if start_bytes

      // We should be looking at a start_byte
      b = serial.read();
      while(b == START_BYTE) {
	start_bytes++;
	if (start_bytes >= START_COUNT) {
	  have_header = true;
	  if (DBG) {
	    Serial.println("Got Header");
	  }
	  return true;
	}
	if (! serial.available())
	  return false;
	b = serial.read();
      }
      resetMessagePump();
    }
    return false;
  }

  bool getMessageLength(Stream& serial) {
    if (serial.available() >= 2) {
      int b0 = serial.read();
      int b1 = serial.read();
      data_length = (b1 & 0x00ff) | ((b0 << 8) & 0xff00);
      if (DBG) {
	sprintf(dbg_buf, "Got length: %d b0(0x%02x) b1(0x%02x) len(0x%02x)", data_length, b0, b1, data_length);
	Serial.println(dbg_buf);
      }
      have_length = true;
      return true;
    }
    return false;
  }

  bool getMessageData(Stream& serial, char* buffer, int buffer_size) {
    if (have_data)
      return true;
    while ((data_count < data_length) && serial.available()) {
      int b = serial.read();

      if (DBG) {
	if (isPrintable(b))
	  sprintf(dbg_buf, " - data[%d of %d]: 0x%02x '%c'", data_count, data_length, b, b);
	else 
	  sprintf(dbg_buf, " - data[%d of %d]: 0x%02x", data_count, data_length, b);

	Serial.println(dbg_buf);
      }

      int i = data_count;
      if (data_count < buffer_size) {
	buffer[i] = (char)b;
      }

      data_count++;

    }

    if (data_count >= data_length) {

      if (DBG)
	Serial.println("Got all data");

      have_data = true;
    }
    have_data = true;
    return have_data;
  }

  bool getTrailer(Stream& serial) {
    int n = 0;
    if (have_trailer)
      return true;

    if (serial.available() >= START_COUNT) {
      int b = serial.read();
      /*
      if (DBG) {
	Serial.println("Looking for Trailer");
      }
      */
      while (n < START_COUNT) {
	if (b != END_BYTE) {
	  if (DBG) {
	    sprintf(dbg_buf, "- bad trailer byte: 0x%02x (expected 0x%02x)", b, END_BYTE);
	    Serial.println(dbg_buf);
	  }
	  resetMessagePump();
	  return false;
	}
	/*
	if (DBG) {
	  sprintf(dbg_buf, "Got Trailer Byte 0x%02x (%d of %d)", END_BYTE, n, START_COUNT);
	  Serial.println(dbg_buf);
	}
	*/
	b = serial.read();
	n++;
      }
      have_trailer = true;
    }
    return have_trailer;
  }
  /*
   * Reentrant message reader.
   * Will return false until it has a whole message.
   */
  bool checkForMessage(Stream& serial, char* buffer, int buffer_size) {

    if (DBG)
      Serial.println("Check for Message");

    if (have_message)
      return true;

    if (!have_header)
      findHeader(serial);

    if (have_header)
      getMessageLength(serial);

    if (have_length)
      getMessageData(serial, buffer, buffer_size);

    if (have_data)
      getTrailer(serial);

    return have_trailer;
  }

  bool gotMessage() {
    resetMessagePump();
    return true;
  }
  // TODO error checking
  // XXX - Build the message in a buffer, and then send it all at once.
  bool writeMessage(char* data, uint16_t len, Stream& serial) {

    int total_len = START_COUNT + START_COUNT + 2 + len;

    if (total_len > _buffer_size)
      return false;

    int pos = 0;
    // write the start bytes
    for (int i = 0; i < START_COUNT; i++) {
      _buffer[pos++] = START_BYTE;
    }
    // write the length
    byte b0 = (len & 0xff00) >> 8;
    byte b1 = (len & 0x00ff);
    _buffer[pos++] = b0;
    _buffer[pos++] = b1;
    // write the data
    for (int i = 0; i < len; i++) {
      _buffer[pos++] = data[i];

      if (DBG) {
	if (isPrintable(data[i]))
	  sprintf(dbg_buf, "0x%02x '%c'", data[i], data[i]);
	else
	  sprintf(dbg_buf, "0x%02x", data[i]);
      
	Serial.println(dbg_buf);
      }
    }
    // write the end bytes
    for (int i = 0; i < START_COUNT; i++) {
      _buffer[pos++] = END_BYTE;
    }
    serial.write((uint8_t*)_buffer, total_len);
    return true;
  }

  comm_result write_short(uint16_t data, Stream& serial) {
    uint8_t b = data & 0x00ff;
    if (serial.write(b) != 1)
      return IO_ERROR;
    b = (data & 0xff00) >> 8;
    if (serial.write(b) != 1)
      return IO_ERROR;

    return SUCCESS;
  }

  comm_result read_short(uint16_t& data, Stream& serial) {
    if (serial.available() < 2)
      return NO_DATA;

    int b = serial.read();
    if (b == -1)
      return IO_ERROR;
    
    uint8_t b0 = b;

    b = serial.read();
    if (b == -1)
      return IO_ERROR;
    uint8_t b1 = b;

    data = (b1 << 8) & b0;

    return SUCCESS;
  }

  comm_result write_message(message& msg) {
    return write_message(msg, Serial);
  }

  comm_result write_message(message& msg, Stream& serial) {
    if (msg.header.msg_len > 0 && msg.data == 0)
      return BUFFER_TOO_SMALL;

    if (msg.data_size < msg.header.msg_len)
      return BUFFER_TOO_SMALL;

    for (int i = 0; i < START_COUNT; i++) {
      if (serial.write(START_BYTE) < 1)
	return IO_ERROR;
    }

    if (serial.write(msg.header.src) < 1)
      return IO_ERROR_SRC;

    if (serial.write(msg.header.dst) < 1)
      return IO_ERROR_DST;

    comm_result r = SUCCESS;
    if ((r = write_short(msg.header.msg_id, serial)) != SUCCESS)
      return IO_ERROR_ID;

    if ((r = write_short(msg.header.msg_len, serial)) != SUCCESS)
      return IO_ERROR_LEN;

    if (msg.header.msg_len > 0)
      if (serial.write(msg.data, msg.header.msg_len) < msg.header.msg_len)
	return IO_ERROR_DATA;

    for (int i = 0; i < START_COUNT; i++) {
      if (serial.write(END_BYTE) < 1)
	return IO_ERROR;
    }

    return SUCCESS;
  }

  comm_result read_header(message& msg, Stream& serial) {
    if (serial.available() < sizeof(msg.header))
      return NO_DATA;

    int b = serial.read();
    if (b == -1)
      return IO_ERROR;

    msg.header.src = b;

    b = serial.read();
    if (b == -1)
      return IO_ERROR;

    msg.header.dst = b;

    comm_result r = SUCCESS;

    if ((r = read_short(msg.header.msg_id, serial)) != SUCCESS)
      return r;

    if ((r = read_short(msg.header.msg_len, serial)) != SUCCESS)
      return r;

    return SUCCESS;
  }

  comm_result read_message(message& msg) {
    return read_message(msg, Serial);
  }

  comm_result read_message(message& msg, Stream& serial) {
    comm_result r = SUCCESS;

    if ((r = read_header(msg, serial)) != SUCCESS)
      return r;

    if (msg.data == 0 && msg.header.msg_len > 0)
      return BUFFER_TOO_SMALL;

    if (msg.data_size < msg.header.msg_len)
      return BUFFER_TOO_SMALL;

    if (msg.header.msg_len > 0 &&
	(serial.readBytes(reinterpret_cast<char*>(msg.data), msg.header.msg_len) < msg.header.msg_len))
      return IO_ERROR;
	
    
    return SUCCESS;
  }
}
