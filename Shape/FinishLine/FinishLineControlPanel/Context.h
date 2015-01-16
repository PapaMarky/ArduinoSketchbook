// Copyright 2015, Mark Dyer
#include "Component.h"

const int max_components = 5;

class Context {
  public:
    Context() : _nComponents (0) {}

    // adds the component, returns it's id
    // returns < 0 if there is an error
    int addComponent(Component* c) {
      if (_nComponents >= max_components) 
        return -1;

      int id = _nComponents;
      _nComponents++;
      _comps[id] = c;

      return id;
    }
    
    // get the component for 'id'
    // returns 0 (null) if not found
    Component* getComponent(int id);

    // Call each component's setup() function
    void setup() {
      for(int i = 0; i < _nComponents; i++)
        _comps[i]->setup();
    }
    
    // Call each component's loop() function
    void loop() {
      for(int i = 0; i < _nComponents; i++)
        _comps[i]->loop();
    }

  private:
    int _nComponents;
    Component* _comps[max_components];
};

