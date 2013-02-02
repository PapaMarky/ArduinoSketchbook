
/*

(white line of capacitor towards buzzer)

pin3 --(+)100mF ---+---------| 
            Cap    |         |
                  110        |
                  ohm     Buzzer
                   |         |
GND---------------------------


*/

const int pin = 9;

void setup()
{
  pinMode(pin, OUTPUT);
}

void loop()
{
  tone(pin, 500, 250);
  delay(1000);
}

