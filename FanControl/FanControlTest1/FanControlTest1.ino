#include <LiquidCrystal.h>

#include <mdlib.h>
#include <Updatable.h>
///////////// THERMISTOR ////////////////////////
#include <math.h>

#define TempPinUp A1                 // Analog Pin 0
#define ThermistorPinDown A0

//float vcc = 4.91;                       // only used for display purposes, if used
                                        // set to the measured Vcc.
//float pad = 9750;                       // balance/pad resistor value, set this to
                                        // the measured resistance of your pad resistor
float thermr = 10000;                   // thermistor nominal resistance

// float only has 6 ~ 7 digits of accuracy 
const float X = 112.9148; //0.001129148;
const float Y = 23.4125; // 0.000234125;
const float Z = 0.00876741; //0.0000000876741;
const float N = 100000.0;

float Thermistor(int RawADC, float pad) {
  long Resistance;  
  float Temp;  // Dual-Purpose variable to save space.

  Resistance=((1024 * pad / RawADC) - pad); 
  Temp = log(Resistance); // Saving the Log(resistance) so not to calculate  it 4 times later
  Temp = 1.0 / ((X + (Y * Temp) + (Z * Temp * Temp * Temp))/N);
  Temp = Temp - 273.15;  // Convert Kelvin to Celsius                      

  // Uncomment this line for the function to return Fahrenheit instead.
  //temp = (Temp * 9.0)/ 5.0 + 32.0;                  // Convert to Fahrenheit
  return Temp;                                      // Return the Temperature
}

float cuberoot(float x) {
  float rval = pow(x, 1.0/3.0);
  Serial.print(" - cuberoot(");Serial.print(x,7);
  Serial.print(") = "); Serial.println(rval,7);
  return rval;
}

// returns the 'pad' to use
float calibrate(int RawADC, float temp) {
  Serial.print("calibrate("); Serial.print(RawADC);
  Serial.print(", "); Serial.print(temp); Serial.println(")");
  float a = Z;
  float c = Y;
  float d = -(N / (temp + 273.15)) - (X*N);
  
  float tmp1 = (27.0 * a * a * d);
  Serial.print("  tmp1 = "); Serial.println(tmp1,7);
  float tmp2 = (-3 * a * c);
  Serial.print("  tmp2 = "); Serial.println(tmp2,7);
  
  Serial.print(" Q = sqrt( "); Serial.print(tmp1*tmp1, 7);
  Serial.print(" - 4.0 * "); Serial.print(tmp2*tmp2*tmp2, 7); Serial.println(")");

  Serial.print(" Q = sqrt( "); Serial.print(tmp1*tmp1, 7);
  Serial.print(" - "); Serial.print(4.0*tmp2*tmp2*tmp2, 7); Serial.println(")");
  
  Serial.print(" Q = sqrt( "); Serial.print((tmp1 * tmp1) - (4.0 * tmp2 * tmp2 * tmp2), 7);
  Serial.println(")");
  
  float Q = sqrt( tmp1 * tmp1 - 4.0 * tmp2 * tmp2 * tmp2);
  Serial.print("  Q = "); Serial.println(Q,7);
  Serial.print("  a = "); Serial.println(a,7);
  Serial.print("  d = "); Serial.println(d,7);
  Serial.print("  a^2 = "); Serial.println(a*a,7);
  Serial.print("  a^2*d = "); Serial.println(a*a*d,7);
  Serial.print("  27.0*a^2*d = "); Serial.println(27.0*a*a*d,7);
  Serial.print("  Q+27.0*a^2*d = "); Serial.println(Q+27.0*a*a*d,7);
  Serial.print("  Q+27.0*a^2*d/2.0 = "); Serial.println((Q+27.0*a*a*d)/2.0,7);
  float C = cuberoot( (Q + 27.0 * a * a * d) / 2.0 );
  Serial.print("  C = "); Serial.println(C,7);
  
  float x1 = - (C / 3*a) - ( ( - 3.0 * a * c) / (3.0 * a * C) );
  Serial.print("  x1 = "); Serial.println(x1,7);
  
  float pad = pow(10.0, x1 - log((1024.0/RawADC)-1.0));
  Serial.print(" RETURNING: "); Serial.println(pad);
  return pad;
}

////////////////////////////////END THERMISTOR//////////////////////

const int CONTROL_PIN = 2;
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

void setup() {
  Serial.begin(9600);
  pinMode(CONTROL_PIN, OUTPUT);
  pinMode(13, OUTPUT);
  lcd.begin(16, 2);
}
const float start_delta = 2.0;
const float stop_delta = 1.0;

float delta;
float tempUp;
float tempDown;
bool fanIsOn = false;
/*
void loop() {
  float temp;
  float tempUp;
  temp=Thermistor(analogRead(ThermistorPIN));       // read ADC and  convert it to Celsius
  tempUp=Thermistor(analogRead(ThermistorPIN2));
  lcd.clear();
  lcd.home();
  lcd.print("FAN:  ON  ");
  int iTemp = (int)(temp * 10.0);
  String tempStr(temp, 1);
  int pos = 16 - tempStr.length();
  lcd.print(temp, 1);
  lcd.print(" C");
  digitalWrite(CONTROL_PIN, HIGH);
  digitalWrite(13, HIGH);
  delay(3000);
  digitalWrite(13, LOW);
  temp=Thermistor(analogRead(ThermistorPIN));       // read ADC and  convert it to Celsius
  temp = (temp * 9.0)/ 5.0 + 32.0;                  // converts to  Fahrenheit
  lcd.clear();
  lcd.home();
  lcd.print("FAN: OFF  ");
  lcd.print(temp, 1);
  lcd.print(" F");
  digitalWrite(CONTROL_PIN, LOW);
  delay(3000);
}
*/
float PAD1 = 10340.0;
float PAD2 = 10156.0;
void loop() {
  if (Serial.available()) {
    int command = Serial.read();
    if (command == 'C') {
      int t_in = 0;
      while(Serial.available()) {
        t_in = (t_in * 10) + (Serial.read() - 48);
      }
      float temp = (float)t_in/10.0;
      Serial.print(" CALIBRATION: temp = "); Serial.println(temp);
      Serial.println("---------- CALIBRATION (Therm Up) --------------");
      PAD1 = calibrate(analogRead(TempPinUp), temp);
      Serial.println("---------- CALIBRATION (Therm Down) --------------");
      PAD2 = calibrate(analogRead(ThermistorPinDown), temp);
    }
  }
  getTemperatures();
  setFanState();
  updateDisplay();
  
  delay(5000);
}

void updateDisplay() {
  char buffer[32];
  
  lcd.clear();
  lcd.home();
  
  lcd.print(fanIsOn ? " ON" : "OFF");
  
  //float t = (tempUp * 9.0)/ 5.0 + 32.0;
  dtostrf(tempUp, 4, 1, buffer);
  lcd.setCursor(16 - (strlen(buffer) + 2), 0);
  lcd.print(buffer);
  lcd.print(" F");
  
  //t = (tempDown * 9.0)/ 5.0 + 32.0;
  dtostrf(tempDown, 4, 1, buffer);
  lcd.setCursor(16 - (strlen(buffer) + 2), 1);
  lcd.print(buffer);
  lcd.print(" F");
  
  lcd.setCursor(0, 1);
  lcd.print("d: ");
  dtostrf(delta, 4, 1, buffer);
  lcd.print(buffer);  
}

float CtoF(float c) {
  return (c * 9.0)/ 5.0 + 32.0;
}

const float aref_voltage = 5.0;

void getTemperatures() {
  /*
  float tempReading = 0.0;
  for(int i = 0; i < 5; i++) {
    tempReading += analogRead(TempPinUp);
    delay(10);
  }
  tempReading /= 5.0;
  
  float voltage = tempReading * aref_voltage;
  voltage /= 1024.0;
  
  float tempC = (voltage - 0.5) * 100.0;
  */
  tempUp = Thermistor(analogRead(TempPinUp), PAD1);
  tempUp = CtoF(tempUp);
  tempDown = Thermistor(analogRead(ThermistorPinDown), PAD2); //9750.0);
  tempDown = CtoF(tempDown);
}

void setFanState() {
  delta = tempUp - tempDown;
  if (delta < 0) 
    delta = delta * -1.0;
    
  if (fanIsOn && delta <= stop_delta)
    stopFan();
  else if (!fanIsOn && delta >= start_delta)
    startFan();
}

void stopFan() {
  digitalWrite(CONTROL_PIN, LOW);
  fanIsOn = false;
}

void startFan() {
  digitalWrite(CONTROL_PIN, HIGH);
  fanIsOn = true;
}
  
