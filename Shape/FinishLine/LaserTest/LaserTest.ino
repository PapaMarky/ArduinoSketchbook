#include <SoftwareSerial.h>

int lightPin = 0;  //define a pin for Photo resistor
int ledPin=11;     //define a pin for LED
int displayRx = 10;
int displayTx = 9; // This one to the LCD green wire

// OpenLog:
int diskRx = 7;
int diskTx = 6;
int diskReset = 2;

SoftwareSerial disk(diskRx, diskTx);

SoftwareSerial LCD(displayRx, displayTx);
void findDiskPrompt() {
  Serial.println("findDiskPrompt");
 //Wait for OpenLog to respond with '<' to indicate it is alive and recording to a file
  boolean ready = false;
  while(1) {
    if(disk.available()) {
      char c = disk.read();
      Serial.write(c);
      if(c == '<') break;
      if(c == '>') {
        ready = true;
        Serial.println("");
        break;
      }
    }
    else {
      disk.println("x");
    }
  }
  
  if (!ready) {
    disk.write(26);
    disk.write(26);
    disk.write(26);
    
    //Wait for OpenLog to respond with '>' to indicate we are in command mode
    while(1) {
      if(disk.available()) {
        char c = disk.read();
        Serial.write(c);
        if(c == '>') {
          Serial.println("");
          break;
        }
      }
    }
  }
 }

void resetDisk() {
  //Reset disk
  Serial.println("resetDisk");
  digitalWrite(diskReset, LOW);
  delay(100);
  digitalWrite(diskReset, HIGH);
}

void setup()
{
    Serial.begin(9600);  //Begin serial communcation
    Serial.println("setup");
    pinMode( ledPin, OUTPUT );
    
    LCD.begin(9600);

    LCD.write(0xFE);
    LCD.write(0x01);
    LCD.write(0xFE);
    LCD.write(128);
    LCD.print("xxHowdyxx");
    
    pinMode(diskReset, OUTPUT);
    disk.begin(9600);
    resetDisk();
    
    findDiskPrompt();
    
    disk.println("ls");
}

const static int threshold = 990;

void loop()
{
  int l = analogRead(lightPin);
    LCD.write(0xFE);
    LCD.write(0x01);
    LCD.write(0xFE);
    LCD.write(128);
    LCD.println(l); //Write the value of the photoresistor to the serial monitor.

//    digitalWrite(ledPin, (l > threshold ? HIGH : LOW));
   delay(10); //short delay for faster response to light.
}
