#include <LiquidCrystal.h>
#include "debug_lcd.h"
extern LiquidCrystal lcd;

void padMsg(char* buf, int len) {
  while (len < 21) {
    buf[len] = ' ';
    len++;
  }
  buf[21] = '\0';
}

#define DO_DEBUG 0
void debug_lcd::DEBUG(const char* msg) {
#if DO_DEBUG
  char buf[32];
  int len = snprintf(buf, 21, "%s", msg);
  padMsg(buf, len);
  lcd.setCursor(0, 3);
  lcd.print(buf);
  /*
  len = snprintf(buf, 21, "free Ram: %d", freeRam());
  padMsg(buf, len);
  lcd.setCursor(0, 3);
  lcd.print(buf);
*/
  delay(1000);
#endif // DO_DEBUG

}

