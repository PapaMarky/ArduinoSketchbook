#include <LiquidCrystal.h>

#include <mdComm.h>

// Copyright 2015, Mark Dyer
/**
 * LcdProcessorAssembly - the processor stuck to the LCD in the lid
 */
 
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
  * D3 *    - RedLED
  * D4      - diskRx
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
  * D13     - diskTx
  */

#define LCD_RS A5
#define LCD_EN 7
#define LCD_DB4 8
#define LCD_DB5 10
#define LCD_DB6 11
#define LCD_DB7 12
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_DB4, LCD_DB5, LCD_DB6, LCD_DB7);
mdComm::message m;
const int MSG_BUFFER_SIZE = 64;
char buffer[MSG_BUFFER_SIZE];
char buffer2[MSG_BUFFER_SIZE];

uint16_t base_wait = 0;

void setup()
{
  Serial.begin(9600);

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
}

bool waiting_for_base = true;
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

void loop()
{
  if(waiting_for_base) {
    base_wait_loop();
  }
}

 
