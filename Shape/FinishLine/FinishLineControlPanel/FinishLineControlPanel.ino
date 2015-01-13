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
  * Pin List - LID
  *
  * A0      - 5ws - up
  * A1      - 5ws - down
  * A2      - 5ws - left
  * A3      - 5ws - right
  * A4(SDA) - 5ws - select
  * A5(SCL) - LCD_RS
  * 
  * D0(RX)  - BaseTx
  * D1(TX)  - BaseRx
  * D2      - diskReset
  * D3 *    - diskTx
  * D4      - diskRx
  *
  * D5 *
  * D6 *
  * D7      - LCD_EN
  * D8      - LCD_DB4
  *
  * D9 *
  * D10*
  * D11*    - LCD_DB5
  * D12     - LCD_DB6
  * D13     - LCD_DB7 
  */

 /* 
  * Pin List - BASE
  *
  * A0     - PhotoCell
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
  * D5 *
  * D6 *   - LaserLED 
  * D7     - LidTx
  * D8     - LidRx
  *
  * D9 *   - GreenLED
  * D10*   - YellowLED
  * D11*   - RedLED
  * D12    - Tree Tx
  * D13    - Tree Rx
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

StateMachine g_stateMachine;


SoftwareSerial display(displayRx, displayTx);
LCD lcd(&display);

 void setup() {
   Serial.begin(9600);
   Serial.println("Here we go");
   
   //devices[0] = &laserDetector;
   //devices[1] = &goButton;
   
   //data.setup();
   lcd.setup();
   
   g_stateMachine.setup();
 }
 
 
 void loop() {
   
   for(int i = 0; i < nDevices; i++) {
     //devices[i]->loop();
   }
   g_stateMachine.loop();
 }
 
