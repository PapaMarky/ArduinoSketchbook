

class State {
  public:
  State() {}
  virtual void OnEnter() {}
  virtual void OnExit() {}
  virtual void loop() {}
  virtual void OnEvent(int event) {}
};

class StatePowerUp : public State {
  public:
  void OnEnter();
  void OnExit();
  void OnEvent(int event);
};


