
#include <Arduino.h>

#include "Context.h"
#include "LcdComponent.h"
#include "flcp_state.h"
#include "LaserAssembly.h"

void StatePowerUp::OnEnter() {
  Serial.println("StatePowerUp::OnEnter()");
  _start = millis();
  _lcd_connected = false;
  lcd_start_wait = _start;
  Serial.println("Setting LaserSeen to false");
  _laser_ready = false;
  _laser_seen = false;
}

bool StatePowerUp::checkLaserState(uint32_t now) {
  if (_laser_ready)
    return false;

  LaserAssembly* laser = static_cast<LaserAssembly*>(g_context->getComponent(g_laser_id));
  if (! _laser_seen && laser->isVisible()) {
    _laser_seen = true;
    Serial.println("Laser Seen");
    _laser_aquired_time = now;
  }

  if (_laser_seen) {
    if (laser->isVisible() && (now - _laser_aquired_time) >= LASER_READY_TIME) {
      _laser_ready = true;
      Serial.println("Laser Ready");
      return true;
    }
    if (!laser->isVisible()) {
      _laser_seen = false;
    }
  }
  return false;
}

void StatePowerUp::loop() {
  uint32_t now = millis();
  LcdComponent* lcd = static_cast<LcdComponent*>(g_context->getComponent(g_lcd_id));

  if (! _lcd_connected) {
    _lcd_connected = lcd->isConnected();

    if (_lcd_connected) {
      Serial.println("LCD connected");
      if (_laser_ready) {
	Serial.println("Sending laser state : 1");
	lcd->send_message(SerialComponent::msg_laser_state, (byte*)"1", 1);
      }
      else {
	Serial.println("Sending laser state : 0");
	lcd->send_message(SerialComponent::msg_laser_state, (byte*)"0", 1);
      }
    }
  }

  if(checkLaserState(now) && _lcd_connected) {
    Serial.println("Sending base:laser:1");
    lcd->send_message(SerialComponent::msg_laser_state, (byte*)"1", 1);
  }
}

void StatePowerUp::OnExit() {
  Serial.println("StatePowerUp::OnExit");
}

bool StatePowerUp::OnEvent(int event) {
  Serial.println("StatePowerUp::OnEvent");
}

