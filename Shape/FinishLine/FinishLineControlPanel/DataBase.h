
#include <SoftwareSerial.h>

class DataBase {
  public:
  DataBase(SoftwareSerial* serial, int reset) : 
  _serial(serial), _reset(reset)
  {  }
  
  void setup() {
    pinMode(_reset, OUTPUT);
    _serial->begin(9600);
    reset();
    
    startCommandMode();
    
  }
  
  boolean isReady() { return _ready; }
  
  void reset() {
    digitalWrite(_reset, LOW);
    delay(100);
    digitalWrite(_reset, HIGH);
  }
    
  private:
  void startCommandMode() {
    // TODO - LOOK FOR '!' in prompt.
    //Wait for OpenLog to respond with '<' to indicate it is alive and recording to a file
    boolean ready = false;
    while(1) {
      if(_serial->available()) {
        char c = _serial->read();
        if(c == '<') break;
        if(c == '>') {
          ready = true;
          break;
        }
      }
    }
    
    if (!ready) {
      // 26 is ^Z
      _serial->write(26);
      _serial->write(26);
      _serial->write(26);
      
      //Wait for OpenLog to respond with '>' to indicate we are in command mode
      while(1) {
        if(_serial->available()) {
          char c = _serial->read();
          if(c == '>') {
            break;
          }
        }
      }
    }  
  }
  
  SoftwareSerial* _serial;
  int _reset;
  boolean _ready;
};

