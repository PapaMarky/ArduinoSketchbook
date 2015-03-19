#ifndef DEBUG_H
#define DEBUG_H

class debug {
 public:
  debug();
  virtual ~debug();

  virtual void DEBUG(const char* msg) = 0;
  int freeRam();
};

extern debug* gdbg;

#endif // DEBUG_H

