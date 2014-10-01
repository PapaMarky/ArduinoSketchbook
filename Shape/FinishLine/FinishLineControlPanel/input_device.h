// 

class InputDevice {
  public:
  virtual void loop() = 0;
};

<<<<<<< HEAD
class LaserDetector {
  public:
  virtual void loop();
};

class GoButton {
  public:
  virtual void loop();
=======
class LaserDetector : public InputDevice {
  public:
  virtual void loop() {}
};

class GoButton : public InputDevice {
  public:
  virtual void loop() {}
>>>>>>> FinishLine
};

