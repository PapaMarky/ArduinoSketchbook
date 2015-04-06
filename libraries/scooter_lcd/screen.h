#ifndef SCREEN_H
#define SCREEN_H
#include <arduino.h>
#include "Component.h"
#include "Controller.h"

extern AnalogOutComponent red_led;
extern AnalogOutComponent yellow_led;
extern AnalogOutComponent green_led;
extern AnalogOutComponent go_button_led;

extern Controller g_controller;

#include "BaseComponent.h"
extern BaseComponent base;

class LiquidCrystal;

class Screen {
  public:
  Screen(LiquidCrystal* lcd) : _lcd(lcd) {}

  virtual void setup() = 0;
  virtual void onEnter() = 0;
  virtual void onLeave() {}
  virtual void update(uint32_t now) = 0;

  void set_line(int linenum, char* str);

  enum ScreenType {
    scr_none = 0,
    scr_console = 1,
    scr_general = 2,
    scr_startup = 2
    
  };
  virtual ScreenType type() = 0;
  virtual bool onMessage(uint16_t cmd, uint8_t len, byte* buffer) = 0;

  protected:
  LiquidCrystal* _lcd;
};

class GeneralScreen : public Screen {
 public:
 GeneralScreen(LiquidCrystal* lcd) : Screen(lcd), _logo_rotate_start(0), _showing_logo(true) {}
  virtual void setup();
  virtual void onEnter();
  virtual void update(uint32_t now);
  virtual ScreenType type() {return scr_general;}
  static void load_shape_logo(LiquidCrystal* lcd);

 protected:
  void display_shape_logo();

  uint32_t _logo_rotate_start;
  bool _showing_logo; // we are either showing the logo or "scooterbot"
};

class StartupScreen : public GeneralScreen {
 public:
 StartupScreen(LiquidCrystal* lcd) : GeneralScreen(lcd) {}
  virtual void onEnter();
  virtual bool onMessage(uint16_t cmd, uint8_t len, byte* buffer);
};
extern StartupScreen* g_startup_screen;

class ReadyScreen : public GeneralScreen {
 public:
 ReadyScreen(LiquidCrystal* lcd) : GeneralScreen(lcd) {}
  virtual void onEnter();
  virtual bool onMessage(uint16_t cmd, uint8_t len, byte* buffer);
 private:
  void onLaserOn();
  void onLaserOff();
};
extern ReadyScreen* g_ready_screen;

class CountdownScreen : public GeneralScreen {
 public:
 CountdownScreen(LiquidCrystal* lcd) : GeneralScreen(lcd) {}
  virtual void onEnter();
  virtual bool onMessage(uint16_t cmd, uint8_t len, byte* buffer);
  virtual void update(uint32_t now);
 private:
  byte _n;
  byte _ndots;
  uint32_t _dot_start;
};

extern CountdownScreen* g_countdown_screen;

class FoulScreen : public GeneralScreen {
 public:
 FoulScreen(LiquidCrystal* lcd) : GeneralScreen(lcd) {}
  void set_message(char* msg);
  virtual void onEnter();
  virtual bool onMessage(uint16_t cmd, uint8_t len, byte* buffer);
  virtual void update(uint32_t now);
 private:
  char _msg[21];
  uint32_t _blink_start;
};
extern FoulScreen* g_foul_screen;

class TimingScreen : public GeneralScreen {
 public:
 TimingScreen(LiquidCrystal* lcd) : GeneralScreen(lcd) {}
  virtual void onEnter();
  virtual bool onMessage(uint16_t cmd, uint8_t len, byte* buffer);
  virtual void update(uint32_t now);

  static void elapsedToString(uint32_t elapsed, char* buffer);

 private:
  void showSpinner(uint32_t elapsed);
  uint32_t _start;
};
extern TimingScreen* g_timing_screen;

class ResultsScreen : public GeneralScreen {
 public:
 ResultsScreen(LiquidCrystal* lcd) : GeneralScreen(lcd) {}
  virtual void onEnter();
  virtual bool onMessage(uint16_t cmd, uint8_t len, byte* buffer);
};

#define CONSOLE_LINES 3
class ConsoleScreen : public Screen {
  public:
  ConsoleScreen(LiquidCrystal* lcd) : Screen(lcd) {}
  
  virtual void setup();
  virtual void onEnter();
  virtual void update(uint32_t now);
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

