//Analog out
//by Amanda Ghassaei
//http://www.instructables.com/id/Arduino-Audio-Output/
//Sept 2012

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
*/

void setup(){
  //set digital pins 0-7 as outputs
  for (int i=0;i<8;i++){
    pinMode(i,OUTPUT);
  }
}

void loop(){
  PORTD = 0;//send (0/255)*5 = 0V out DAC
  delay(1);//wait 1ms
  PORTD = 127;//send (127/255)*5 = 2.5V out DAC
  delay(2);//wait 2ms
  PORTD = 51;//send (51/255)*5 = 1V out DAC
  delay(1);//wait 1ms
  PORTD = 255;//send (255/255)*5 = 5V out DAC
  delay(3);//wait 3ms
}

