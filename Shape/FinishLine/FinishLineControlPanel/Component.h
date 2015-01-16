// Copyright 2015, Mark Dyer

#ifndef _COMPONENT_H_
#define _COMPONENT_H_

class Component {
  public:
  virtual void loop() = 0;
  virtual void setup() = 0;
};

#endif // _COMPONENT_H_

