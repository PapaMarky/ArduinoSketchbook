
#include <Arduino.h>

#include "Context.h"
#include "Component.h"
#include "flcp_state.h"
#include "DataBase.h"

extern DataBase data;



void StatePowerUp::OnEnter() {
  Serial.println("StatePowerUp::OnEnter()");
  _start = millis();
  //lcd.setText("Scooter Madness", "v1.0");
  lcd_start_wait = 1000;
}

void StatePowerUp::loop() {
  LcdComponent* lcd = static_cast<LcdComponent*>(g_context->getComponent(g_lcd_id));
  if (! _lcd_ready) {
    _lcd_ready = lcd->isReady();
  }

  uint16_t now = millis();
  if (! _lcd_ready && now - lcd_start_wait >= 500) {
    lcd_start_wait = now;
    lcd->send_message("base:start");
  }
  if (data.isReady()) {
    // update LCD
    // fire power-up-done event
  }
}

void StatePowerUp::OnExit() {
}

bool StatePowerUp::OnEvent(int event) {
}

