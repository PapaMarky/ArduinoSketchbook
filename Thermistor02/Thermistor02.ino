#include <SoftwareSerial.h>

const int rxpin = 2; // unused
const int txpin = 3;

SoftwareSerial myDisplay(rxpin, txpin);
#include <math.h>

#define ThermistorPIN 7                 // Analog Pin 0

float vcc = 5.0;                       // only used for display purposes, if used
                                        // set to the measured Vcc.
float pad = 10195;                       // balance/pad resistor value, set this to
                                        // the measured resistance of your pad resistor
float thermr = 10000;                   // thermistor nominal resistance
//float pinc = 1;

float Thermistor(int RawADC) {
  long Resistance;  
  float Temp;  // Dual-Purpose variable to save space.

  Resistance=((1024.0 * pad / (float)RawADC) - pad); 
  float logTemp = log(Resistance); // Saving the Log(resistance) so not to calculate  it 4 times later
  Temp = 1 / (0.001129148 + (0.000234125 * logTemp) + (0.0000000876741 * logTemp * logTemp * logTemp));
  Temp = Temp - 273.15;  // Convert Kelvin to Celsius                      
  
  /*
  Serial.print("ADC: "); 
  Serial.print(RawADC); 
  Serial.print("/1024");                           // Print out RAW ADC Number
  Serial.print(", vcc: ");
  Serial.print(vcc,2);
  Serial.print(", pad: ");
  Serial.print(pad,3);
  Serial.print(" Kohms, Volts: "); 
  Serial.print(((RawADC*vcc)/1024.0),3);   
  Serial.print(", R: "); 
  Serial.print(Resistance);
  Serial.print(" ohms, ");
  Serial.print(logTemp);
  Serial.print(" logTemp, ");
  //Serial.print(Temp);
  //Serial.print(" C");
  //Serial.println("");
*/
  return Temp;                                      // Return the Temperature
}


void setup()
{
  Serial.begin(9600);
  myDisplay.begin(9600);
}
  
char s[5];
void showTemp(float temp, char unit)
{
  int temp_i = (int)(temp * 10.0);
  //Serial.print("temp_i: ");Serial.println(temp_i);
  
  myDisplay.write(0x77);
  myDisplay.write(0x02);

  long  a,b,c,d;
  
  // use itoa?
  
  a = temp_i/100;
  b = (temp_i - (a * 100))/10;
  c = (temp_i - (a * 100) - (b * 10));
  
  s[0] = a == 0 ? ' ' : (unsigned char)a + '0';
  s[1] = b == 0 && a == 0 ? ' ' : (unsigned char)b + '0';
  s[2] = c == 0 && b == 0 && a == 0 ? ' ' : (unsigned char)c + '0';
  s[3] = unit;
  s[4] = 0;
  myDisplay.print(s);
}

bool decimal_set = false;
int show_time = 3000;
void loop()
{
  float temp = Thermistor(analogRead(ThermistorPIN));

  showTemp(temp, 'C');
  delay(show_time);
  temp = (temp * 9.0)/ 5.0 + 32.0; // converts to  Fahrenheit
  Serial.print(temp,1);
  Serial.println(" F");
  showTemp(temp, 'F');
  delay(show_time);
  //pad += pinc;
}


