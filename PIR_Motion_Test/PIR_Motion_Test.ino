/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 6;
int pir_pin = 7;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);    
 pinMode(pir_pin, INPUT); 
 //Serial.begin(9600);
}

// the loop routine runs over and over again forever:
bool on = false;
void loop() {
  if (digitalRead(pir_pin)) {
    digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
    if (!on) {
      //Serial.println(" *** ON");
      on = true;
    }
  }
  else {
    digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
    if (on) {
      //Serial.println(" *** OFF");
      on = false;
    }
  }
  delay(100);               // wait for a second
}
