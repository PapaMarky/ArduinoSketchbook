#define sensorIR A0               //Must be an analog pin
float sensorValue, inches, cm;    //Must be of type float for pow()

void setup() {
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    int b = Serial.read();
    sensorValue = analogRead(sensorIR);
    inches = 4192.936 * pow(sensorValue,-0.935) - 3.937;
    //cm = 10650.08 * pow(sensorValue,-0.935) - 10;
    Serial.print("Inches: ");
    Serial.println(inches);
    delay(1000);
  }
}
