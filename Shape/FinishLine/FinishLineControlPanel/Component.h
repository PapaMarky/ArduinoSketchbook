// Copyright 2015, Mark Dyer

#ifndef _COMPONENT_H_
#define _COMPONENT_H_

#include <Arduino.h>

class SoftwareSerial;

class Component {
  public:
  virtual void loop() = 0;
  virtual void setup() = 0;
};

class SerialComponent : public Component {
  public:
 SerialComponent(SoftwareSerial* serial, int baud) : _serial(serial), _message_length(0), _baud(baud) {
    }
  
    virtual void loop() = 0;
    virtual void setup();
    int have_message() { return _message_length; }
    bool send_message(char*);

 protected:
    int check_for_message();
    void flush_message() {
      _message_length = 0;
    }
    static const int BUFFER_LENGTH = 64;
    char _buffer[BUFFER_LENGTH];
  private:
    SoftwareSerial* _serial;
    int _message_length;
    int _baud;
};

class LcdComponent : public SerialComponent {
  public:
 LcdComponent(SoftwareSerial* lcd, int baud) : SerialComponent(lcd, baud), _is_ready(false) {}
    virtual void loop();
    bool isReady() { return _is_ready; }
  private:
    bool _is_ready;
};
#endif // _COMPONENT_H_

