// 

class InputDevice {
  public:
  virtual void loop() = 0;
};

class LaserDetector : public InputDevice {
  public:
  virtual void loop() {}
};

class GoButton : public InputDevice {
  public:
  virtual void loop() {}
};

