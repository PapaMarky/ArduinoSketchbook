#include <Narcoleptic.h>

#include <ShiftRegister.h>

#include <Button.h>

const int redPin = A3;
const int greenPin = 3;

const int buttonPin = 7;
const int sleepButtonPin = 6;

const int solenoidPin = A5;
const int SER_Pin = 13;   //pin 14 on the 75HC595
const int RCLK_Pin = 12;  //pin 12 on the 75HC595
const int SRCLK_Pin = 11; //pin 11 on the 75HC595

const int speakerPin = 9;

enum State {POWER_UP, READY, SLEEP, LAUNCH};

ShiftRegister shift_register(1, SER_Pin, RCLK_Pin, SRCLK_Pin);

Button button;
Button sleepButton;

void setup(){
  Serial.begin(9600);
  button.setup(buttonPin);
  sleepButton.setup(sleepButtonPin);
  pinMode(solenoidPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(speakerPin, OUTPUT);
  
  shift_register.setup();
  shift_register.clearRegisters(true);
  shift_register.writeRegisters();
}               
int num = -1;
State current_state = POWER_UP;
static unsigned long state_start = 0;

void LightsOn()
{
  shift_register.clearRegisters(false);
  digitalWrite(redPin, HIGH);
  digitalWrite(greenPin, HIGH); 
   shift_register.writeRegisters(); 
}
void LightsOff()
{
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, LOW);
  shift_register.clearRegisters(true);
   shift_register.writeRegisters(); 
}

void PowerUpLoop()
{
  Serial.println("Power Up!");
  playTheme();
  for(int i = 0; i < 10; i++) {
    LightsOn();
    delay(100);
    
    LightsOff();
    delay(100);
  }
  
  EnterReadyState();
}
void EnterReadyState()
{
  Serial.println("Enter Ready State");
  state_start = millis();
  digitalWrite(greenPin, HIGH);
  digitalWrite(redPin, LOW);
  current_state = READY;
}

void ReadyLoop()
{
  if(button.JustPressed()){
    StartLaunchSequence();
  }
  else if(sleepButton.State() == Button::PRESSED) {
    GoToSleep();
  }
}
void GoToSleep()
{
  Serial.println("** GO TO SLEEP **");
  LightsOff();
  Display(' ');
  state_start = millis();
  current_state = SLEEP;
}

void WakeUp()
{
  Serial.println("** WAKE UP **");
  EnterReadyState();
}

void SleepLoop()
{
  if(sleepButton.State() == Button::NOT_PRESSED) {
    WakeUp();
  }
  else {
    Narcoleptic.delay(3000);
    // Green LED blinks 0.1 seconds once every 3 seconds
    digitalWrite(greenPin, HIGH);
    delay(100);
    digitalWrite(greenPin, LOW);
  }
}
void StartLaunchSequence()
{
  Serial.println("Start launch sequence");
  digitalWrite(greenPin, LOW);
  current_state = LAUNCH;
  state_start = millis();
}

const int countStart = 5000;
void LaunchSequenceLoop()
{
  const int lo_tone = 600;
  const int hi_tone = 900;
  unsigned long elapsed = millis() - state_start;
  // if sleep button pressed, cancel launch sequence
  static int current_display = -1;
  static bool spkr_on = false;
  static bool led_on = false;
  if(elapsed <= countStart) {
    float seconds = (float)elapsed/1000.0;
    uint32_t s = (countStart/1000 - (uint32_t)seconds);
    
    uint32_t x = (uint32_t)((seconds - (float)(uint32_t)seconds) * 1000.0);
    
    Serial.print("X: "); Serial.print(x);
    Serial.print(", seconds = "); Serial.print(seconds);
    Serial.print(", s: "); Serial.print(s);
    Serial.print(spkr_on ? " Speaker ON, " : " Speaker OFF, ");
    Serial.print(led_on ? "LED ON":"LED OFF");
    Serial.println("");
    
    if(s != current_display) {
      Display((int)s);
      shift_register.writeRegisters();
      current_display = s;
      digitalWrite(redPin, HIGH);
      tone(speakerPin, lo_tone);
      spkr_on = true;
      led_on = true;
      Serial.println(" -- Start tone");
    }
    if(x >= 200 && spkr_on) {
      noTone(speakerPin);
      Serial.println(" -- End tone");
      spkr_on = false;
    }
    if(x >= 500 && led_on) {
      noTone(speakerPin);
      digitalWrite(redPin, LOW);
      led_on = false;
    }
  }
  else if(elapsed <= countStart + 1250) {
    if(current_display != 0)
    {
      Display(0);
      digitalWrite(redPin, HIGH);
      tone(speakerPin, hi_tone);
      current_display = 0;
      Serial.println("LAUNCH");
      // open the valve
      digitalWrite(solenoidPin, HIGH);
    }
    if(elapsed >= countStart + 750) {
      noTone(speakerPin);
    }
  }
  else {
    digitalWrite(solenoidPin, LOW);
    shift_register.clearRegisters(true);
    shift_register.writeRegisters();
    digitalWrite(redPin, LOW);
    Serial.println("Close Valve");
    EnterReadyState();
  }
}

void loop(){
  button.update();
  sleepButton.update();
  
  switch(current_state) {
    case POWER_UP:
      PowerUpLoop();
      break;
    case SLEEP:
      SleepLoop();
      break;
    case LAUNCH:
      LaunchSequenceLoop();
      break;
    case READY:
    default:
      ReadyLoop();
      break;
  }
}


void setSeg(int s)
{
  for(int i = 0; i < 8; i++) {
    shift_register.setRegisterPin(i, s==i?LOW:HIGH);
  }
  shift_register.writeRegisters();
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
  Serial.print(" - Display (int)");
  Serial.println(b);
  
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
  Serial.print("Display('");
  Serial.print(c);
  Serial.println("')");
  
  shift_register.clearRegisters(true);
  switch(c) {
    case '0':
      shift_register.setRegisterPin(A, LOW);
      shift_register.setRegisterPin(B, LOW);
      shift_register.setRegisterPin(C, LOW);
      shift_register.setRegisterPin(D, LOW);
      shift_register.setRegisterPin(E, LOW);
      shift_register.setRegisterPin(F, LOW);
      break;
      
    case '1':
      shift_register.setRegisterPin(B, LOW);
      shift_register.setRegisterPin(C, LOW);
      break;
 
    case '2':
      shift_register.setRegisterPin(A, LOW);
      shift_register.setRegisterPin(B, LOW);
      shift_register.setRegisterPin(G, LOW);
      shift_register.setRegisterPin(E, LOW);
      shift_register.setRegisterPin(D, LOW);
      break;
            
    case '3':
      shift_register.setRegisterPin(A, LOW);
      shift_register.setRegisterPin(B, LOW);
      shift_register.setRegisterPin(G, LOW);
      shift_register.setRegisterPin(C, LOW);
      shift_register.setRegisterPin(D, LOW);
      break;
            
    case '4':
      shift_register.setRegisterPin(F, LOW);
      shift_register.setRegisterPin(B, LOW);
      shift_register.setRegisterPin(G, LOW);
      shift_register.setRegisterPin(C, LOW);
      break;
            
    case '5':
    Serial.println("FIVE");
      shift_register.setRegisterPin(A, LOW);
      shift_register.setRegisterPin(F, LOW);
      shift_register.setRegisterPin(G, LOW);
      shift_register.setRegisterPin(C, LOW);
      shift_register.setRegisterPin(D, LOW);
      break;
            
    case '6':
    Serial.println("SIX");
      shift_register.setRegisterPin(A, LOW);
      shift_register.setRegisterPin(F, LOW);
      shift_register.setRegisterPin(G, LOW);
      shift_register.setRegisterPin(C, LOW);
      shift_register.setRegisterPin(D, LOW);
      shift_register.setRegisterPin(E, LOW);
      break;

    case '7':
      shift_register.setRegisterPin(A, LOW);
      shift_register.setRegisterPin(B, LOW);
      shift_register.setRegisterPin(C, LOW);
      break;
            
    case '8':
      shift_register.setRegisterPin(A, LOW);
      shift_register.setRegisterPin(B, LOW);
      shift_register.setRegisterPin(C, LOW);
      shift_register.setRegisterPin(D, LOW);
      shift_register.setRegisterPin(E, LOW);
      shift_register.setRegisterPin(F, LOW);
      shift_register.setRegisterPin(G, LOW);
      break;

    case '9':
      shift_register.setRegisterPin(A, LOW);
      shift_register.setRegisterPin(F, LOW);
      shift_register.setRegisterPin(B, LOW);
      shift_register.setRegisterPin(G, LOW);
      shift_register.setRegisterPin(C, LOW);
      break;
            
    case 'E':
      shift_register.setRegisterPin(A, LOW);
      shift_register.setRegisterPin(F, LOW);
      shift_register.setRegisterPin(G, LOW);
      shift_register.setRegisterPin(E, LOW);
      shift_register.setRegisterPin(D, LOW);
      break;
    
    case ' ':
    default:
    break;
  }
  Serial.println("writeRegisters()");
  shift_register.writeRegisters();
}

// 0 = G
// 1 = F
// 2 = A
// 3 = B
// 4 = DP
// 5 = C
// 6 = D
// 7 = E

char noteNames[] = {'C', 'D', 'E', 'F', 'G', 'a', 'b'};
//unsigned int frequencies[] = {262, 294, 330, 349, 392, 440, 494}; // from C4
unsigned int frequencies[] = {131, 147, 156, 175, 196, 220, 245};
const byte noteCount = sizeof(noteNames);
char score[] = "GGGEbGEbG";
float beats[]  = {1, 1, 1, 0.5, 0.5, 1, 0.5, 0.5, 1};
const byte scoreLen = sizeof(score);

void playTheme()
{
  int duration = 333;
  int pause = duration/2;
  for(int i = 0; i < scoreLen; i++) {
    playNote(score[i], duration * beats[i]);
    delay(pause*beats[i]);
  }
}

void playNote(char note, int duration)
{
  for(int i = 0; i < noteCount; i++) {
    if(noteNames[i] == note) {
      tone(speakerPin, frequencies[i], duration);
      break;
    }
  }
  delay(duration);
}

