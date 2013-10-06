#include <Button.h>
#include <EventQueue.h>
#include <LightedButton.h>
#include <Timers.h>

#include <mdBase.h>
#include <MultiColorLED.h>
#include <Queue.h>
#include <SevenSegmentDisplay.h>
#include <StateMachine.h>


#include <Narcoleptic.h>

// Pin definitions
const int NIGHT_LIGHT = 3;
const int LOW_BATTERY_LED = 4;
const int TOUCH_SENSOR = 7;

const int PHOTOCELL = A0;
const int BATTERY = A1;

const float BATTERY_THRESHOLD_ON = 4.0f;

namespace CampLight {

  // Parts
enum Events {
  TOUCH_SWITCH_ON = 500,
  TOUCH_SWITCH_OFF = 501,
};

class TouchSwitch : public mdlib::BaseIO {
  public:
    TouchSwitch() : cut_off_(5), is_on_(false) {};
    virtual void setup() const;
    virtual void update();
    
    void set_cut_off(int s) { cut_off_ = s; }

  private:
    int cut_off_;
    bool is_on_;
};

class BatteryMonitor {
  public:
    BatteryMonitor() : battery_is_low_(false) {
      set_threshold(BATTERY_THRESHOLD_ON);
    }
    void setup();
    void update();
    void set_threshold(float t);
  private:
    mdlib::DigitalOutput battery_low_light_;
    mdlib::AnalogInput battery_voltage_;
    int threshold_;
    bool battery_is_low_;
};

class Context : public mdlib::StateContext {
  public:
    Context() : brightness_(128) {};
    
    void setup();
    void update();
    
    static Context* Get() { return reinterpret_cast<Context*>(mdlib::State::s_context); };
    
    mdlib::AnalogOutput* night_light() { return &night_light_; }
    mdlib::AnalogInput* photo_cell() { return &photo_cell_; }
    
    void set_brightness(byte b) { brightness_ = b; }
    byte brightness() { return brightness_; }
    
  private:
    TouchSwitch touch_switch_;
    BatteryMonitor battery_monitor_;
    mdlib::AnalogOutput night_light_;
    mdlib::AnalogInput photo_cell_;
    byte brightness_; // 0 - 254
} context_;

class OnState : public mdlib::State {
  public:
    virtual mdlib::State* loop();
    virtual mdlib::State* handle_event(mdlib::Event e);
    virtual void enter_state();
    virtual void leave_state();
    virtual const char* name() const { return "OnState"; }
} ON_STATE;

class OffState : public mdlib::State {
  public:
    virtual mdlib::State* loop();
    virtual mdlib::State* handle_event(mdlib::Event e);
    virtual void enter_state();
    virtual void leave_state();
    virtual const char* name() const { return "OffState"; }
} OFF_STATE;

class AdjustBrightnessState : public mdlib::State {
  public:
    AdjustBrightnessState() : direction_(1) {}
    virtual mdlib::State* loop();
    virtual mdlib::State* handle_event(mdlib::Event e);
    virtual void enter_state();
    virtual void leave_state();
    virtual const char* name() const { return "AdjustBrightnessState"; }
  private:
    const static byte MAX_B = 254;
    const static byte MIN_B = 3;
    const static unsigned long DELAY = 2000;
    int direction_;
    byte initial_brightness_;
    unsigned long elapsed_offset_;
    int seconds_;
} ADJUST_BRIGHTNESS_STATE;

mdlib::State* state_ = (mdlib::State*)0;
unsigned long state_start_millis_ = 0;

void set_state(mdlib::State* state) {
  if (state_) {
    state_->leave_state();
  }
  state_ = state;
  state_start_millis_ = millis();
  state_->enter_state();
#if 0
  Serial.print("set_state(\"");
  Serial.print(state_->name());
  Serial.println("\")");
#endif
}

} // namespace CampLamp

void setup() {
  Serial.begin(9600);

  // Set up state machine
  CampLight::ON_STATE.set_next_state(&CampLight::OFF_STATE);
  CampLight::ON_STATE.set_timeout_next_state(&CampLight::ADJUST_BRIGHTNESS_STATE);
  
  CampLight::OFF_STATE.set_next_state(&CampLight::ON_STATE);
  CampLight::OFF_STATE.set_timeout_next_state(&CampLight::ADJUST_BRIGHTNESS_STATE);
  
  CampLight::ADJUST_BRIGHTNESS_STATE.set_next_state(&CampLight::ON_STATE);
  CampLight::ADJUST_BRIGHTNESS_STATE.set_timeout_next_state(&CampLight::ON_STATE);

  mdlib::State::set_context(&CampLight::context_);  
  CampLight::context_.setup();
  CampLight::set_state(&CampLight::ON_STATE);
}
void loop() {
  CampLight::context_.update();

  // Manage Events
  while (mdlib::CountEvents() > 0) {
    mdlib::Event e = mdlib::HandleEvent();
    //Serial.print("HANDLING EVENT: ");
    //Serial.println(e.event_type);

    mdlib::State* next_state = CampLight::state_->handle_event(e);
    if (next_state)
      CampLight::set_state(next_state);
  }

  mdlib::State* next_state = CampLight::state_->loop();
  if (next_state)
    CampLight::set_state(next_state);
}

namespace CampLight {
///////////////////////////////////////////////////////
// Implementation: TouchSwitch
///////////////////////////////////////////////////////
void TouchSwitch::setup() const {
}
// Based on:
// http://playground.arduino.cc/Code/CapacitiveSensor

// readCapacitivePin
//  Input: Arduino pin number
//  Output: A number, from 0 to 17 expressing
//  how much capacitance is on the pin
//  When you touch the pin, or whatever you have
//  attached to it, the number will get higher
#include "pins_arduino.h" // Arduino pre-1.0 needs this

void TouchSwitch::update() {
  // Variables used to translate from Arduino to AVR pin naming
  volatile uint8_t* port;
  volatile uint8_t* ddr;
  volatile uint8_t* PIN;
  // Here we translate the input pin number from
  //  Arduino pin number to the AVR PORT, PIN, DDR,
  //  and which bit of those registers we care about.
  byte bitmask;
  port = portOutputRegister(digitalPinToPort(pin()));
  ddr = portModeRegister(digitalPinToPort(pin()));
  bitmask = digitalPinToBitMask(pin());
  PIN = portInputRegister(digitalPinToPort(pin()));
  // Discharge the pin first by setting it low and output
  *port &= ~(bitmask);
  *ddr  |= bitmask;
  delay(1);
  // Prevent the timer IRQ from disturbing our measurement
  noInterrupts();
  // Make the pin an input with the internal pull-up on
  *ddr &= ~(bitmask);
  *port |= bitmask;

  // Now see how long the pin to get pulled up. This manual unrolling of the loop
  // decreases the number of hardware cycles between each read of the pin,
  // thus increasing sensitivity.
  uint8_t cycles = 17;
       if (*PIN & bitmask) { cycles =  0;}
  else if (*PIN & bitmask) { cycles =  1;}
  else if (*PIN & bitmask) { cycles =  2;}
  else if (*PIN & bitmask) { cycles =  3;}
  else if (*PIN & bitmask) { cycles =  4;}
  else if (*PIN & bitmask) { cycles =  5;}
  else if (*PIN & bitmask) { cycles =  6;}
  else if (*PIN & bitmask) { cycles =  7;}
  else if (*PIN & bitmask) { cycles =  8;}
  else if (*PIN & bitmask) { cycles =  9;}
  else if (*PIN & bitmask) { cycles = 10;}
  else if (*PIN & bitmask) { cycles = 11;}
  else if (*PIN & bitmask) { cycles = 12;}
  else if (*PIN & bitmask) { cycles = 13;}
  else if (*PIN & bitmask) { cycles = 14;}
  else if (*PIN & bitmask) { cycles = 15;}
  else if (*PIN & bitmask) { cycles = 16;}

  // End of timing-critical section
  interrupts();

  // Discharge the pin again by setting it low and output
  //  It's important to leave the pins low if you want to 
  //  be able to touch more than 1 sensor at a time - if
  //  the sensor is left pulled high, when you touch
  //  two sensors, your body will transfer the charge between
  //  sensors.
  *port &= ~(bitmask);
  *ddr  |= bitmask;

//  Serial.print("* cycles: ");
//  Serial.println(cycles);
  bool new_is_on = cycles > cut_off_;
  
  if (is_on_ != new_is_on) {
    is_on_ = new_is_on;
    mdlib::PostEvent(mdlib::Event((is_on_ ? TOUCH_SWITCH_ON : TOUCH_SWITCH_OFF), (int)this));
  }
}
///////////////////////////////////////////////////////
// Implementation: BatteryMonitor
///////////////////////////////////////////////////////
void BatteryMonitor::setup() {
  battery_low_light_.set_pin(LOW_BATTERY_LED);
  battery_low_light_.setup();

  battery_voltage_.set_pin(BATTERY);
  battery_voltage_.setup();
}

void BatteryMonitor::update() {
  battery_voltage_.update();
  battery_low_light_.update();
  
  int level = battery_voltage_.read();
#if 0
  Serial.print("battery level: ");
  Serial.print(level);
  Serial.print(", threshold: ");
  Serial.println(threshold_);
#endif
  bool new_battery_is_low = level < threshold_;
  
  if (new_battery_is_low != battery_is_low_) {
    battery_is_low_ = new_battery_is_low;
    if (battery_is_low_)
      battery_low_light_.TurnOn();
    else
      battery_low_light_.TurnOff();
  }
}

void BatteryMonitor::set_threshold(float t) {
  threshold_ = (int)((t/5.0f) * 1023.0);
}

///////////////////////////////////////////////////////
// Implementation: Context
///////////////////////////////////////////////////////
void Context::setup() {
  battery_monitor_.setup();
  
  night_light_.set_pin(NIGHT_LIGHT);
  night_light_.setup();

  photo_cell_.set_pin(PHOTOCELL);
  photo_cell_.setup();

  touch_switch_.set_pin(TOUCH_SENSOR);
  touch_switch_.setup();
}

void Context::update() {
  battery_monitor_.update();
  
  photo_cell_.update();
  night_light_.update();
  touch_switch_.update();
}

///////////////////////////////////////////////////////
// Implementation: OnState
///////////////////////////////////////////////////////
mdlib::State* OnState::loop() {
  const unsigned long ON_DELAY = 1000;
  const int OFF_THRESHOLD = 190;
  unsigned long elapsed = millis() - start_time_;
  
  if (elapsed > ON_DELAY) {
    int light = Context::Get()->photo_cell()->read();
    if (light > OFF_THRESHOLD) {
      return next_state_;
    }
  }
  
  return (mdlib::State*)0; 
}

mdlib::State* OnState::handle_event(mdlib::Event e) {
  if (e.event_type == TOUCH_SWITCH_ON) {
    return timeout_next_state_;
  }
  return (mdlib::State*)0; 
}

void OnState::enter_state() {
  SetStartTime();
  Context::Get()->night_light()->setLevel(Context::Get()->brightness());
}

void OnState::leave_state() {
}

///////////////////////////////////////////////////////
// Implementation: OffState
///////////////////////////////////////////////////////
mdlib::State* OffState::loop() {
  // TODO rewrite using narcoleptic delay
  const unsigned long OFF_DELAY = 1000;
  const int ON_THRESHOLD = 130;
  unsigned long elapsed = millis() - start_time_;
  
  if (elapsed > OFF_DELAY) {
    int light = Context::Get()->photo_cell()->read();
    if (light < ON_THRESHOLD) {
      return next_state_;
    }
  }
  
  return (mdlib::State*)0; 
}

mdlib::State* OffState::handle_event(mdlib::Event e) {
  if (e.event_type == TOUCH_SWITCH_ON) {
    return timeout_next_state_;
  }
  return (mdlib::State*)0; 
}

void OffState::enter_state() {
  SetStartTime();
  Context::Get()->night_light()->setLevel(0.0f);
}

void OffState::leave_state() {}


///////////////////////////////////////////////////////
// Implementation: AdjustBrightnessState
///////////////////////////////////////////////////////
mdlib::State* AdjustBrightnessState::loop() {
  unsigned long elapsed = millis() - start_time_;
  elapsed += elapsed_offset_;
  int n = elapsed / DELAY;
  float pct = (float)elapsed / (float)DELAY;
  pct = pct - (int)pct;
  
  if (seconds_ != n) {
    seconds_ = n;
    direction_ = direction_ * -1;
    Serial.println(" ** change direction **");
  }
  // calculate the new b based on elapsed time
  if (direction_ < 0)
    pct = 1.0f - pct;

#if 0
  Serial.print(" - seconds: ");
  Serial.print(seconds_);
  Serial.print(", elapsed: ");
  Serial.print(elapsed);
  Serial.print(", pct: ");
  Serial.print(pct, 2);
#endif

  int b = (byte)((float)MIN_B + pct * (float)(MAX_B - MIN_B));
  if (b > 254)
    b -= 254;
  if (b < 0)
    b += 254;
#if 0
  Serial.print(", b: ");
  Serial.print(b);
  Serial.print(" -> ");
  Serial.println(initial_brightness_);
#endif
  Context::Get()->set_brightness((byte)b);
  Context::Get()->night_light()->setLevel((byte)b);
  return (mdlib::State*)0; 
}

mdlib::State* AdjustBrightnessState::handle_event(mdlib::Event e) {
  if (e.event_type == TOUCH_SWITCH_OFF) {
    return next_state_;
  }
  return (mdlib::State*)0; 
}
void AdjustBrightnessState::enter_state() {
  SetStartTime();
  initial_brightness_ = Context::Get()->brightness();
  Context::Get()->night_light()->setLevel(initial_brightness_);
  // calculate the 'elapsed time offset' based on direction_ and initial_brightness_
  // it takes DELAY to get from MIN_B to MAX_B (or back)
  int off = 0;
  float pct = (float)(initial_brightness_ - MIN_B)/(float)(MAX_B - MIN_B);
  if (direction_ < 0)
    pct = 1.0f - pct;
    
  elapsed_offset_ = (int)(pct * (float)DELAY);
  seconds_ = 0;
}
void AdjustBrightnessState::leave_state() {
}

} // namespace CampLamp
