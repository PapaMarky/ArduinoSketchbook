// -*-c++-*-
// Copyright 2015, Mark Dyer
/**
 * LcdProcessorAssembly - the processor stuck to the LCD in the lid
 *
 * 
 */
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <mdComm.h>

#include "scooter_data.h"

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
  * D2      - diskTx (RX on this side)
  * D3 *    - RedLED
  * D4      - diskRx (TX on this side)
  *
  * D5 *    - GreenLED
  * D6 *    - GoButtonLED
  * D7      - LCD_EN
  * D8      - LCD_DB4
  *
  * D9 *    - YellowLED
  * D10*    - LCD_DB5
  * D11*    - LCD_DB6
  * D12     - LCD_DB7
  * D13     - diskReset (GRN)
  */

#define LCD_RS A5
#define LCD_EN 7
#define LCD_DB4 8
#define LCD_DB5 10
#define LCD_DB6 11
#define LCD_DB7 12
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_DB4, LCD_DB5, LCD_DB6, LCD_DB7);

#define DISK_RESET 13
#define DISK_TX 2
#define DISK_RX 4
SoftwareSerial disk(DISK_RX, DISK_TX);
ScooterData database(DISK_RESET, &disk);

#define SW5_SELECT A0
#define SW5_RIGHT  A1
#define SW5_DOWN   A2
#define SW5_LEFT   A3
#define SW5_UP     A4

#define GO_BUTTON_LED 6

#define RED_LED 3
#define YELLOW_LED 9
#define GREEN_LED 5

mdComm::message m;
const int MSG_BUFFER_SIZE = 64;
char buffer[MSG_BUFFER_SIZE];
char buffer2[MSG_BUFFER_SIZE];

uint16_t base_wait = 0;

void setup()
{
  Serial.begin(9600);
  
  pinMode(SW5_SELECT, INPUT);
  pinMode(SW5_UP, INPUT);
  pinMode(SW5_DOWN, INPUT);
  pinMode(SW5_LEFT, INPUT);
  pinMode(SW5_RIGHT, INPUT);
  
  pinMode(GO_BUTTON_LED, OUTPUT);
  
  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  
  for(int i = 0; i < MSG_BUFFER_SIZE; i++)
    buffer[i] = 0xcc;
 
  m.data = (byte*)buffer;
  m.data_size = MSG_BUFFER_SIZE;
  strcpy(buffer, "oink");

  lcd.begin(20,4);
  lcd.home();
  //lcd.print("01234567890123456789")
  lcd.print("   Shape Scooter");
  lcd.setCursor(0, 1);
  lcd.print("    Finish Line");
  
  lcd.setCursor(0, 3);
  lcd.print("Base: Waiting...");

  database.begin();
  database.start_command_mode();
  lcd.setCursor(18, 3);
  lcd.print("d");
  
  analogWrite(RED_LED, 0);
  analogWrite(YELLOW_LED, 0);
  analogWrite(GREEN_LED, 0);
}

bool waiting_for_base = true;
bool waiting_for_disk = true;

void base_wait_loop() {
  uint16_t elapsed = millis() - base_wait;
  if (Serial.available()) {
    if(Serial.findUntil("base:start", "\n")) {
      Serial.println("lcd:ack start");
      waiting_for_base = false;
      lcd.setCursor(0,3);
      lcd.print("Base: Connected");
    }
  }
  else if (elapsed >= 5000) {
    // ping the base unit
  }
}

void data_wait_loop() {
  database.loop();
  if (database.is_ready()){
    waiting_for_disk = false;
    lcd.setCursor(18, 3);
    lcd.print("d");
  }
}

bool go = false;

void go_wait_loop() {
  if (Serial.available()) {
    delay(10);
    if(Serial.readBytesUntil('\n', buffer, MSG_BUFFER_SIZE) > 0) {
      if (strcmp(buffer, "base:go:up") == 0) {
        lcd.setCursor(12, 2);
        lcd.print("   ");
      }
      else if (strcmp(buffer, "base:go:down") == 0) {
        lcd.setCursor(12, 2);
        lcd.print("GO!");
      }
    }
  }
}

int bstate = -1;
bool wasPressed = false;

void setLEDs(byte red, byte yellow, byte green) {
  analogWrite(RED_LED, red);
  analogWrite(YELLOW_LED, yellow);
  analogWrite(GREEN_LED, green);
}

void loop()
{
  int bs = -5;
  if(digitalRead(SW5_SELECT) == HIGH) {
    bs = SW5_SELECT;
  }
  else if (digitalRead(SW5_UP) == HIGH) {
    bs = SW5_UP;
  }
  else if (digitalRead(SW5_DOWN) == HIGH) {
    bs = SW5_DOWN;
  }
  else if (digitalRead(SW5_LEFT) == HIGH) {
    bs = SW5_LEFT;
  }
  else if (digitalRead(SW5_RIGHT) == HIGH) {
    bs = SW5_RIGHT;
  }
  bool pressed = true;
  if (bstate != bs) {
    lcd.setCursor(0,2);
    switch(bs) {
     case SW5_SELECT:
      lcd.print("SW5_SELECT");
      setLEDs(255, 255, 255);
      break;
     case SW5_UP:
      lcd.print("SW5_UP    ");
      setLEDs(255, 0, 0);
      break;
     case SW5_DOWN:
      lcd.print("SW5_DOWN  ");
      setLEDs(0, 0, 255);
      break;
     case SW5_LEFT:
      lcd.print("SW5_LEFT  ");
      setLEDs(128, 255, 0);
      break;
     case SW5_RIGHT:
      lcd.print("SW5_RIGHT ");
      setLEDs(0, 255, 128);
      break;
     default:
      pressed = false;
      lcd.print("          ");
      setLEDs(0,0,0);
      break;
    }
  }
  
  if (wasPressed != pressed) {
    wasPressed = pressed;
    if (pressed){
      digitalWrite(GO_BUTTON_LED, HIGH);
    } else {
      digitalWrite(GO_BUTTON_LED, LOW);
    }
  }  
  if(waiting_for_base) {
    base_wait_loop();
  }

  if(waiting_for_disk) {
    data_wait_loop();
  }
}

 
