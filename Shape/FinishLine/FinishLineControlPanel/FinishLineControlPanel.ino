#include <mdComm.h>

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
  * D4     - GoButton
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
  Serial.begin(19200);
  Serial.println("Starting Serial");
  delay(500);
  LcdSerial.begin(9600);

  //g_laser_id = baseContext.addComponent(&laserAssembly);
  //baseContext.setup();
  //g_stateMachine.setup();
  
  mdComm::startDebug();
  Serial.println("let's go");
}

bool lcd_connected = false;

const int MSG_BUFFER_SIZE = 64;
mdComm::message m;
char buffer[MSG_BUFFER_SIZE];
bool stop = false;
/*
void SendHello() {
  m.header.src = mdComm::BASE;
  m.header.dst = mdComm::LCD;
  m.header.msg_id = mdComm::MSG_HELLO;
  m.header.msg_len = 0;
  m.data = buffer;
  m.data_size = MSG_BUFFER_SIZE;
  mdComm::comm_result r = mdComm::write_message(m, LcdSerial);
  Serial.print("Sent HELLO, result: "); Serial.println(r);
  lcd_connected = false;
}
*/
char cbuffer[MSG_BUFFER_SIZE];

void print(Stream& serial, char* string, char* header) {
  if (header != 0) {
    Serial.print(header); Serial.print(": ");
  }
  serial.println(string);
}
int dumpStream(Stream& serial, char* header = 0) {
  int i = 0;
  if (serial.available() > 0) {
    delay(10);
    sprintf(cbuffer, "Got %d bytes", serial.available());
    print(Serial, cbuffer, header);
    while(serial.available() > 0) {
      int b = serial.read();
      buffer[i] = (b & 0xff);
      i++;
      if (b >= ' ' && b <= '~')
        sprintf(cbuffer, "0x%02x '%c'", b, b);
      else
        sprintf(cbuffer, "0x%02x", b);
        
      print(Serial, cbuffer, header);
    }
    buffer[i] = '\0';
    i++;
    
    sprintf(cbuffer, "'%s' + null: %d bytes", buffer, i);
    print(Serial, cbuffer, header);
    print(Serial, "------------------------------", header);
  }
  return i;
}

bool waiting = false;
uint16_t lcd_start_wait = 1000;

void loop() {
  //baseContext.loop();
  //g_stateMachine.loop();

  if (! lcd_connected) {
    uint15_t now = millis();
    
    if (LcdSerial.available()) {
      if (LcdSerial.findUntil("lcd:ack start", "\n")) {
        lcd_connected = true;
      }
    }
    if (! lcd_connected && now - lcd_start_wait >= 500) {
      lcd_start_wait = now;
      LcdSerial.println("base:start");
    }
  }
  
  int i = 0;
  if ((i = dumpStream(Serial, "USB")) > 0) {
    //LcdSerial.println("hello");
    mdComm::writeMessage(buffer, i, LcdSerial);
    waiting = true;
  }
  
  cbuffer[0] = '\0';
  if (waiting && mdComm::checkForMessage(LcdSerial, buffer, MSG_BUFFER_SIZE)) {
    mdComm::gotMessage();
    waiting = false;
    Serial.println("GOT MESSAGE: ");
    Serial.println(buffer);

    //cbuffer[5] = '\0';
    //Serial.println("----------------------------");  
  }
}

