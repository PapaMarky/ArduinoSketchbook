// Copyright (c) 2013 Mark Dyer. All rights reserved.

#include <Arduino.h>
#include <Narcoleptic.h>
#include "../mdlib/mdBase.h"
#include "BoozeSensor.h"
#include "LightedButton.h"
#include "MultiColorLED.h"
#include "SevenSegmentDisplay.h"
#include "StateContext.h"
#include "States.h"

namespace mdlib {
  extern const char* event_name(mdlib::Event e);
}

namespace BOM {
  const float DISPLAY_NORMAL = 1.0;
  const float DISPLAY_DIM = 0.83;

////////////////////////////////////////////////////////////////////
//      StartUpState
////////////////////////////////////////////////////////////////////

void StartUpState::enter_state() {
  SetStartTime();

  StateContext::Get()->led()->set_color(mdlib::WHITE);
  StateContext::Get()->fan()->TurnOn();
  StateContext::Get()->sensor()->TurnOn();
  StateContext::Get()->button()->TurnOn();
  StateContext::Get()->display()->Reset();
  StateContext::Get()->display()->SetBrightness(DISPLAY_NORMAL);
  //  StateContext::Get()->display()->set(8888);

  for(int i = 0; i < 5; i++)
    segments[i] = 0x00;
}

void StartUpState::leave_state() {
  StateContext::Get()->led()->set_color(mdlib::BLACK);
  StateContext::Get()->fan()->TurnOff();
  StateContext::Get()->button()->TurnOff();
}

mdlib::State* StartUpState::loop() {
  unsigned long elapsed = millis() - start_time_;
  static int segments = 0;
  static int gap = 0;
  static bool bright = false;
  if (segments < 38) {
    if (gap == 0) {
      int n = random(0,38);
      bool found = TryToSetSegment(n);
      int i = 1;
      while (!found && i < 38) {
	if (n - i >= 0)
	  if(found = TryToSetSegment(n - i))
	    break;
	if (n + i < 38)
	  found = TryToSetSegment(n + i);

	i++;
      }
      if(found) {
	segments++;
	if (segments == 38) {
	  StateContext::Get()->display()->SetBrightness(DISPLAY_NORMAL);
	}
	else if (segments % 5 == 0) {
	  StateContext::Get()->display()->SetBrightness(bright ? DISPLAY_NORMAL : DISPLAY_DIM);
	  bright = !bright;
	}
      }
    }
    gap++;
    if (gap > 12)
      gap = 0;
  }
  if (elapsed >= DURATION)
    return next_state_;

  int b = (elapsed / 333) % 3;

  switch (b) {
  case 0:
    StateContext::Get()->button()->SetBrightness(0.33);
    break;
  case 1:
    StateContext::Get()->button()->SetBrightness(0.66);
    break;
  case 2:
    StateContext::Get()->button()->SetBrightness(1.00);
    break;
  }
  
  int h = (int)(360.0f * (float)elapsed / (float)DURATION);
  while (h > 360) h -= 360;
  while (h < 0) h += 360;
  StateContext::Get()->led()->set_hsv(h, 1.0f, 1.0f);
  return (mdlib::State*)0;
}

mdlib::State* StartUpState::handle_event(mdlib::Event e) {
  return (mdlib::State*)0;
}

static byte SEGMENT_COMMAND[5] = {0x7B, 0x7C, 0x7D, 0x7E, 0x77};

bool StartUpState::TryToSetSegment(int n) {
  int b = n / 8;
  int s = n % 8;
  uint8_t bit = 0x01 << s;

  Serial.print("TryToSetSegment(");
  Serial.print(n);
  Serial.print(") b: ");
  Serial.print(b);
  Serial.print(", s: ");
  Serial.print(s);
  Serial.print(", bit: ");
  Serial.print(bit, BIN);
  Serial.print(", cmd: 0x");
  Serial.print(SEGMENT_COMMAND[b], HEX);
  Serial.print(", ");
  Serial.print(segments[b] & bit ? "FALSE" : "TRUE");
  Serial.println("");

  if (segments[b] & bit)
    return false;

  segments[b] |= bit;
  StateContext::Get()->display()->SetSegment(SEGMENT_COMMAND[b], segments[b]);

  return true;
}
  
/////// WarmUpState
  const float BASE_STABLE_SIZE = 0.8;
void WarmUpState::enter_state() {
  SetStartTime();
  StateContext::Get()->sensor()->SetDataStableSize(BASE_STABLE_SIZE);
  StateContext::Get()->sensor()->TurnOn();
  StateContext::Get()->fan()->TurnOff();
  StateContext::Get()->button()->SetBrightness(0.5);
  StateContext::Get()->button()->TurnOff();
  StateContext::Get()->display()->clear();
  StateContext::Get()->display()->SetBrightness(DISPLAY_NORMAL);
  display_value_ = 0.0;
  pulse_start_ = millis();
}

void WarmUpState::leave_state() {
  // the user may have double clicked the button and turned the fan on,
  // make sure it is off
  StateContext::Get()->fan()->TurnOff();
  SetDataStableSize(BASE_STABLE_SIZE);
  //  Serial.println("Leaving WarmUpState");
}

mdlib::State* WarmUpState::loop() {
  const unsigned long PULSE_TIME = 450;
  const unsigned long GAP_TIME = 1550;
  unsigned long elapsed = millis() - pulse_start_;

  if (elapsed > PULSE_TIME + GAP_TIME) {
    pulse_start_ = millis();
    elapsed = 0;
  }

  //   0 - Red
  //  60 - Yellow
  // 120 - Green
  // 180 - Turquoise
  // 240 - Blue
  // 300 - Magenta

  int color = 60; // blue
  switch(display_mode_) {
  case GRAPHIC:
    color = 240; // blue
    break;
  case ALCOHOL_STDDEV:
  case ALCOHOL_VALUE:
    color = 300; // violet
    break;
  case THERMISTOR_STDDEV:
  case THERMISTOR_VALUE:
    color = 180; // turquoise
    break;
  }

  float v;
  if (elapsed < PULSE_TIME) {
    v = 1.0 - (float)elapsed / (float)PULSE_TIME;
    v *= v;
    StateContext::Get()->led()->set_hsv(color, 1.0, v);
  }
  else {
    StateContext::Get()->led()->set_hsv(color, 1.0, 0.005);
  }

  switch(display_mode_) {
  case GRAPHIC:
    UpdateGraphicDisplay();
    break;
  case ALCOHOL_STDDEV:
    v = StateContext::Get()->sensor()->DataStdDev();
    if (v != display_value_) {
      display_value_ = v;
      StateContext::Get()->display()->set(display_value_, 2);
    }
    break;
  case THERMISTOR_STDDEV:
    v = StateContext::Get()->sensor()->TempStdDev();
    if (v != display_value_) {
      display_value_ = v;
      StateContext::Get()->display()->set(display_value_, 2);
    }
    break;
  case ALCOHOL_VALUE:
    v = StateContext::Get()->sensor()->RawAlcoholValue();
    if (v != display_value_) {
      display_value_ = v;
      StateContext::Get()->display()->set((int)display_value_);
    }
    break;
  case THERMISTOR_VALUE:
    v = StateContext::Get()->sensor()->RawThermistor();
    if (v != display_value_) {
      display_value_ = v;
      StateContext::Get()->display()->set((int)display_value_);
    }
    break;
  }

  if (StateContext::Get()->sensor()->IsReady() || elapsed > (5 * 60 * 1000))
    return next_state_;

  if (data_stable_size_ == BASE_STABLE_SIZE && elapsed > (3 * 60 * 1000)) {
    SetDataStableSize(2 * BASE_STABLE_SIZE);
  }
  return (mdlib::State*)0;
}

void WarmUpState::SetDataStableSize(float s) {
  data_stable_size_ = s;
  StateContext::Get()->sensor()->SetDataStableSize(s);
}
mdlib::State* WarmUpState::handle_event(mdlib::Event e) {
  if (e.event_type == mdlib::Event::BUTTON_DOUBLE_CLICK) {
    if (StateContext::Get()->fan()->IsOn())
      StateContext::Get()->fan()->TurnOff();
    else
      StateContext::Get()->fan()->TurnOn();
  }
  else if (e.event_type == mdlib::Event::BUTTON_LONG_CLICK) {
    RotateDisplayMode();
  }
  return 0;
}

void WarmUpState::UpdateGraphicDisplay() {
  // A = 0x01 (TOP)
  // G = 0x40 (MIDDLE)
  // D = 0x08 (BOTTOM)
  // C + F = 0x24 (NOT READY)
  // A + G + D = 0x49 (UNSTABLE)
  int temperature_trend = StateContext::Get()->sensor()->GetTemperatureTrend();
  //  if (temperature_trend != temperature_trend_) {
  {
    temperature_trend_ = temperature_trend;
    byte mask;
    switch(temperature_trend_) {
    case BoozeSensor::DATA_NOT_READY:
      mask = 0x24;
      break;
    case BoozeSensor::DATA_RISING:
      mask = 0x01;
      break;
    case BoozeSensor::DATA_STABLE:
      mask = 0x40;
      break;
    case BoozeSensor::DATA_FALLING:
      mask = 0x08;
      break;
    case BoozeSensor::DATA_UNSTABLE:
      mask = 0x49;
      break;
    }
    StateContext::Get()->display()->SetDigitSegments(0, mask);
    StateContext::Get()->display()->SetDigitSegments(1, mask);
  }

  int alcohol_trend = StateContext::Get()->sensor()->GetAlcoholTrend();
  //  if (alcohol_trend != alcohol_trend_) {
  {
    alcohol_trend_ = alcohol_trend;
    byte mask;
    switch(alcohol_trend_) {
    case BoozeSensor::DATA_NOT_READY:
      mask = 0x24;
      break;
    case BoozeSensor::DATA_RISING:
      mask = 0x01;
      break;
    case BoozeSensor::DATA_STABLE:
      mask = 0x40;
      break;
    case BoozeSensor::DATA_FALLING:
      mask = 0x08;
      break;
    case BoozeSensor::DATA_UNSTABLE:
      mask = 0x49;
      break;
    }
    StateContext::Get()->display()->SetDigitSegments(2, mask);
    StateContext::Get()->display()->SetDigitSegments(3, mask);
  }
}
  
void WarmUpState::RotateDisplayMode() {
  display_value_ = -1; // force display update
  StateContext::Get()->display()->clear();
  switch(display_mode_) {
  case GRAPHIC:
    display_mode_ = ALCOHOL_STDDEV;
    Serial.print("ALCOHOL_STDDEV");
    break;
  case ALCOHOL_STDDEV:
    display_mode_ = ALCOHOL_VALUE;
    Serial.print("ALCOHOL_VALUE");
    break;
  case ALCOHOL_VALUE:
    display_mode_ = THERMISTOR_STDDEV;
    Serial.print("THERMISTOR_STDDEV");
    break;
  case THERMISTOR_STDDEV:
    display_mode_ = THERMISTOR_VALUE;
    Serial.print("THERMISTOR_VALUE");
    break;
  case THERMISTOR_VALUE:
    display_mode_ = GRAPHIC;
    Serial.print("GRAPHIC");
    break;
  }
}

/////////////////// ReadyState

  void ReadyState::enter_state() {
    SetStartTime();
    StartTimer();

    StateContext::Get()->sensor()->TurnOff();
    StateContext::Get()->sensor()->TurnOn();
    StateContext::Get()->button()->SetBrightness(0.5);
    StateContext::Get()->button()->TurnOff();
    StateContext::Get()->display()->clear();
    StateContext::Get()->display()->SetBrightness(DISPLAY_NORMAL);
    StateContext::Get()->fan()->TurnOff();
    StateContext::Get()->led()->set_hsv(120, 1.0, 0.1);
  }

  void ReadyState::leave_state() {
    StopTimer();
  }

  mdlib::State* ReadyState::loop() {
    UpdateTimer();
    unsigned long elapsed = millis() - start_time_;
    
    // make the button blink
    float secs =  floor((float)elapsed/1000.0f);
    if( (int)secs % 2) {
      float fraction = (float)elapsed/1000.0f - secs;
      if (fraction < 0.1)
	StateContext::Get()->button()->TurnOn();
      else
	StateContext::Get()->button()->TurnOff();
    }
    else
      StateContext::Get()->button()->TurnOff();

    // count down display
    static unsigned long t = 999999;
    float time_left = timer_.GetSecondsRemaining();
    if ((int)time_left != t) {
      t = time_left;
      StateContext::Get()->display()->SetSeconds(t);
    }
#if 0
    // Make the led Pulse green

    static int rise_count = 0;
    if (StateContext::Get()->sensor()->IsRising()) {
      rise_count++;
      if (rise_count > 5)
	return next_state_;
    }
    else
      rise_count = 0;
#endif 
    return (mdlib::State*)0;
  }

  mdlib::State* ReadyState::handle_event(mdlib::Event e) {
    // advance to sampling if button clicked
    if (e.event_type == mdlib::Event::BUTTON_CLICK) {
      return next_state_;
    }
    else if (e.event_type == mdlib::Event::TIMER_DONE) {
      return timeout_next_state_;
    }

    return (mdlib::State*)0;
  }

  ///////////////////// SamplingState
  static int hue = 0;

  void SamplingState::enter_state() {
    SetStartTime();
    StartTimer();
    hue = 120;
    StateContext::Get()->display()->clear();
    StateContext::Get()->display()->set(1.0);
    StateContext::Get()->display()->SetBrightness(DISPLAY_NORMAL);
    StateContext::Get()->led()->set_hsv(hue, 1.0f,  1.0f);
    StateContext::Get()->sensor()->StartRecording();

    start_sample_ = StateContext::Get()->sensor()->RawAlcoholValue();
  }

  void SamplingState::leave_state() {
    StopTimer();
  }

  mdlib::State* SamplingState::handle_event(mdlib::Event e) {
    static bool fanIsOn = false;

    if (e.event_type == mdlib::Event::TIMER_DONE) {
      return timeout_next_state_;
    }

    if (e.event_type == mdlib::Event::BUTTON_CLICK) {
      return next_state_;
    }

    if (e.event_type == BoozeSensor::BOOZE_MAX_CHANGED) {
      UpdateDisplay();
      StartTimer();
      // If we peg the meter, might as well move along
      if (StateContext::Get()->sensor()->GetMaximum() == 1023)
        return next_state_;
    }
    return 0;
  }

  int hueMap(float x) {
    const int GREEN = 120;
    const int RED = 0;
    
    float in_min = 0.25; // below 25% is green
    float in_max = 0.90; // above 90% is red

    float out_min = GREEN; // 
    float out_max = RED;

    if (x <= in_min)
      return (int)out_min;
    
    if (x >= in_max)
      return (int)out_max;

    return (int)( (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min );
  }

  void SamplingState::UpdateDisplay() {
    int sample = StateContext::Get()->sensor()->GetMaximum();

    float pct = (float)(sample - start_sample_) / (float)(1023 - start_sample_);
    int hue = hueMap(pct);

    StateContext::Get()->led()->set_hsv(hue, 1.0f, 1.0f);
    StateContext::Get()->display()->set((int)(pct * 100.0));
  }

  mdlib::State* SamplingState::loop() {
    UpdateTimer();
    return 0;
  }
  ///////////////////////// PostSampleState

  void PostSampleState::enter_state() {
    Serial.print("ENTERING ");
    Serial.println(name());
    SetStartTime();
    StartTimer();
    StateContext::Get()->display()->SetBrightness(DISPLAY_NORMAL);
    StateContext::Get()->fan()->TurnOn();
    StateContext::Get()->button()->TurnOff();
  }

  void PostSampleState::leave_state() {
    StopTimer();
  }

  mdlib::State* PostSampleState::handle_event(mdlib::Event e) {
    if (e.event_type == mdlib::Event::TIMER_DONE)
      return timeout_next_state_;

    if (e.event_type == mdlib::Event::BUTTON_CLICK)
      return next_state_;

    return 0;
  }

  mdlib::State* PostSampleState::loop() {
    // blink the display
    unsigned long elapsed = millis() - start_time_;
    static bool display_on = false;
    if (elapsed < 2000) {
      bool on = ((elapsed / 200) % 2) == 0;

      if (on != display_on) {
	display_on = on;
	Serial.print(" - ON: ");
	Serial.println(on);
	StateContext::Get()->display()->SetBrightness( on ? DISPLAY_NORMAL : 0.0);
      }
    }
    else if (!display_on) {
      display_on = true;
      Serial.print(" + ON: ");
      Serial.println(display_on);
      StateContext::Get()->display()->SetBrightness(DISPLAY_NORMAL);
    }
    UpdateTimer();
    return 0;
  }

  ///////////////////////// PostSampleState2

  void PostSample2State::enter_state() {
    SetStartTime();
    StartTimer();
    StateContext::Get()->display()->SetBrightness(DISPLAY_NORMAL);
    StateContext::Get()->fan()->TurnOff();
    StateContext::Get()->button()->TurnOff();
  }

  void PostSample2State::leave_state() {
    StopTimer();
  }

  mdlib::State* PostSample2State::handle_event(mdlib::Event e) {
    if (e.event_type == mdlib::Event::TIMER_DONE)
      return timeout_next_state_;

    if (e.event_type == mdlib::Event::BUTTON_CLICK)
      return next_state_;

    return 0;
  }

  ///////////////// PowerSaverState

  void PowerSaverState::enter_state() {
    SetStartTime();
    StartTimer();
    
    StateContext::Get()->fan()->TurnOff();

    StateContext::Get()->led()->set_hsv(120, 1.0, 0.005);
    StateContext::Get()->button()->SetBrightness(0.5);
    StateContext::Get()->display()->SetBrightness(DISPLAY_DIM);
  }

  void PowerSaverState::leave_state() {
    StopTimer();
  }

  mdlib::State* PowerSaverState::handle_event(mdlib::Event e) {
    if (e.event_type == mdlib::Event::BUTTON_CLICK)
      return next_state_;

    if (e.event_type == mdlib::Event::TIMER_DONE)
      return timeout_next_state_;

    return 0;
  }

  // LOOP
  // blink the button and led dimmer & with longer off cycle
  mdlib::State* PowerSaverState::loop() {
    UpdateTimer();
    unsigned long elapsed = millis() - start_time_;
    // make the button blink
    float secs =  floor((float)elapsed/2000.0f);
    if( (int)secs % 2) {
      float fraction = (float)elapsed/2000.0f - secs;
      if (fraction < 0.05)
	StateContext::Get()->button()->TurnOn();
      else
	StateContext::Get()->button()->TurnOff();
    }
    else
      StateContext::Get()->button()->TurnOff();

    // count down display
    static int t = 999999;
    float time_left = timer_.GetSecondsRemaining();

    if ((int)time_left != t) {
      t = time_left;
      StateContext::Get()->display()->SetSeconds(t);
    }
    
    return 0;
  }
///////////////////////// SleepState

  void SleepState::enter_state() {
    StateContext::Get()->led()->TurnOff();
    StateContext::Get()->button()->TurnOff();
    StateContext::Get()->sensor()->TurnOff();
    StateContext::Get()->display()->clear();
    StateContext::Get()->display()->set("    ");
  }

  mdlib::State* SleepState::handle_event(mdlib::Event e) {
    if (e.event_type == mdlib::Event::BUTTON_CLICK ||
	e.event_type == mdlib::Event::BUTTON_LONG_CLICK ||
	e.event_type == mdlib::Event::BUTTON_DOWN	||
	e.event_type == mdlib::Event::BUTTON_UP	)
      return next_state_;

    return 0;
  }

  mdlib::State* SleepState::loop() {
    Narcoleptic.delay(2000);
    return 0;
  }
} // namespace BOM

