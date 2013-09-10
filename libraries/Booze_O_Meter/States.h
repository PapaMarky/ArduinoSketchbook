// Copyright (c) 2013 Mark Dyer. All rights reserved.
#ifndef BOM_STATE_MACHINE_H__
#define BOM_STATE_MACHINE_H__

#include "../mdLib/StateMachine.h"

#include "EventQueue.h"
#include "Timers.h"

namespace BOM {

class TimedState : public mdlib::State {
 public:
 TimedState() : timeout_(5*60*1000) {}
  virtual ~TimedState() {}

  virtual void enter_state() { mdlib::State::enter_state(); StartTimer(); }
  virtual void leave_state() { StopTimer(); }

 protected:
  void StartTimer() { timer_.Start(timeout_); }
  void StopTimer() { timer_.Stop(); }
  void SetTimeout(unsigned long t) { timeout_ = t; }
  void UpdateTimer() { timer_.update(); }

 protected:
  mdlib::CountdownTimer timer_;

 private:
  unsigned long timeout_;
};

class StartUpState : public mdlib::State {
 public:
  StartUpState() {}

  virtual void enter_state();
  virtual void leave_state();

  virtual mdlib::State* loop();
  virtual mdlib::State* handle_event(mdlib::Event e);

  virtual ~StartUpState() {}

  virtual const char* name() const { return "StartUpState"; }
 private:
  static const unsigned int DURATION = 8000;
  // 0 : 0x7B : digit 1 segments ( 0 -  7)
  // 1 : 0x7C : digit 2 segments ( 8 - 15)
  // 2 : 0x7D : digit 3 segments (16 - 23)
  // 3 : 0x7E : digit 4 segments (24 - 31)
  // 4 : 0x77 : decimal point segments (32 - 37)
  uint8_t segments[5];

  // 'n' is between 0 and 37 inclusive
  // returns 'true' if segment is set successfully
  bool TryToSetSegment(int n);
};

class WarmUpState : public mdlib::State {
 public:
 WarmUpState() : display_mode_(GRAPHIC) {}
  ~WarmUpState() {}

  virtual void enter_state();
  virtual void leave_state();

  virtual mdlib::State* loop();
  virtual mdlib::State* handle_event(mdlib::Event e);

  virtual const char* name() const { return "WarmUpState"; }

 private:
  void RotateDisplayMode();
  void UpdateGraphicDisplay();
  
  enum DisplayMode {
    GRAPHIC,
    ALCOHOL_STDDEV,
    THERMISTOR_STDDEV,
    ALCOHOL_VALUE,
    THERMISTOR_VALUE
  };

  DisplayMode display_mode_;

  // BoozeSensor::DataTrend
  int alcohol_trend_;
  int temperature_trend_;
 
  float display_value_;
  
  unsigned long pulse_start_;

  void SetDataStableSize(float s);
  double data_stable_size_;
};

class ReadyState : public TimedState {
 public:
  ReadyState() { SetTimeout(3L * 60L * 1000L); }
  ~ReadyState() {}
  
  virtual void enter_state();
  virtual void leave_state();

  virtual mdlib::State* loop();
  virtual mdlib::State* handle_event(mdlib::Event e);

  virtual const char* name() const { return "ReadyState"; }
 private:

};

class SamplingState : public TimedState {
 public:
  SamplingState() {
    // go to PostSampleState after 10 seconds
    SetTimeout(10L*1000L);
  }
  ~SamplingState() {}

  virtual void enter_state();
  virtual void leave_state();

  virtual mdlib::State* loop();
  virtual mdlib::State* handle_event(mdlib::Event e);

  virtual const char* name() const { return "SamplingState"; }
 private:
  void UpdateDisplay();
  int start_sample_;
};

class PostSampleState : public TimedState {
 public:
  PostSampleState() { SetTimeout(60L*1000L); }
  ~PostSampleState() {}

  virtual void enter_state();
  virtual void leave_state();

  virtual mdlib::State* loop();
  virtual mdlib::State* handle_event(mdlib::Event e);

  virtual const char* name() const { return "PostSampleState"; }
 private:
};

class PostSample2State : public TimedState {
 public:
  PostSample2State() { SetTimeout(60L*1000L); }
  ~PostSample2State() {}

  virtual void enter_state();
  virtual void leave_state();

  virtual mdlib::State* loop() { return 0;}
  virtual mdlib::State* handle_event(mdlib::Event e);

  virtual const char* name() const { return "PostSampleState 2"; }
 private:
};

class SleepState : public mdlib::State {
 public:
  SleepState() {}
  ~SleepState() {}

  virtual void enter_state();

  virtual mdlib::State* loop();
  virtual mdlib::State* handle_event(mdlib::Event e);

  virtual const char* name() const { return "SleepState"; }
 private:
};

class PowerSaverState : public TimedState {
 public:
  PowerSaverState() { SetTimeout(20L * 60L * 1000L); }
  ~PowerSaverState() {}

  virtual void enter_state();
  virtual void leave_state();
  
  virtual mdlib::State* loop();
  virtual mdlib::State* handle_event(mdlib::Event e);

  virtual const char* name() const { return "PowerSaverState"; }
 private:
};
}

#endif // BOM_STATE_MACHINE_H__
