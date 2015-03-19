// Copyright 2015, Mark Dyer
#ifndef LASER_ASSEMBLY_H
#define LASER_ASSEMBLY_H
#include <Arduino.h>

#include "Component.h"

class LaserAssembly : public Component {
 public:

  bool isVisible() {
    return _value > 188;
  }

  int value() {
    return _value;
  }

  void initialize(int led_pin, int photocell_pin) {
    _led_pin = led_pin;
    _photocell_pin = photocell_pin;
    _value = 0;
  }
  // Component Implementation
  virtual void setup() {
    pinMode(_led_pin, OUTPUT);
    pinMode(_photocell_pin, INPUT);
  }
    
  virtual void loop(uint32_t now) {
    // Set LED based on photocell reading
    _value = 0;
    for(int i = 0; i < 5; i++) {
      _value += analogRead(_photocell_pin);
      delay(5);
    }
    _value = _value/20;
    char bb[50];
    snprintf(bb,50,"Laser: %d (%s)", _value, isVisible() ? "VISIBLE" : "NOT VISIBLE");
    gdbg->DEBUG(bb);

    int n = isVisible() ? 255 : 0;
    analogWrite(_led_pin, n);
  }
  
  // LaserAssembly Implementation

 private:
  int _led_pin;
  int _photocell_pin;
  int _value;
};

#endif // LASER_ASSEMBLY_H

