#include <Button.h>

const int buttonPin = 6;
const int solenoidPin = A5;
const int SER_Pin = 13;   //pin 14 on the 75HC595
const int RCLK_Pin = 12;  //pin 12 on the 75HC595
const int SRCLK_Pin = 11; //pin 11 on the 75HC595

//How many of the shift registers - change this
#define number_of_74hc595s 1 

//do not touch
#define numOfRegisterPins number_of_74hc595s * 8

boolean registers[numOfRegisterPins];

Button button;

void setup(){
  Serial.begin(9600);
  button.setup(buttonPin);
  pinMode(solenoidPin, OUTPUT);

  pinMode(SER_Pin, OUTPUT);
  pinMode(RCLK_Pin, OUTPUT);
  pinMode(SRCLK_Pin, OUTPUT);
  
  //reset all register pins
  clearRegisters();
  writeRegisters();
}               

//set all register pins to LOW
void clearRegisters(){
  for(int i = numOfRegisterPins - 1; i >=  0; i--){
     registers[i] = HIGH;
  }
} 

//Set and display registers
//Only call AFTER all values are set how you would like (slow otherwise)
void writeRegisters(){

  digitalWrite(RCLK_Pin, LOW);

  for(int i = numOfRegisterPins - 1; i >=  0; i--){
    digitalWrite(SRCLK_Pin, LOW);

    int val = registers[i];

    digitalWrite(SER_Pin, val);
    digitalWrite(SRCLK_Pin, HIGH);

  }
  digitalWrite(RCLK_Pin, HIGH);

}

//set an individual pin HIGH or LOW
void setRegisterPin(int index, int value){
  registers[index] = value;
}

void setSeg(int s)
{
  for(int i = 0; i < 8; i++) {
    setRegisterPin(i, s==i?LOW:HIGH);
  }
  writeRegisters();
}
const int A = 1;
const int B = 0;
const int C = 5;
const int D = 6;
const int E = 7;
const int F = 2;
const int G = 3;
const int DP = 4;

void Display(int b) {
  switch(b) {
    case 0:
      Display('0');
      break;
    case 1:
      Display('1');
      break;
    case 2:
      Display('2');
      break;
    case 3:
      Display('3');
      break;
    case 4:
      Display('4');
      break;
    case 5:
      Display('5');
      break;
    case 6:
      Display('6');
      break;
    case 7:
      Display('7');
      break;
    case 8:
      Display('8');
      break;
    case 9:
      Display('9');
      break;
    default:
      Display('E');
      break;
  }
}

void Display(char c)
{
  clearRegisters();
  switch(c) {
    case '0':
      registers[A] = LOW;
      registers[B] = LOW;
      registers[C] = LOW;
      registers[D] = LOW;
      registers[E] = LOW;
      registers[F] = LOW;
      break;
      
    case '1':
      registers[B] = LOW;
      registers[C] = LOW;
      break;
 
    case '2':
      registers[A] = LOW;
      registers[B] = LOW;
      registers[G] = LOW;
      registers[E] = LOW;
      registers[D] = LOW;
      break;
            
    case '3':
      registers[A] = LOW;
      registers[B] = LOW;
      registers[G] = LOW;
      registers[C] = LOW;
      registers[D] = LOW;
      break;
            
    case '4':
      registers[F] = LOW;
      registers[B] = LOW;
      registers[G] = LOW;
      registers[C] = LOW;
      break;
            
    case '5':
      registers[A] = LOW;
      registers[F] = LOW;
      registers[G] = LOW;
      registers[C] = LOW;
      registers[D] = LOW;
      break;
            
    case '6':
      registers[A] = LOW;
      registers[F] = LOW;
      registers[G] = LOW;
      registers[C] = LOW;
      registers[D] = LOW;
      registers[E] = LOW;
      break;

    case '7':
      registers[A] = LOW;
      registers[B] = LOW;
      registers[C] = LOW;
      break;
            
    case '8':
      registers[A] = LOW;
      registers[B] = LOW;
      registers[C] = LOW;
      registers[D] = LOW;
      registers[E] = LOW;
      registers[F] = LOW;
      registers[G] = LOW;
      break;

    case '9':
      registers[A] = LOW;
      registers[F] = LOW;
      registers[B] = LOW;
      registers[G] = LOW;
      registers[C] = LOW;
      break;
            
    case 'E':
      registers[A] = LOW;
      registers[F] = LOW;
      registers[G] = LOW;
      registers[E] = LOW;
      registers[D] = LOW;
      break;
    
    case ' ':
    default:
    break;
  }
  writeRegisters();
}

// 0 = G
// 1 = F
// 2 = A
// 3 = B
// 4 = DP
// 5 = C
// 6 = D
// 7 = E
int num = -1;
void loop(){
  button.update();
  if(button.JustPressed()){
    num++;
    if(num > 9 
                                                                                          ) {num = 0;}
    Display(num);
    Serial.print("Display: ");
    Serial.println(num);
    
    digitalWrite(solenoidPin, HIGH);
    delay(1000);
    digitalWrite(solenoidPin, LOW);
    Serial.println("Ok");
   
  }  
}

