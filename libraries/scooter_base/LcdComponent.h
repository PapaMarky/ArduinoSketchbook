// copyright 2015, Mark Dyer
#ifndef LCD_COMPONENT_H
#define LCD_COMPONENT_H
#include "SerialComponent.h"
#include "scooter_common.h"

class LcdComponent : public SerialComponent {
 public:
 LcdComponent() : _ready(false) {}

  void handleMessage(uint8_t cmd, uint8_t len, byte* buffer);
  bool isReady() {return _ready;}

 private:
  bool _ready;
};

#endif // LCD_COMPONENT_H
