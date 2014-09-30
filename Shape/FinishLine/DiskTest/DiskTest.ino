#include <SoftwareSerial.h>

/*
  6-1-2011
  SparkFun Electronics 2011
  Nathan Seidle
  
  Controlling OpenLog command line from an Arduino
  
  Connect the following OpenLog to Arduino:
    TXO of OpenLog to RX of the Arduino
    RXI to TX
    GRN to 2
    VCC to 5V
    GND to GND
    
  NOTE: When uploading this example code you must temporarily disconnect TX and RX while uploading 
  the new code to the Arduino. Otherwise you will get a "avrdude: stk500_getsync(): not in sync" error.
  
  This example code assumes the OpenLog is set to operate at 9600bps in NewLog mode, meaning OpenLog 
  should power up and output '12<'. This code then sends the three escape characters and then sends 
  the commands to create a new random file called nate###.txt where ### is a random number from 0 to 999.
  
  This code assume OpenLog is in the default state of 9600bps with ASCII-26 as the esacape character.
  
  Be careful when sending commands to OpenLog. println() sends extra newline characters that 
  cause problems with the command parser. The new v2.51 ignores \n commands so it should be easier to 
  talk to on the command prompt level. This example code works with all OpenLog v2 and higher.
  
 */

char buff[50];
int fileNumber;

int statLED = 13;
int resetOpenLog = 2;

SoftwareSerial disk(7, 6);

void findDiskPrompt() {
 //Wait for OpenLog to respond with '<' to indicate it is alive and recording to a file
  boolean ready = false;
  while(1) {
    if(disk.available()) {
      char c = disk.read();
      Serial.write(c);
      if(c == '<') break;
      if(c == '>') {
        ready = true;
        break;
      }
    }
  }
  
  if (!ready) {
    disk.write(26);
    disk.write(26);
    disk.write(26);
    
    //Wait for OpenLog to respond with '>' to indicate we are in command mode
    while(1) {
      if(disk.available())
        if(disk.read() == '>') break;
    }
  }
 }

void setup() {                
  pinMode(statLED, OUTPUT);
  pinMode(resetOpenLog, OUTPUT);
  
  randomSeed(analogRead(0));
  Serial.begin(9600);
  disk.begin(9600);
 
  //Reset OpenLog
  digitalWrite(resetOpenLog, LOW);
  delay(100);
  digitalWrite(resetOpenLog, HIGH);

  findDiskPrompt();

  fileNumber = random(999); //Select a random file #, 0 to 999
  
  //Send new (random from 0 to 999) file name
  
  disk.println("ls");
  
  //Wait for OpenLog to return to waiting for a command
  while(1) {
 
    if(disk.available()) {
      char c = disk.read();
      Serial.write(c);
      if(c == '>') break;
    }
  }
  sprintf(buff, "append nate%03d.txt\r", fileNumber);
  Serial.print(buff);
  disk.print(buff);
  //Wait for OpenLog to indicate file is open and ready for writing
  while(1) {
    if(disk.available())
      if(disk.read() == '<') break;
  }
}
void loop() {
  Serial.println("Yay!");
  digitalWrite(13, HIGH);
  delay(1000);
  digitalWrite(13, LOW);
  delay(1000);
}


