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

 // Joystick (?)

 /* 
  * Pin List
  *
  * A0 - PhotoCell
  * A1
  * A2
  * A3
  * A4
  * A5
  * 
  * D0
  * D1
  * D2  - diskReset
  * D3
  * D4
  * D5
  * D6  - diskTx
  * D7  - diskRx
  * D8
  * D9  - displayTx
  * D10 - displayRx
  * D11
  * D12
  * D13
  */
  
const int photoCellPin = A0;
const int diskReset = 2;
const int diskTx = 6;
const int diskRx = 7;
const int displayTx = 9;  // This one connects to LCD
const int displayRx = 10; // not connected
 
 #include "flcp_statemachine.h"
 #include "input_device.h"

 #include "DataBase.h"
 #include "LCD.h"
 
 const int nDevices = 2;
 InputDevice* devices[nDevices];

 LaserDetector laserDetector;
 GoButton goButton;

SoftwareSerial disk(diskRx, diskTx);
DataBase data(&disk, diskReset);

SoftwareSerial display(displayRx, displayTx);
LCD lcd(&display);

 void setup() {
   devices[0] = &laserDetector;
   devices[1] = &goButton;
   
   data.setup();
   lcd.setup();
   
   lcd.clear();
 }
 
 
 void loop() {
   for(int i = 0; i < nDevices; i++) {
     devices[i]->loop();
   }
   g_stateMachine.loop();
 }
 
