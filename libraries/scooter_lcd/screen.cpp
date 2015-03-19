#include "screen.h"
#include "debug_lcd.h"

#include <string.h>
#include <LiquidCrystal.h>

Screen* g_current_screen = 0;

void ConsoleScreen::onEnter() {
  _lcd->clear();
  line0 = 0;
  n_lines = 0;
  dirty = false;
  for (int i = 0; i < CONSOLE_LINES; i++) {
    line_len[i] = 0;
  }
  addLine("ScooterBot");
}

void ConsoleScreen::update(uint32_t now) {
}

void ConsoleScreen::draw() {
  char buff[22];
  snprintf(buff, 21, "draw(), dirty: %s", dirty ? "T" : "F");
  gdbg->DEBUG(buff);
  if (!dirty)
    return;
    
  dirty = false;
  int n = line0 - 1;
  if (n < 0) {
    n = CONSOLE_LINES - 1;
  }
  for (int i = 0; i < n_lines; i++) {
    snprintf(buff,21, "%d:%d:%s", n,n_lines,lines[n]);
    gdbg->DEBUG(buff);
    _lcd->setCursor(0, i);
    _lcd->print(lines[n]);
 
    int l = strlen(lines[n]);
    for (byte s = 0; s < line_len[i] - l; s++) {
      _lcd->write(' ');
    }
    line_len[i] = l;
    
    n--;
    if (n < 0) {
      n = CONSOLE_LINES - 1;
    }
}
}
static int hb_count = 0;
void ConsoleScreen::onHeartbeat() {
  hb_count++;
  char b[20];
  sprintf(b, "Heartbeat: %d", hb_count);
  addLine(b);
}

void ConsoleScreen::addLine(const char* line) {
  char buff[22];
  strncpy(lines[line0], line, 20);
  snprintf(buff, 21, "add:%d:%d:%s", line0,n_lines,lines[line0]);
  gdbg->DEBUG(buff);
  //snprintf(buff, 21, "NL: %d, L0: %d", n_lines, line0);
  //gdbg->DEBUG(buff);

  if (n_lines < CONSOLE_LINES) {
    n_lines++;
  }
  line0++;
  if (line0 >= CONSOLE_LINES) {
    line0 = 0;
  }
  dirty = true;
  //gdbg->DEBUG("Line Added");
}

