int pin[8] = {13,12,11,10,9,8,7,6};

// using 180 ohm resistors

void setup()
{
  Serial.begin(9600);
  
  for(int i = 0; i < 8; i++)
  {
    pinMode(pin[i], OUTPUT);
  }
}

void clear()
{
  for(int i = 0; i < 8; i++) {
    digitalWrite(pin[i], HIGH);
  }
}

// 0 = B (6)
// 1 = A (7)
// 2 = F (9)
// 3 = G (10)
// 4 = E (1)
// 5 = D (2)
// 6 = C (4)
// 7 = DP (5)

const int pinA = 1;
const int pinB = 0;
const int pinC = 6;
const int pinD = 5;
const int pinE = 4;
const int pinF = 2;
const int pinG = 3;
const int pinDP = 7;

void SetSegment(int s)
{
  digitalWrite(pin[s], LOW); // LOW is on
}

void setA() { digitalWrite(pin[pinA], LOW); }
void setB() { digitalWrite(pin[pinB], LOW); }
void setC() { digitalWrite(pin[pinC], LOW); }
void setD() { digitalWrite(pin[pinD], LOW); }
void setE() { digitalWrite(pin[pinE], LOW); }
void setF() { digitalWrite(pin[pinF], LOW); }
void setG() { digitalWrite(pin[pinG], LOW); }
void setDP() { digitalWrite(pin[pinDP], LOW); }

void SetOne() {
  clear(); setB(); setC();
  Serial.println("One");
}

void SetTwo() {
  clear(); setA();setB();setG();setE();setD();
  Serial.println("Two");
}

void SetThree() {
  clear();setA();setB();setG();setC();setD();
  Serial.println("Three");
}
void SetFour() {
  clear();setF();setG();setB();setC();
  Serial.println("Four");
}
void SetFive() {
  clear(); setA();setF();setG();setC();setD();
  Serial.println("Five");
}
void SetSix() {
  clear();setA();setF();setG();setE();setC();setD();
  Serial.println("Six");
}

void SetSeven() {
  clear(); setA();setB();setC();
  Serial.println("Seven");
}
void SetEight() {
  clear(); setA();setB();setC();setD();setE();setF();setG();
  Serial.println("Eight");
}
void SetNine() {
  clear(); setA();setF();setB();setC();setG();
  Serial.println("Nine");
}
void SetZero(){
  clear();setA();setB();setC();setD();setE();setF();
  Serial.println("Zero");
}
void loop()
{
  setA(); delay(1000);
  setB(); delay(1000);
  setC(); delay(1000);
  setD(); delay(1000);
  setE(); delay(1000);
  setF(); delay(1000);
  setG(); delay(1000);
  setDP(); delay(1000);
  /**/
  SetNine(); delay(1000);
  SetEight(); delay(1000);
  SetSeven(); delay(1000);
  SetSix(); delay(1000);
  SetFive(); delay(1000);
  SetFour(); delay(1000);
  SetThree(); delay(1000);
  SetTwo(); delay(1000);
  SetOne(); delay(1000);
  SetZero(); delay(1000);
  /**/
  clear(); delay(1000);
}

