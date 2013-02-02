#include <SoftwareSerial.h>
#include <Booze_O_Meter.h>

const int JUMPER_PIN      = 8;
const int FAN_PIN         = 10;

const int DISPLAY_RX_PIN  = 5; // don't need to wire this
const int DISPLAY_TX_PIN  = 6; // connect to RX on seven segment display

const int MAIN_BUTTON_PIN = 2;
const int UP_BUTTON_PIN   = 3;
const int DOWN_BUTTON_PIN = 4;

const int SENSOR_CONTROL_PIN = 7;
const int SENSOR_DATA_PIN = A2;
const int SENSOR_TEMPERATURE_PIN = A3;

Booze_O_Meter bom(FAN_PIN, 
                  JUMPER_PIN, 
                  DISPLAY_RX_PIN, DISPLAY_TX_PIN, 
                  MAIN_BUTTON_PIN, UP_BUTTON_PIN, DOWN_BUTTON_PIN,
                  SENSOR_CONTROL_PIN, SENSOR_DATA_PIN, SENSOR_TEMPERATURE_PIN);

void setup() {
  Serial.begin(9600);
  
  bom.setup();
  bom.loop(); // run the POWER_ON state's loop
  
  Serial.print("Standalone: ");
  Serial.println(bom.isStandalone() ? "TRUE" : "FALSE");
}

void loop() {
  bom.loop();
}

