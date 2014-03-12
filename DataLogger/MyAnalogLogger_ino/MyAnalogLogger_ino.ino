// A simple data logger for the Arduino analog pins with optional DS1307
// uses RTClib from https://github.com/adafruit/RTClib
#include <SdFat.h>
#include <SdFatUtil.h>  // define FreeRam()

#define SD_CHIP_SELECT  SS  // SD chip select pin
#define USE_DS1307       1  // set nonzero to use DS1307 RTC
#define LOG_INTERVAL  60000  // mills between entries
#define SENSOR_COUNT     2  // number of analog pins to log
#define ECHO_TO_SERIAL   1  // echo data to serial port if nonzero
#define WAIT_TO_START    0  // Wait for serial input in setup()
#define ADC_DELAY       10  // ADC delay for high impedence sensors

// file system object
SdFat sd;

// text file for logging
ofstream logfile;

// Serial print stream
ArduinoOutStream cout(Serial);

// buffer to format data - makes it eaiser to echo to Serial
char buf[80];
//------------------------------------------------------------------------------
#if SENSOR_COUNT > 6
#error SENSOR_COUNT too large
#endif  // SENSOR_COUNT
//------------------------------------------------------------------------------
// store error strings in flash to save RAM
#define error(s) sd.errorHalt_P(PSTR(s))
//------------------------------------------------------------------------------
#if USE_DS1307
// use RTClib from Adafruit
// https://github.com/adafruit/RTClib

// The Arduino IDE has a bug that causes Wire and RTClib to be loaded even
// if USE_DS1307 is false.

//#error remove this line and uncomment the next two lines.
#include <Wire.h>
#include <RTClib.h>
RTC_DS1307 RTC;  // define the Real Time Clock object
//------------------------------------------------------------------------------
// call back for file timestamps
void dateTime(uint16_t* date, uint16_t* time) {
    DateTime now = RTC.now();

  // return date using FAT_DATE macro to format fields
  *date = FAT_DATE(now.year(), now.month(), now.day());

  // return time using FAT_TIME macro to format fields
  *time = FAT_TIME(now.hour(), now.minute(), now.second());
}
//------------------------------------------------------------------------------
// format date/time
ostream& operator << (ostream& os, DateTime& dt) {
  os << dt.year() << '/' << int(dt.month()) << '/' << int(dt.day()) << ' ';
  os << int(dt.hour()) << ':' << setfill('0') << setw(2) << int(dt.minute());
  os << ':' << setw(2) << int(dt.second()) << setfill(' ');
  return os;
}
#endif  // USE_DS1307
//------------------------------------------------------------------------------

////////////////////////////// Marky Code //////////////////////////////////////
#include <DHT22.h>

#include <LiquidCrystal.h>

int nSamples = 0;
String fileName("FILE ERROR");

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
bool lcd_online = false;
char disp_buf0[32];
char disp_buf1[32];
float hum[2];
float temp[2];
void display(String r1, String r2) {
  if (!lcd_online)
    return;
  lcd.clear();
  lcd.print(r1);
  lcd.setCursor(0,1);
  lcd.print(r2);
}

char STRING_BUFFER[64];
ostream& operator << (ostream& os, String& str) {
  str.toCharArray(STRING_BUFFER, 64);
  os << STRING_BUFFER;
  return os;
}

void setupLCD() {
  Serial.println("setupLCD");
  lcd.begin(16, 2);
  delay(10);
  lcd_online = true;
  display("Data Logger", "Start Up");
}

ostream& UpdateLcd(ostream& line1, ostream& line2, DateTime& dt) {
  int hour = dt.hour();
  if (hour == 0) 
    hour = 12;
  else while (hour > 12)
    hour -= 12;
    
  const char* ampm = (hour >= 12 ? " AM" : " PM");
  line1 << setfill('0') << setw(2) << hour << ':' << int(dt.minute());
  line1 << ampm;
  //line1 << ':' << setw(2) << int(dt.second());
  line1 << "  " << setprecision(0) << hum[0] << "% RH";
  line2 << setprecision(0) << "Pot:" << temp[1] << "C  Air:" << temp[0] << 'C';
  display(disp_buf0, disp_buf1);
  return line1;
}

ostream& UpdateLcd2(ostream& line1, ostream& line2) {
  line1 << fileName;
  line2 << nSamples << " Sample";
  if (nSamples != 1)
    line2 << 's';
  
  display(disp_buf0, disp_buf1);
  return line1;
}

////////////////////////// DHT22 ///////////////////////////
const int sensor_pin[2] = {A0, A1};

void setupSensors() {
  display("Data Logger", "set up sensors...");
  delay(2000);
}

bool ReadSensor(int n) {
  DHT22_Err_t result = getDHT22(sensor_pin[n], &temp[n], &hum[n]);  
  if (result != DHT22_ERR_NONE) {
    display("Sensor Error", String(result));
    delay(5000);
    temp[n] = hum[n] = NAN;
    return false;
  }
  return true;
}

bool started = false;
////////////////////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(9600);
  while (!Serial){}  // wait for Leonardo
  
  setupLCD();
  // pstr stores strings in flash to save RAM
  cout << endl << pstr("FreeRam: ") << FreeRam() << endl;

#if WAIT_TO_START
  cout << pstr("Type any character to start\n");
  while (Serial.read() <= 0) {}
  delay(400);  // catch Due reset problem
#endif  // WAIT_TO_START

#if USE_DS1307
  // connect to RTC
  Wire.begin();
  if (!RTC.begin()) error("RTC failed");

#if RESET_TIMER 
  RTC.adjust(DateTime(__DATE__, __TIME__));
  delay(500);
#endif

  // set date time callback function
  SdFile::dateTimeCallback(dateTime);
  DateTime now = RTC.now();
  cout  << now << endl;
#endif  // USE_DS1307

  // initialize the SD card at SPI_HALF_SPEED to avoid bus errors with
  if (!sd.begin(SD_CHIP_SELECT, SPI_HALF_SPEED)) sd.initErrorHalt();

  // create a new file in root, the current working directory
  char name[] = "LOGGER00.CSV";

  for (uint8_t i = 0; i < 100; i++) {
    name[6] = i/10 + '0';
    name[7] = i%10 + '0';
    if (sd.exists(name)) continue;
    logfile.open(name);
    break;
  }
  if (!logfile.is_open()) error("file.open");
  fileName = name;
  Serial.print("fileName: '");
  Serial.print(fileName);
  Serial.println("'");
  display("Data Logger", name);
  delay(10000);  // wait 10 secs to let user see file name
  cout << pstr("Logging to: ") << name << endl;
  cout << pstr("Type any character to stop\n\n");

  // format header in buffer
  obufstream bout(buf, sizeof(buf));
#if 0
  bout << pstr("millis");
#endif //0

#if USE_DS1307
  bout << pstr("timestamp");
#endif  // USE_DS1307

#if 0
  for (uint8_t i = 0; i < SENSOR_COUNT; i++) {
    bout << pstr(",sens") << int(i);
  }
#else
  bout << pstr(",air temp,air rh,pot temp,pot rh");
#endif // 0
  
  logfile << buf << endl;

#if ECHO_TO_SERIAL
  cout << buf << endl;
#endif  // ECHO_TO_SERIAL
}
//------------------------------------------------------------------------------
void loop() {
  uint32_t m;
  byte disp_version = 3;
  // wait for time to be a multiple of interval
  if (started) {
    do {
      m = millis();
      // pump the LCD display between the two formats
      byte v = (m/5000)%2;
      
      if (v != disp_version) {
        disp_version = v;
        obufstream dd0out(disp_buf0, sizeof(buf));
        obufstream dd1out(disp_buf1, sizeof(buf));
        if (disp_version) {
          DateTime now = RTC.now();
          UpdateLcd(dd0out, dd1out, now);
        }
        else
          UpdateLcd2(dd0out, dd1out);
      }
      
    } while (m % LOG_INTERVAL);
  }
  else
    started = true;

  // use buffer stream to format line
  obufstream bout(buf, sizeof(buf));
  obufstream d0out(disp_buf0, sizeof(buf));
  obufstream d1out(disp_buf1, sizeof(buf));

#if 0
  // start with time in millis
  bout << m;
#endif // 0

#if USE_DS1307
  DateTime now = RTC.now();
  //bout << ',' << now;
  bout << now;
#endif  // USE_DS1307

  // read analog pins and format data
  for (uint8_t ia = 0; ia < SENSOR_COUNT; ia++) {
#if 0
#if ADC_DELAY
    analogRead(ia);
    delay(ADC_DELAY);
#endif  // ADC_DELAY
    bout << ',' << analogRead(ia);
#endif // 0
    ReadSensor(ia);
    bout << ',' << temp[ia];
    bout << ',' << hum[ia];
  }
    
  bout << endl;
  // log data and flush to SD
  logfile << buf << flush;
  nSamples++;

  // check for error
  if (!logfile) error("write data failed");

#if ECHO_TO_SERIAL
  cout << buf;
#endif  // ECHO_TO_SERIAL
  UpdateLcd(d0out, d1out, now);

  // don't log two points in the same millis
  if (m == millis()) delay(1);
  
  if (!Serial.available()) return;
  logfile.close();
  cout << pstr("Done!");
  display("Data Logger", "Done!");
  while (1);
}
