#include <BaseComponent.h>
#include <button_id.h>
#include <Controller.h>
#include <debug_lcd.h>
#include <scooter_data.h>
#include <scooter_lcd.h>
#include <screen.h>

// -*-c++-*-
// Copyright 2015, Mark Dyer
/**
 * LcdProcessorAssembly - the processor stuck to the LCD in the lid
 *
 * 
 */
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>


#include "scooter_common.h"

#include "scooter_data.h"
#include "debug_lcd.h"
#include "screen.h"
#include "Controller.h"
#include "BaseComponent.h"

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

Controller controller;

LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_DB4, LCD_DB5, LCD_DB6, LCD_DB7);

debug_lcd DEBUGGER;

ConsoleScreen startup_screen(&lcd);

#define DISK_RESET 13
#define DISK_TX 2
#define DISK_RX 4
//SoftwareSerial disk(DISK_RX, DISK_TX);
//ScooterData database(DISK_RESET, &disk);

#define SW5_SELECT_PIN A0
#define SW5_RIGHT_PIN  A1
#define SW5_DOWN_PIN   A2
#define SW5_LEFT_PIN   A3
#define SW5_UP_PIN     A4

#define RED_LED_PIN       3
#define YELLOW_LED_PIN    9
#define GREEN_LED_PIN     5
#define GO_BUTTON_LED_PIN 6

const int MSG_BUFFER_SIZE = 64;
char buffer[MSG_BUFFER_SIZE];

uint32_t base_wait = 0;
#include "button_id.h"

ButtonComponent select_button(SW5_SELECT_PIN, SELECT_BUTTON, &controller);
ButtonComponent right_button(SW5_RIGHT_PIN, RIGHT_BUTTON, &controller);
ButtonComponent left_button(SW5_LEFT_PIN, LEFT_BUTTON, &controller);
ButtonComponent up_button(SW5_UP_PIN, UP_BUTTON, &controller);
ButtonComponent down_button(SW5_DOWN_PIN, DOWN_BUTTON, &controller);

AnalogOutComponent red_led(RED_LED_PIN);
AnalogOutComponent yellow_led(YELLOW_LED_PIN);
AnalogOutComponent green_led(GREEN_LED_PIN);
AnalogOutComponent go_button_led(GO_BUTTON_LED_PIN);

BaseComponent base;

void setup()
{
  gdbg = &DEBUGGER;
  Serial.begin(BASE_LID_BAUD);
  lcd.begin(20,4);

  base.initialize(&Serial, HEARTRATE);
  base.setController(&controller);
  controller.addComponent(&base);

  controller.addComponent(&select_button);
  controller.addComponent(&right_button);
  controller.addComponent(&left_button);
  controller.addComponent(&up_button);
  controller.addComponent(&down_button);

  controller.addComponent(&red_led);
  controller.addComponent(&yellow_led);
  controller.addComponent(&green_led);
  controller.addComponent(&go_button_led);

  controller.setup();

  controller.setScreen(&startup_screen);

  //database.begin();
  //database.start_command_mode();
  //  lcd.setCursor(18, 3);
  //  lcd.print("d");
  startup_screen.addLine("Powering Up...");
}

void loop()
{
  uint32_t now = millis();
  controller.loop(now);
}

 
