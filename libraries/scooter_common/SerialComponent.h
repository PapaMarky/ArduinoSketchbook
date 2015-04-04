// copyright 2015, Mark Dyer
#ifndef SERIAL_COMPONENT_H
#define SERIAL_COMPONENT_H

#include <Arduino.h>

#include <Component.h>

#define SCOOTER_BUFFER_LENGTH 64

#define SCOOTER_VERB_MAX_LENGTH 10
#define SCOOTER_DETAIL_MAX_LENGTH 20

#define SCOOTER_MESSAGE_TERMINATOR "\n"
#define SCOOTER_MESSAGE_TERMINATOR_LENGTH 1

class Stream;

// message format:
// two hex digits: command
// two hex digits: length of message
// n bytes: message
// newline

class SerialComponent : public Component {
 public:
  enum Message {
    msg_none        =  0,
    msg_log         =  2,
    msg_laser_seen  =  3,
    msg_laser_ready =  4,
    msg_base_hello  =  5,
    msg_lid_hello   =  6,
    msg_lid_ready   =  7,
    msg_base_ready  =  8,
    msg_laser_off   =  9,
    msg_laser_on    = 10,
    msg_count_5     = 11,
    msg_count_4     = 12,
    msg_count_3     = 13,
    msg_count_2     = 14,
    msg_count_1     = 15,
    msg_go          = 16,
    msg_jump_start  = 17,
    msg_foul_done   = 18,
  };

  void TEST();
  virtual char* name() {return "serial";}

  virtual void handleMessage(uint16_t cmd, uint8_t len, byte* buffer) = 0;

  void initialize(Stream* serial) {
    _serial = serial;
    _message_length = 0;
    _msg_command = msg_none;
    _msg_length = 0;
    _position = 0;
    _is_connected = false;
    _is_master = false;
  }
  virtual void loop(uint32_t now);
  virtual void setup() {};

  bool isConnected() { return _is_connected; }

  bool have_message() { 
    return _message_length > 0; 
  }
  int message_length() { return _message_length; }
  void send_message(uint16_t cmd);
  void send_message(uint16_t cmd, char* data, byte len);

  void become_master() { _is_master = true; }
 protected:
    bool check_for_message();
    char _buffer[SCOOTER_BUFFER_LENGTH];
  private:
    bool to_hex(char c, uint8_t& v);
    bool str_to_int(char* data, uint8_t& value);

    bool to_char(byte b, char& c);
    bool int_to_str(uint16_t value, char* data);

    bool check_for_terminator();
    void done_with_message();
    int _message_length;
    int _position;
    uint8_t _msg_command;
    uint8_t _msg_length;
    Stream* _serial;

    bool _is_connected;
    bool _is_master;
};
#endif // SERIAL_COMPONENT_H
