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

const int PHOTOCELL = A0;
const int BATTERY = A2;

const int TOUCH_SENSOR = 7;

const float BATTERY_THRESHOLD_ON = 3.5;

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
    Context() {};
    
    void setup();
    void update();
    
    static Context* Get() { return reinterpret_cast<Context*>(mdlib::State::s_context); };
    
  private:
    TouchSwitch touch_switch;
    BatteryMonitor battery_monitor;
    mdlib::AnalogOutput night_light;
    mdlib::AnalogInput photo_cell;    
} context_;

class OnState : public mdlib::State {
  public:
  virtual mdlib::State* loop() { return (mdlib::State*)0; }
  virtual mdlib::State* handle_event(mdlib::Event e) { return (mdlib::State*)0; }
  virtual const char* name() const { return "OnState"; }
} ON_STATE;

class OffState : public mdlib::State {
  virtual mdlib::State* loop() { return (mdlib::State*)0; }
  virtual mdlib::State* handle_event(mdlib::Event e) { return (mdlib::State*)0; }
  virtual const char* name() const { return "OffState"; }
} OFF_STATE;

class AdjustBrightnessState : public mdlib::State {
  virtual mdlib::State* loop() { return (mdlib::State*)0; }
  virtual mdlib::State* handle_event(mdlib::Event e) { return (mdlib::State*)0; }
  virtual const char* name() const { return "AdjustBrightnessState"; }
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

uint8_t readCapacitivePin(int pinToMeasure);

void setup() {
  Serial.begin(9600);

  // Set up state machine
  CampLight::ON_STATE.set_next_state(&CampLight::OFF_STATE);
  CampLight::ON_STATE.set_timeout_next_state(&CampLight::ADJUST_BRIGHTNESS_STATE);
  
  CampLight::OFF_STATE.set_next_state(&CampLight::ON_STATE);
  CampLight::OFF_STATE.set_timeout_next_state(&CampLight::ADJUST_BRIGHTNESS_STATE);
  
  CampLight::ADJUST_BRIGHTNESS_STATE.set_next_state(&CampLight::ON_STATE);

  mdlib::State::set_context(&CampLight::context_);  
  CampLight::context_.setup();
  CampLight::set_state(&CampLight::ON_STATE);
}

static float brightness = 255;
const int off_threshhold = 190;
const int on_threshhold = 130;
static bool battery_is_low = false;

static bool switch_on = false;
void loop() {
  CampLight::context_.update();

  // Manage Events
  while (mdlib::CountEvents() > 0) {
    //Serial.println("HANDLING EVENT");
    mdlib::Event e = mdlib::HandleEvent();

    mdlib::State* next_state = CampLight::state_->handle_event(e);
    if (next_state)
      CampLight::set_state(next_state);
  }

  mdlib::State* next_state = CampLight::state_->loop();
  if (next_state)
    CampLight::set_state(next_state);
    
#if 0  
  static bool on = false;
  int light = analogRead(PHOTOCELL);
  //Serial.print("light: ");
  //Serial.println(light);
  //int d = 1024 - analogRead(DIMMER);
  //float dimmer = 1.0;(float)d /1023.0;
  float battery = 5.0 * (float)analogRead(BATTERY)/1024.0;
  uint8_t cap = readCapacitivePin(TOUCH_SENSOR);
  bool swtch = cap > 5; //check_switch();
  Serial.print("CAP: ");
  Serial.println(cap);
  //Serial.print("battery level: ");
  //Serial.println(battery);
  //battery_is_low = swtch;
  battery = swtch ? 0 : BATTERY_THRESHOLD_ON;
  if (!battery_is_low && battery < BATTERY_THRESHOLD_ON) {
    digitalWrite(LOW_BATTERY_LED, HIGH);
    battery_is_low = true;
  } else if (battery >= BATTERY_THRESHOLD_ON) {
    battery_is_low = false;
    digitalWrite(LOW_BATTERY_LED, LOW);
  }
  
  on = (light < (on ? off_threshhold : on_threshhold));
  int brightness = (int)max(255.0, 1.0);
  int b = on ? brightness : 0;
  //Serial.println(brightness);
  static int last_b = 0;
  if (b != last_b) {
    last_b = b;
    analogWrite(NIGHT_LIGHT, b);
  }

  if (on)
    delay(10);
  else
    delay(1000); // Narcoleptic.delay(1000);
#endif // 0
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
  
  bool new_battery_is_low = level < threshold_;
  
  if (new_battery_is_low != battery_is_low_) {
    battery_is_low_ = new_battery_is_low;
    if (battery_is_low)
      battery_low_light_.TurnOn();
    else
      battery_low_light_.TurnOff();
  }
}

void BatteryMonitor::set_threshold(float t) {
  threshold_ = (int)(t * 1023.0);
}

///////////////////////////////////////////////////////
// Implementation: Context
///////////////////////////////////////////////////////
void Context::setup() {
  battery_monitor.setup();
  
  night_light.set_pin(NIGHT_LIGHT);
  night_light.setup();

  photo_cell.set_pin(PHOTOCELL);
  photo_cell.setup();

  touch_switch.set_pin(TOUCH_SENSOR);
  touch_switch.setup();
}

void Context::update() {
  battery_monitor.update();
  
  photo_cell.update();
  night_light.update();
  touch_switch.update();
}
} // namespace CampLight
