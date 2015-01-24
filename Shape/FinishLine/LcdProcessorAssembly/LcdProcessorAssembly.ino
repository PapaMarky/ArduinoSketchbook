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
  * D3 *    - diskTx
  * D4      - diskRx
  *
  * D5 *    - Button LED
  * D6 *    - Button
  * D7      - LCD_EN
  * D8      - LCD_DB4
  *
  * D9 *
  * D10*
  * D11*    - LCD_DB5
  * D12     - LCD_DB6
  * D13     - LCD_DB7 
  */
 
void setup()
{
  Serial.begin(9600);
}
 
void loop()
{
  int incomming = 0;
  if (Serial.available() > 0) {
    incomming = Serial.read();
    Serial.write(incomming + 1);
  }
}
 
