#include <Button.h>

const int buttonPin = 8;
const int solenoidPin = 6;

/*
 * Hook up per instructions on page 301.
 */
 
Button button;

void setup()
{
  pinMode(solenoidPin, OUTPUT);
  button.setup(buttonPin);
  Serial.begin(9600);
}

void loop()
{
  button.update();
  
  if(button.JustPressed())
  {
    Serial.println("Click!");
    
    digitalWrite(solenoidPin, HIGH);
    delay(1000);
    digitalWrite(solenoidPin, LOW);
    Serial.println("Ok");
  }
}

