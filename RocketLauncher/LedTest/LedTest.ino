int pin = A0;

void setup()
{
  pinMode(pin, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
    //digitalWrite(pin, LOW);
    //delay(900);
    digitalWrite(pin, HIGH);
    delay(100);
}

