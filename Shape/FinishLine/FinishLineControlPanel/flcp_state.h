

class State {
  public:
  State() {}
  virtual void OnEnter() {}
  virtual void OnExit() {}
  virtual void loop() {}
  virtual bool OnEvent(int event) { return true; }
};

const int NUMBER_OF_STATES = 3;

class StatePowerUp : public State {
  public:
  void OnEnter();
  void OnExit();
  bool OnEvent(int event);
};

class StateCheckDisk : public State {
  public:
  void OnEnter();
  void OnExit();
  bool OnEvent(int event);
};

class StateAquireLaser : public State {
  public:
  void OnEnter();
  void OnExit();
  bool OnEvent(int event);
};

