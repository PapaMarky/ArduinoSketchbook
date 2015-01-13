// which analog pin to connect
#define THERMISTORPIN A0         
// resistance at 25 degrees C
#define THERMISTORNOMINAL 10000      
// temp. for nominal resistance (almost always 25 C)
#define TEMPERATURENOMINAL 25   
// how many samples to take and average, more takes longer
// but is more 'smooth'
#define NUMSAMPLES 5
// The beta coefficient of the thermistor (usually 3000-4000)
#define BCOEFFICIENT 3950
// the value of the 'other' resistor
#define SERIESRESISTOR 10000    
 
int samples[NUMSAMPLES];
 
void setup(void) {
  Serial.begin(9600);
  analogReference(EXTERNAL);
}

void sendSample() {
  uint8_t i;
  float average;
 
  // take N samples in a row, with a slight delay
  average = 0.0;
  int retries = 0;
  for (i=0; i< NUMSAMPLES; i++) {
   samples[i] = analogRead(THERMISTORPIN);
   //Serial.print("SAMPLE: "); Serial.println(samples[i]);
   if (samples[i] == 0 || samples[i] == 1023) {
     retries++;
     if (retries < 20) {
       i--;
       delay(10);
       continue;
     } else {
       //Serial.print("TOO MANY RETRIES, i = "); Serial.println(i);
       retries = 0;
     }
   }
   average += samples[i];
   delay(10);
  }
  average /= NUMSAMPLES;
 
  //Serial.print("Average analog reading "); 
  //Serial.println(average);
 
  // convert the value to resistance
  average = 1023 / average - 1;
  average = SERIESRESISTOR / average;
  //Serial.print("Thermistor resistance "); 
  //Serial.println(average);
 
  float steinhart;
  steinhart = average / THERMISTORNOMINAL;     // (R/Ro)
  steinhart = log(steinhart);                  // ln(R/Ro)
  steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                 // Invert
  steinhart -= 273.15;                         // convert to C
 
  //Serial.print("Temperature "); 
  Serial.println(steinhart);
  //Serial.println(" *C");
}

void loop(void) {
  if (Serial.available() <= 0)
    return;
  
  while(Serial.available() > 0) {
    int incommingByte = Serial.read();
  }
  sendSample();
}
