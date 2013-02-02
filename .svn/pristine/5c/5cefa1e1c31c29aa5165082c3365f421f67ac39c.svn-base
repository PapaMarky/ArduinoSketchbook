#include <SoftwareSerial.h>

const int rxpin = 2; // unused
const int txpin = 3;

SoftwareSerial myDisplay(rxpin, txpin);

void setup()
{
  Serial.begin(9600);
  myDisplay.begin(9600);
  
}

long count = 0;
unsigned int max_count = 9999;

int incr = 1;
  
char s[5];

bool decimal_set = false;
void loop()
{
  //if(!decimal_set) {
    myDisplay.write(0x77);
    myDisplay.write(0x04);
    decimal_set = true;
  //}

  long  a,b,c,d;
  
  // use itoa?
  
  a = count/1000;
  b = (count - (a * 1000))/100;
  c = (count - (a * 1000) - (b * 100))/10;
  d = count - (a * 1000) - (b * 100) - (c * 10);
  
  s[0] = a == 0 ? ' ' : (unsigned char)a + '0';
  s[1] = b == 0 && a == 0 ? ' ' : (unsigned char)b + '0';
  s[2] = c == 0 && b == 0 && a == 0 ? ' ' : (unsigned char)c + '0';
  s[3] = (unsigned char) d + '0';
  s[4] = 0;
  myDisplay.print(s);
  /*
  Serial.print("Count: ");
  Serial.print(count);
  Serial.print(" : ");
  Serial.print(a);
  Serial.print(",");
  Serial.print(b);
  Serial.print(",");
  Serial.print(c);
  Serial.print(",");
  Serial.println(d);
  Serial.println(s);
  */
  delay(100);
  //myDisplay.write(0x77);
  //myDisplay.write((unsigned char)0x00);
  
  //delay(500);  
  
  if(incr == 1 && count >= max_count){
    incr = -1;
  }
  else if(incr == -1 && count <= 0){
    incr = 1;
  }
  count += incr;
}


