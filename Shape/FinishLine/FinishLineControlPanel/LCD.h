#include <SoftwareSerial.h>

class LCD {
  public:
  LCD(SoftwareSerial* s) : _serial(s) {}
  
  void setup() {
    _serial->begin(9600);
    clear();
  }
  
  void clear() {
    _serial->write(0xFE);
    _serial->write(0x01);
    _serial->write(0xFE);
    _serial->write(128);
  }
  
  private:
  SoftwareSerial* _serial;
};

