// Copyright (c) 2013 Mark Dyer. All rights reserved.

#include <SoftwareSerial.h>
#include <Booze_O_Meter.h>
#include <StateContext.h>

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

const int JUMPER_PIN      = 8;
const int FAN_PIN         = 13;

const int DISPLAY_RX_PIN  = 5; // don't need to wire this
const int DISPLAY_TX_PIN  = 6; // connect to RX on seven segment display

const int MAIN_BUTTON_PIN = 2;
const int UP_BUTTON_PIN   = 3;
const int DOWN_BUTTON_PIN = 4;

const int SENSOR_CONTROL_PIN = 7;
const int SENSOR_DATA_PIN = A5;
const int SENSOR_TEMPERATURE_PIN = A4;

const int RED_PIN = 11;
const int GREEN_PIN = 10;
const int BLUE_PIN = 9;

mdlib::DigitalOutput fan;
SoftwareSerial display(DISPLAY_RX_PIN, DISPLAY_TX_PIN);
mdlib::MultiColorLED rgb_led;
BOM::BoozeSensor sensor;

BOM::StateContext context;
BOM::Booze_O_Meter bom;
mdlib::Button main_button;

void setup() {
  Serial.begin(9600);
  
  fan.set_pin(FAN_PIN);
  rgb_led.set_pins(RED_PIN, GREEN_PIN, BLUE_PIN);
  sensor.set_pins(SENSOR_CONTROL_PIN, SENSOR_DATA_PIN, SENSOR_TEMPERATURE_PIN);
  
  context.set_fan(&fan);
  context.set_display(&display);
  context.set_rgb_led(&rgb_led);
  
  main_button.set_pin(MAIN_BUTTON_PIN);
  bom.set_context(&context);
  
  bom.set_up_down_button_pins(UP_BUTTON_PIN, DOWN_BUTTON_PIN);
  
  bom.setup();
  context.fan()->setup();
  context.sensor()->setup();
  context.display()->begin(9600);
  delay(10);
  context.display()->write('v'); // 0x76); // clear
  context.display()->write('w');
  context.display()->write((uint8_t)0x00);

  context.led()->setup();
  context.led()->set_color(mdlib::BLACK);
 
  bom.loop(); // run the POWER_ON state's loop
  
  Serial.print("Standalone: ");
  Serial.println(bom.isStandalone() ? "TRUE" : "FALSE");
}

void loop() {
  bom.loop();
}


