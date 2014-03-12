#include <SoftwareSerial.h>

#include <mdBase.h>
#include <MultiColorLED.h>
#include <Queue.h>
#include <SevenSegmentDisplay.h>
#include <StateMachine.h>

const int dispRx = 2;
const int dispTx = 3;
const int latchPin = 8;
const int clockPin = 12;
const int dataPin = 11;

const int buzzerPin = 6;
const int n_buttons = 6;
const int button[n_buttons] =
{A0, A1, A2, A3, A4, A5};
const int advanceButton = 4;
int button_state[n_buttons];

byte led_state = 0x00;
mdlib::SevenSegmentDisplay display(dispRx, dispTx);

void beep(unsigned char delayms, int n) {
  Serial.print("beep: ");
  Serial.println(n);
  analogWrite(buzzerPin, n);
  delay(delayms);
  analogWrite(buzzerPin, 0);
  delay(delayms);
}

void shiftOut(int myDataPin, int myClockPin, byte myDataOut) {
  Serial.println(myDataOut, HEX);
  // This shifts 8 bits out MSB first, 
  //on the rising edge of the clock,
  //clock idles low

  digitalWrite(latchPin, LOW);
  //internal function setup
  int i=0;
  int pinState;
  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, OUTPUT);

  //clear everything out just in case to
  //prepare shift register for bit shifting
  digitalWrite(myDataPin, 0);
  digitalWrite(myClockPin, 0);

  //for each bit in the byte myDataOut�
  //NOTICE THAT WE ARE COUNTING DOWN in our for loop
  //This means that %00000001 or "1" will go through such
  //that it will be pin Q0 that lights. 
  for (i=7; i>=0; i--)  {
    digitalWrite(myClockPin, 0);

    //if the value passed to myDataOut and a bitmask result 
    // true then... so if we are at i=6 and our value is
    // %11010100 it would the code compares it to %01000000 
    // and proceeds to set pinState to 1.
    if ( myDataOut & (1<<i) ) {
      pinState= 1;
    }
    else {  
      pinState= 0;
    }

    //Sets the pin to HIGH or LOW depending on pinState
    digitalWrite(myDataPin, pinState);
    //register shifts bits on upstroke of clock pin  
    digitalWrite(myClockPin, 1);
    //zero the data pin after shift to prevent bleed through
    digitalWrite(myDataPin, 0);
  }
  digitalWrite(latchPin, HIGH);
}

void SetLedState(int led, bool state) {
  if (led < 0 || led >= n_buttons) 
    return;
   
   led = n_buttons - led - 1;
   if (state) {
     bitSet(led_state, led);
   }
   else {
     bitClear(led_state, led);
   }
   
   shiftOut(dataPin, clockPin, led_state);
}

void setup() {
  Serial.begin(9600);
  display.setup();
  display.set(" APU");
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  
  pinMode(buzzerPin, OUTPUT);
  
  for(int i = 0; i < n_buttons; i++) {
    pinMode(button[i], INPUT);
    button_state[i] = LOW;
  }
}

unsigned int tones[8] = {
  65, // c2 - 0
  73, // d2 - 1
  82, // e2 - 2
  87, // f2 - 3
  98, // g2 - 4
  110, // a2 - 5
  123, // b2
  131  // c3
};

void loop() {
  int bs = digitalRead(advanceButton);
  if (bs)
    Serial.println("ADVANCE");
  for(int i = 0; i < n_buttons; i++) {
    int bs = digitalRead(button[i]);
    if (button_state[i] != bs) {
      button_state[i] = bs;
      SetLedState(i, bs);
      Serial.print("Button[");
      Serial.print(i);
      Serial.print("] is ");
      Serial.println( bs == HIGH ? "HIGH" : "LOW");
      if(bs) {
        display.set(6 - i);
        tone(buzzerPin, tones[5 - i], 200);
      }
      else
        display.clear();
    }
  }
  display.update();
}

