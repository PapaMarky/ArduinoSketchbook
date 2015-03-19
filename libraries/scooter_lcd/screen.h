#ifndef SCREEN_H
#define SCREEN_H
#include <arduino.h>

class LiquidCrystal;

class Screen {
  public:
  Screen(LiquidCrystal* lcd) : _lcd(lcd) {}
  
  virtual void onEnter() = 0;
  virtual void onLeave() {}
  virtual void update(uint32_t now) = 0;
  virtual void draw() = 0;
  virtual void onHeartbeat() {};
  enum ScreenType {
    scr_none = 0,
    scr_console = 1
  };
  virtual ScreenType type() = 0;

  protected:
  LiquidCrystal* _lcd;
};

extern Screen* g_current_screen;

//#define CONSOLE_LINES 4
#define CONSOLE_LINES 3
class ConsoleScreen : public Screen {
  public:
  ConsoleScreen(LiquidCrystal* lcd) : Screen(lcd) {}
  
  virtual void onEnter();
  virtual void update(uint32_t now);
  virtual void draw();
  virtual void onHeartbeat();
  virtual ScreenType type() {return Screen::scr_console; }
  
  void addLine(const char* line);

  private:
  char lines[CONSOLE_LINES][21];
  uint8_t line_len[CONSOLE_LINES];
  int line0;
  int n_lines;
  bool dirty;
};

#endif // SCREEN_H

