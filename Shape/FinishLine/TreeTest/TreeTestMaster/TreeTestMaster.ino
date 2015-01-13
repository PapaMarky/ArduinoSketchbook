#include <Wire.h>



void setup()
{
  Wire.begin(); // join i2c bus as master
}

const int tree_address = 4;

char stage = '1';

void loop() {
  Wire.beginTransmission(tree_address);
  Wire.write(stage);
  Wire.endTransmission();
  delay(1000);
  stage++;
  if (stage > '4') stage = '1';
}
