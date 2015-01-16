// Copyright 2015, Mark Dyer
#ifndef LASER_ASSEMBLY_H
#define LASER_ASSEMBLY_H
#include <Arduino.h>

#include "Component.h"

class LaserAssembly : public Component {
  public:
    LaserAssembly(int led_pin, int photocell_pin) 
     : _led_pin(led_pin),
       _photocell_pin(photocell_pin),
       _value(0)
     {}

    // Component Implementation
    virtual void setup() {
      pinMode(_led_pin, OUTPUT);
      pinMode(_photocell_pin, INPUT);
    }
    
    virtual void loop() {
      // Set LED based on photocell reading
      _value = 0;
      for(int i = 0; i < 20; i++) {
        _value += analogRead(_photocell_pin);
        delay(5);
      }
      _value = _value/20;

      float fvalue = 100.0 * (float)_value / 1023.0;
      int n = 0;
      /*
      if(_value >= 0 && _value < 511)
        n = 5;
      else {
        int nn = _value - 512;
        float f = ((float)(nn)/511.0);
        //Serial.println(f,2);
        n = (int)(f * 250.0) + 5;
      }
      */
      n = _value > 800 ? 255 : 0; //_value/4;
      /*
      Serial.print("PHOTOCELL: raw: "); Serial.print(_value);
      Serial.print(", ("); Serial.print(fvalue, 2); Serial.print("%)");
      Serial.print(", n = "); Serial.print(n);
      Serial.println("");
      */
      analogWrite(_led_pin, n);
    }
    
    // LaserAssembly Implementation

  private:
    int _led_pin;
    int _photocell_pin;
    int _value;
};

#endif // LASER_ASSEMBLY_H

