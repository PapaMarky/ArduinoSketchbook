#ifndef TIMER_H
#define TIMER_H

class Timer {
  public:
 Timer(): _start(0), _active(false) {}
  void start(unsigned long duration, int event) {
    _start = millis();
    _active = true;
    _event = event;
    _duration = duration;
  }
  void stop() {
    _active = false;
  }
  
  void loop() {
    if (_active) {
      if (elapsed() >= _duration) {
        // fire the event
        stop();
    }
  }
  
  unsigned long elapsed() {
    return _active ? millis() - _start : 0;
  }
  
  private:
  unsigned long _start;
  unsigned long _duration;
  boolean _active;
  int _event;
};

#endif // TIMER_H

