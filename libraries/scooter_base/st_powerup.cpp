// Copyright 2015, Mark Dyer
#include <Arduino.h>

#include "Context.h"
#include "LcdComponent.h"
#include "flcp_state.h"
#include "flcp_statemachine.h"
#include "LaserAssembly.h"

void StatePowerUp::OnEnter() {
  //  Serial.println("StatePowerUp::OnEnter()");
  _start = millis();
  _lcd_connected = false;
  lcd_start_wait = _start;
  //  Serial.println("Setting LaserSeen to false");
  _laser_ready = false;
  _laser_seen = false;
}

bool StatePowerUp::checkLaserState(uint32_t now) {
  if (_laser_ready)
    return false;

  if (! _laser_seen && g_laser->isVisible()) {
    _laser_seen = true;
    _laser_aquired_time = now;
  }

  if (_laser_seen) {
    if (g_laser->isVisible() && (now - _laser_aquired_time) >= LASER_READY_TIME) {
      _laser_ready = true;
      return true;
    }
    if (!g_laser->isVisible()) {
      _laser_seen = false;
    }
  }
  return false;
}

static uint32_t last_hello = -1001;
const int hello_wait = 1000;
static bool was_seen = false;
void StatePowerUp::loop(uint32_t now) {
  if (! _lcd_connected) {
    _lcd_connected = g_lcd->isConnected();

    if (_lcd_connected) {
      if (_laser_ready) {
	g_lcd->send_message(SerialComponent::msg_laser_ready);
      }
      else if (_laser_seen) {
	g_lcd->send_message(SerialComponent::msg_laser_seen);
	was_seen = true;
      }
    } else if (now - last_hello > hello_wait) {
      g_lcd->send_message(SerialComponent::msg_base_hello);
    }
  }

  if (checkLaserState(now) && _lcd_connected) {
    
    g_lcd->send_message(SerialComponent::msg_laser_ready);
  }
  else if (_lcd_connected && _laser_seen && ! was_seen) {
    g_lcd->send_message(SerialComponent::msg_laser_seen);
    was_seen = true;
  }

  if (_laser_ready && g_lcd->isReady()) {
    g_stateMachine->set_state(&g_stReady);
  }
}

void StatePowerUp::OnExit() {
  //  gdbg->DEBUG("StatePowerUp::OnExit");
}

