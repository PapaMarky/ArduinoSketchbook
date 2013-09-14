// Copyright (c) 2013 Mark Dyer. All rights reserved.
#ifndef STATE_MACHINE_H__
#define STATE_MACHINE_H__

#include <arduino.h>
#include "../mdEvents/EventQueue.h"

namespace mdlib {

class StateContext {
 public:
  StateContext() {}
  virtual ~StateContext() {}

  virtual void setup() = 0;
  virtual void update() = 0;

  // add setters and getters for context items
 private:
  // context items (things the states need to manipulate)
  // buttons do not belong here, because they generate events
  // that are handled by the States' handle_event(...) overloads
};

class State {
 public:
  State() {};
  virtual ~State();

  static void set_context(StateContext* context) { s_context = context; }

  virtual void enter_state() {};
  virtual void leave_state() {};

  virtual State* loop() = 0;
  virtual State* handle_event(mdlib::Event e) = 0;

  void set_next_state(State* next_state) {next_state_ = next_state;}
  void set_timeout_next_state(State* timeout_next_state) {timeout_next_state_ = timeout_next_state;}

  virtual const char* name() const = 0;

  static StateContext* s_context;
 protected:
  unsigned long start_time_;
  State* next_state_;
  State* timeout_next_state_;

  void SetStartTime() { start_time_ = millis(); }
};
 
} // namespace mdlib
#endif // STATE_MACHINE_H__
