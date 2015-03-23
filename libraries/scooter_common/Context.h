// Copyright 2015, Mark Dyer
#ifndef CONTEXT_H
#define CONTEXT_H

#include "Component.h"

const int max_components = 5;

class Context {
  public:

    void init() {_nComponents = 0; }
    // adds the component, returns it's id
    // returns < 0 if there is an error
    int addComponent(Component* c) {
      //      Serial.print("Context::addComponent: 0x"); Serial.println((int)(void*)c, HEX);
      //      Serial.print(" _nComponents: "); Serial.print(_nComponents); 
      //      Serial.print(", max: "); Serial.println(max_components);
      if (_nComponents >= max_components) 
        return -1;

      int id = _nComponents;
      _nComponents++;
      _comps[id] = c;

      return id;
    }
    
    // get the component for 'id'
    // returns 0 (null) if not found
    Component* getComponent(int id) {
      if (id < 0 || id >= _nComponents)
	return 0;
      return _comps[id];
    }

    // Call each component's setup() function
    void setup() {
      for(int i = 0; i < _nComponents; i++) {
        _comps[i]->setup();
      }
    }
    
    // Call each component's loop() function
    void loop(uint16_t now) {
      //      gdbg->DEBUG("Context loop");
      char b[50];
      for(int i = 0; i < _nComponents; i++) {
	//	snprintf(b,50,"component %d", i);
	//	gdbg->DEBUG(b);
        _comps[i]->loop(now);
      }
    }

  private:
    int _nComponents;
    Component* _comps[max_components];
};
#endif // CONTEXT_H

