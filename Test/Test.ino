const int PWM_1 = 3;
const int MODE_SWITCH = 2;
const int PHOTOCELL = A0;
const int DIMMER = A1;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(PWM_1, OUTPUT);
  pinMode(MODE_SWITCH, INPUT);
  pinMode(PHOTOCELL, INPUT);
  pinMode(DIMMER, INPUT);
}
static float brightness = 255;
const int dark = 200;
void loop() {
  int light = analogRead(PHOTOCELL);
  int d = 1024 - analogRead(DIMMER);
  float dimmer = (float)d /1023.0;
  int mode = digitalRead(MODE_SWITCH);
  /*
  Serial.print("dimmer: ");
  Serial.print(d);
  Serial.print(" : ");
  Serial.print(dimmer, 2);
  Serial.println("");
  */
  int brightness = (int)max(255.0 * dimmer, 1.0);
  int b = (mode || (light < dark)) ? brightness : 0;
  //Serial.println(brightness);
  static int last_b = 0;
  if (b != last_b) {
    last_b = b;
    analogWrite(PWM_1, b);
  }
  delay(10);
  
}
