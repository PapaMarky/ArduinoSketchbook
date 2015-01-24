#include <SoftwareSerial.h>

/**
 * FinishLineControlPanel for Shape Security scooter races.
 */
 
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
 
 // Speaker (move to DigitalClock?)
 
 // Serial to Big7Segment display - INPUT/OUTPUT - TX/RX
 
 // Data logger - OUTPUT (Serial, needs TX/RX, only uses TX)


 /** Add another processor for the speaker? **/

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
  * D2     - DigitalDixplayTx  
  * D3 *   - DigitalDixplayRx
  * D4  
  *
  * D5 *   - LaserLED
  * D6 *    
  * D7     - LidTx
  * D8     - LidRx
  *
  * D9 *   - GreenLED
  * D10*   - YellowLED
  * D11*   - RedLED
  * D12    - Tree Tx
  * D13    - Tree Rx
  */

const int laserLedPin = 5;
const int photoCellPin = A0;

/*
const int diskReset = 2;
const int diskTx = 6;
const int diskRx = 7;
*/
const int displayTx = 7;  // This one connects to LCD
const int displayRx = 8; // not connected

 
#include "flcp_statemachine.h"
#include "Component.h"
#include "Context.h"

#include "LaserAssembly.h"
 
LaserAssembly laserAssembly(laserLedPin, photoCellPin);
int g_laser_id = -1;

Context baseContext;
StateMachine g_stateMachine;
SoftwareSerial LcdSerial(displayRx, displayTx);

void setup() {
  Serial.begin(9600);
  Serial.println("Here we go");
  
  LcdSerial.begin(9600);

  g_laser_id = baseContext.addComponent(&laserAssembly);
  baseContext.setup();
  g_stateMachine.setup();
}

void loop() {
  baseContext.loop();
  g_stateMachine.loop();
  
  if (Serial.available() > 0) {
    int outgoing = Serial.read();
    Serial.print("From USB: "); Serial.println(outgoing);
    LcdSerial.write(outgoing);
  }
 
  if(LcdSerial.available() > 0) {
    int incomming = LcdSerial.read();
    Serial.print("From LCD: "); Serial.println(incomming);
   }
}

