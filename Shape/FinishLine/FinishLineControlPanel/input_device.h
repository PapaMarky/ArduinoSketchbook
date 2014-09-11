// 

class InputDevice {
  public:
  virtual void loop() = 0;
};

class LaserDetector {
  public:
  virtual void loop();
};

class GoButton {
  public:
  virtual void loop();
};

