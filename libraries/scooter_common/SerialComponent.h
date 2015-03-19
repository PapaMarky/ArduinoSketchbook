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
    msg_none = 0,
    msg_heartbeat = 1,
    msg_log = 2,
    msg_laser_state = 3
  };
  enum HeartBeatStatus {
    hb_good = 0,
    hb_late = 1,
    hb_missing = 2
  };
  virtual char* name() {return "serial";}

  virtual void handleMessage(uint8_t cmd, uint8_t len, byte* buffer) = 0;

  void initialize(Stream* serial, uint32_t heartrate) {
    _serial = serial;
    _message_length = 0;
    _msg_command = msg_none;
    _msg_length = 0;
    _position = 0;
    _is_connected = false;
    _last_heartbeat_sent = -10000;
    _last_heartbeat_received = -10000;
    _hb_status = hb_missing;
    _is_master = false;
  }
  virtual void loop(uint32_t now);
  virtual void setup() {};

  bool isConnected() { return _is_connected; }
  HeartBeatStatus heartbeat_status() { return _hb_status; }

  bool have_message() { 
    return _message_length > 0; 
  }
  int message_length() { return _message_length; }
  void send_message(byte cmd);
  void send_message(byte cmd, byte* data, byte len);

  virtual void onHeartbeat() {}

  void become_master() { _is_master = true; }
 protected:
    bool check_for_message();
    char _buffer[SCOOTER_BUFFER_LENGTH];
  private:
    bool to_hex(char c, uint8_t& v);
    bool str_to_int(char* data, uint8_t& value);

    bool to_char(byte b, byte& c);
    bool int_to_str(uint8_t value, byte* data);

    bool check_for_terminator();
    void done_with_message();
    int _message_length;
    int _position;
    uint8_t _msg_command;
    uint8_t _msg_length;
    Stream* _serial;

    void pump_heart(uint32_t now);

    uint32_t _heartrate;
    int _last_heartbeat_sent;
    int _last_heartbeat_received;
    bool _is_connected;
    HeartBeatStatus _hb_status;
    bool _is_master;
};
#endif // SERIAL_COMPONENT_H
