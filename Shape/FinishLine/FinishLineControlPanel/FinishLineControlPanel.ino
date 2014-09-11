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
 #include "flcp_statemachine.h"
 #include "input_device.h"
 
 void setup() {
 }
 
 
 void loop() {
   g_stateMachine.loop();
 }
 
