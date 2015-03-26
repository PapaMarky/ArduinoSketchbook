#include <scooter_base.h>

// -*-c++-*-
// Copyright 2015 Mark Dyer
/**
 * FinishLineControlPanel for Shape Security scooter races.
 */
#include <SoftwareSerial.h>
 
 /*
  * State diagram: https://docs.google.com/drawings/d/1iWZqNdk4uv99Wvf8hHpwqDw-FqMTovA7hFz8g3So90I
  */
#include <Arduino.h>
 
 // Laser sensor (photocell) - INPUT, one analog pin
 // - laser is stand alone, battery operated device
 // LED to indicate Laser detected - OUTPUT
 // Serial LCD Display (Serial, needs TX/RX only uses TX)
 // 'go' button - INPUT, one pin
 // Christmas Tree Display (8 bit shift register)
 // Serial to Big7Segment display - INPUT/OUTPUT - TX/RX
 // Data logger - OUTPUT (Serial, needs TX/RX, only uses TX)


 /** Add another processor for the speaker **/

 /* 
  * Pin List - BASE
  *
  * A0     - Laser PhotoCell
  * A1
  * A2
  * A3
  * A4(SDA)
  * A5(SCL)
  * 
  * D0(RX) - USB-TX
  * D1(TX) - USB-RX
  * D2     - GoButton  
  * D3 *   - DigitalDixplayTx
  * D4     - DigitalDixplayRx
  *
  * D5 *   - LaserLED
  * D6 *    
  * D7     - LidTx
  * D8     - LidRx
  *
  * D9 *   - 
  * D10*   - 
  * D11*   - 
  * D12    - Tree Tx
  * D13    - Tree Rx
  */
#include <scooter_common.h>
#include "LcdComponent.h"

#define GO_BUTTON_PIN 2

const int laserLedPin = 5;
const int photoCellPin = A0;

const int displayTx = 7;  // This one connects to LCD
const int displayRx = 8; // not connected
 
#include "flcp_statemachine.h"
#include "LcdComponent.h"

#include "LaserAssembly.h"
#include "globals.h"

debug_base DEBUGGER;

StateMachine _stateMachine;

LaserAssembly laserAssembly;
ButtonComponent go_button(GO_BUTTON_PIN, GO_BUTTON, &_stateMachine);

SoftwareSerial LcdSerial(displayRx, displayTx);
LcdComponent Lcd;

const int MSG_BUFFER_SIZE = 64;
char buffer[MSG_BUFFER_SIZE];

void setup() {
  Serial.begin(9600);
  gdbg = &DEBUGGER;
  
  LcdSerial.begin(BASE_LID_BAUD);
  Lcd.initialize(&LcdSerial);
  Serial.println("Pretest");
  Lcd.TEST();
  //Lcd.become_master();

  pinMode(GO_BUTTON, INPUT);

  g_stateMachine = &_stateMachine;

  laserAssembly.initialize(laserLedPin, photoCellPin);
  g_laser = &laserAssembly;
  g_laser->setup();

  g_lcd = &Lcd;
  g_lcd->setup();

  g_go_button = &go_button;
  g_go_button->setup();
 
  g_stateMachine->setup();
  
  //gdbg->DEBUG("let's go");
  
}

int lastGoButtonState = -1;
int goButtonState = -1;
int lastGoDebounceTime = 0;
uint32_t debounceDelay = 10;

void checkGoButton() {
  int gb = digitalRead(GO_BUTTON);
  
  uint32_t now = millis();
  if (gb != lastGoButtonState) {
    lastGoDebounceTime = now;
  }
  else if ((now - lastGoDebounceTime) > debounceDelay) {
    if (gb != goButtonState) {
      goButtonState = gb;
      Serial.print("Go Button: ");
      Serial.println(goButtonState == HIGH ? "DOWN" : "UP");
      // Do something
      #if 0
      if (Lcd.isConnected()) {
        Serial.println(" - Send message to LCD");
        sprintf(buffer, "base:go:%s", goButtonState == HIGH ? "down" : "up");
        LcdSerial.println(buffer);
      }
      #endif
    }
  }
  lastGoButtonState = gb;
}

void loop() {
  uint32_t now = millis();
  g_laser->loop(now);
  g_go_button->loop(now);
  g_lcd->loop(now);
  g_stateMachine->loop();
}

