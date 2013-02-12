#include <SoftwareSerial.h>
#include <Booze_O_Meter.h>
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
  * + D10 : FAN_PIN // fan on/off control //XXX MOVE FAN TO D12
  * + D11 :
  * - D12 : 
  * - D13 : 
  *
  * - A1  :
  * - A2  : SENSOR_DATA_PIN
  * - A3  : SENSOR_TEMPERATURE_PIN
  * - A4  : 
  * - A5  : 
  */

const int JUMPER_PIN      = 8;
const int FAN_PIN         = 10;

const int DISPLAY_RX_PIN  = 5; // don't need to wire this
const int DISPLAY_TX_PIN  = 6; // connect to RX on seven segment display

const int MAIN_BUTTON_PIN = 2;
const int UP_BUTTON_PIN   = 3;
const int DOWN_BUTTON_PIN = 4;

const int SENSOR_CONTROL_PIN = 7;
const int SENSOR_DATA_PIN = A0;
const int SENSOR_TEMPERATURE_PIN = A1;

Booze_O_Meter bom(DISPLAY_RX_PIN, DISPLAY_TX_PIN);

void setup() {
  Serial.begin(9600);
  
  bom.set_fan_pin(FAN_PIN);
  bom.set_main_button_pin(MAIN_BUTTON_PIN);
  bom.set_up_down_button_pins(UP_BUTTON_PIN, DOWN_BUTTON_PIN);
  bom.set_booze_sensor_pins(SENSOR_CONTROL_PIN, SENSOR_DATA_PIN, SENSOR_TEMPERATURE_PIN);
  
  bom.setup();
  bom.loop(); // run the POWER_ON state's loop
  
  Serial.print("Standalone: ");
  Serial.println(bom.isStandalone() ? "TRUE" : "FALSE");
}

void loop() {
  bom.loop();
}

