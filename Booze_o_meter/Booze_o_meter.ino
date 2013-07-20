#include <Narcoleptic.h>

// Copyright (c) 2013 Mark Dyer. All rights reserved.

#include <SoftwareSerial.h>
#include <SevenSegmentDisplay.h>
#include <Booze_O_Meter.h>
#include <StateContext.h>
#include <LightedButton.h>

/**
  * ARDUINO PIN USAGE
  * (Note: pins 2 & 3 can be used for external interrupts)
  * http://www.arduino.cc/en/Guide/Board?from=Tutorial.ArduinoBoard
  * - D0  : 
  * - D1  : 
  * - D2  : MAIN_BUTTON_PIN
  * + D3  : UP_BUTTON_PIN
  * - D4  : DOWN_BUTTON_PIN
  * + D5  : DISPLAY_RX_PIN
  * + D6  : DISPLAY_TX_PIN
  * - D7  : SENSOR_CONTROL_PIN // sensor on/off control
  * - D8  : JUMPER_PIN // standalone mode
  * + D9  :
  * + D10 : 
  * + D11 :
  * - D12 : 
  * - D13 : FAN_PIN // fan on/off control //XXX MOVE FAN TO D12
  *
  * - A1  :
  * - A2  : 
  * - A3  : xxx
  * - A4  : SENSOR_TEMPERATURE_PIN
  * - A5  : SENSOR_DATA_PIN
  */

const int JUMPER_PIN      = 2;
const int FAN_PIN         = A5;

const int DISPLAY_RX_PIN  = 2; // don't need to wire this
const int DISPLAY_TX_PIN  = 3; // connect to RX on seven segment display

const int MAIN_BUTTON_PIN     = 8;
const int MAIN_BUTTON_LED_PIN = 6;

const int SENSOR_CONTROL_PIN = 12;
const int SENSOR_DATA_PIN = A1;
const int SENSOR_TEMPERATURE_PIN = A2;

const int RED_PIN = 11;
const int GREEN_PIN = 10;
const int BLUE_PIN = 9;

mdlib::DigitalOutput fan;
mdlib::SevenSegmentDisplay display(DISPLAY_RX_PIN, DISPLAY_TX_PIN);
mdlib::MultiColorLED rgb_led;
BOM::BoozeSensor sensor;

BOM::StateContext context;
BOM::Booze_O_Meter bom;
mdlib::LightedButton main_button;

void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0));
  fan.set_pin(FAN_PIN);
  rgb_led.set_pins(RED_PIN, GREEN_PIN, BLUE_PIN);
  sensor.set_pins(SENSOR_CONTROL_PIN, SENSOR_DATA_PIN, SENSOR_TEMPERATURE_PIN);
  main_button.set_pins(MAIN_BUTTON_PIN, MAIN_BUTTON_LED_PIN);
  
  context.set_display(&display);
  context.set_fan(&fan);
  context.set_rgb_led(&rgb_led);
  context.set_sensor(&sensor);
  context.set_button(&main_button);
  
  bom.set_context(&context);

  bom.setup();
}

void loop() {
  bom.update();
}


