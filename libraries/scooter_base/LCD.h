#ifndef LCD_H
#define LCD_H

#include <SoftwareSerial.h>

class LCD {
  public:
  LCD(SoftwareSerial* s) : _serial(s) {
    Serial.println("LCD::CTOR()");
    
    _serial->begin(9600);
  }
  
  void setup() {
    Serial.println("LCD::setup()");
  }
  
  void setText(char* line0, char* line1) {
    clear();
    Serial.print("LCD::setText('");
    Serial.print(line0);
    Serial.print("', '");
    Serial.print(line1);
    Serial.println("')");
    
    int len = strLen(line0);
    if (len > 0 && len < 17) {
      setCursor(0,0);
      _serial->print(line0);
    }
    
    len = strLen(line1);
    if (len > 0 && len < 17) {
      setCursor(1,0);
      _serial->print(line1);
    }
  }
  
  void clear() {
    // clear
    _serial->write(0xFE);
    _serial->write(0x01);
    // move cursor to 0,0
    _serial->write(0xFE);
    _serial->write(128);
  }
  
  void clearLine(int line) {
    if (line < 0 || line >= 2) return;
    setCursor(line, 0);
    _serial->write("                ");
    setCursor(line, 0);
  }
  
  void setCursor(int line, int pos) {
    if (line < 0 || line > 1 || pos < 0 || pos > 15) return;
    
    _serial->write(0xFE);
    int linestart = 128 + (line == 0 ? 0 : 64);
    _serial->write(linestart + pos);
  }
  
  private:
  
  // truncates strings that are too long.
  int strLen(char* s) {
    if (s == 0) return 0;
    int len = 0;
    while(*(s + len) != '\0') {
      len++;
      if (len >= 16) {
        *(s + 16) = '\0';
        len = 16;
        break;
      }
    }
    return len;
  }
  SoftwareSerial* _serial;
};

#endif // LCD_H
