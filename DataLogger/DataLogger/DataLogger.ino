#include <SD.h>
#include <Wire.h>

#include <RTClib.h>

#include <SPI.h>
#include <LiquidCrystal.h>
//#include <DHT22.h>

/*** 
 * PIN USAGE
 * SD CARD: 10, 11, 12, 13
 * RTC:     A4, A5
 * LCD:     2, 3, 4, 5, 11, 13
 * Sensors: 
 */
 
/*
  SD card datalogger
 
 This example shows how to log data from three analog sensors 
 to an SD card using the SD library.
 	
 The circuit:
 * SD card attached to SPI bus as follows:
 ** UNO:  MOSI - pin 11, MISO - pin 12, CLK - pin 13, CS - pin 4 (CS pin can be changed)
  and pin #10 (SS) must be an output
 ** Mega:  MOSI - pin 51, MISO - pin 50, CLK - pin 52, CS - pin 4 (CS pin can be changed)
  and pin #52 (SS) must be an output
 ** Leonardo: Connect to hardware SPI via the ICSP header
 		Pin 4 used here for consistency with other Arduino examples
 
 created  24 Nov 2010
 modified 9 Apr 2012 by Tom Igoe
 
 This example code is in the public domain.
 	 
 */
// Big lookup Table (approx 750 entries), subtract 238 from ADC reading to start at 0*C. Entries in 10ths of degree i.e. 242 = 24.2*C Covers 0*C to 150*C For 10k resistor/10k thermistor voltage divider w/ therm on the + side.
const int temps[] PROGMEM = { 0, 1, 2, 3, 4, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 143, 144, 145, 146, 147, 148, 149, 150, 151, 151, 152, 153, 154, 155, 156, 157, 158, 159, 159, 160, 161, 162, 163, 164, 165, 166, 167, 167, 168, 169, 170, 171, 172, 173, 174, 175, 175, 176, 177, 178, 179, 180, 181, 182, 182, 183, 184, 185, 186, 187, 188, 189, 190, 190, 191, 192, 193, 194, 195, 196, 197, 197, 198, 199, 200, 201, 202, 203, 204, 205, 205, 206, 207, 208, 209, 210, 211, 212, 212, 213, 214, 215, 216, 217, 218, 219, 220, 220, 221, 222, 223, 224, 225, 226, 227, 228, 228, 229, 230, 231, 232, 233, 234, 235, 235, 236, 237, 238, 239, 240, 241, 242, 243, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 252, 253, 254, 255, 256, 257, 258, 259, 260, 260, 261, 262, 263, 264, 265, 266, 267, 268, 269, 269, 270, 271, 272, 273, 274, 275, 276, 277, 278, 279, 279, 280, 281, 282, 283, 284, 285, 286, 287, 288, 289, 289, 290, 291, 292, 293, 294, 295, 296, 297, 298, 299, 300, 301, 301, 302, 303, 304, 305, 306, 307, 308, 309, 310, 311, 312, 313, 314, 315, 315, 316, 317, 318, 319, 320, 321, 322, 323, 324, 325, 326, 327, 328, 329, 330, 331, 332, 333, 334, 335, 335, 336, 337, 338, 339, 340, 341, 342, 343, 344, 345, 346, 347, 348, 349, 350, 351, 352, 353, 354, 355, 356, 357, 358, 359, 360, 361, 362, 363, 364, 365, 366, 367, 368, 369, 370, 371, 372, 373, 374, 375, 376, 377, 378, 379, 380, 381, 382, 383, 384, 385, 386, 387, 388, 389, 390, 392, 393, 394, 395, 396, 397, 398, 399, 400, 401, 402, 403, 404, 405, 406, 407, 408, 410, 411, 412, 413, 414, 415, 416, 417, 418, 419, 420, 422, 423, 424, 425, 426, 427, 428, 429, 430, 432, 433, 434, 435, 436, 437, 438, 439, 441, 442, 443, 444, 445, 446, 448, 449, 450, 451, 452, 453, 455, 456, 457, 458, 459, 460, 462, 463, 464, 465, 466, 468, 469, 470, 471, 472, 474, 475, 476, 477, 479, 480, 481, 482, 484, 485, 486, 487, 489, 490, 491, 492, 494, 495, 496, 498, 499, 500, 501, 503, 504, 505, 507, 508, 509, 511, 512, 513, 515, 516, 517, 519, 520, 521, 523, 524, 525, 527, 528, 530, 531, 532, 534, 535, 537, 538, 539, 541, 542, 544, 545, 547, 548, 550, 551, 552, 554, 555, 557, 558, 560, 561, 563, 564, 566, 567, 569, 570, 572, 574, 575, 577, 578, 580, 581, 583, 585, 586, 588, 589, 591, 593, 594, 596, 598, 599, 601, 603, 604, 606, 608, 609, 611, 613, 614, 616, 618, 620, 621, 623, 625, 627, 628, 630, 632, 634, 636, 638, 639, 641, 643, 645, 647, 649, 651, 653, 654, 656, 658, 660, 662, 664, 666, 668, 670, 672, 674, 676, 678, 680, 683, 685, 687, 689, 691, 693, 695, 697, 700, 702, 704, 706, 708, 711, 713, 715, 718, 720, 722, 725, 727, 729, 732, 734, 737, 739, 741, 744, 746, 749, 752, 754, 757, 759, 762, 764, 767, 770, 773, 775, 778, 781, 784, 786, 789, 792, 795, 798, 801, 804, 807, 810, 813, 816, 819, 822, 825, 829, 832, 835, 838, 842, 845, 848, 852, 855, 859, 862, 866, 869, 873, 877, 881, 884, 888, 892, 896, 900, 904, 908, 912, 916, 920, 925, 929, 933, 938, 942, 947, 952, 956, 961, 966, 971, 976, 981, 986, 991, 997, 1002, 1007, 1013, 1019, 1024, 1030, 1036, 1042, 1049, 1055, 1061, 1068, 1075, 1082, 1088, 1096, 1103, 1110, 1118, 1126, 1134, 1142, 1150, 1159, 1168, 1177, 1186, 1196, 1206, 1216, 1226, 1237, 1248, 1260, 1272, 1284, 1297, 1310, 1324, 1338, 1353, 1369, 1385, 1402, 1420, 1439, 1459, 1480, 1502 };

// On the Ethernet Shield, CS is pin 4. Note that even if it's not
// used as the CS pin, the hardware CS pin (10 on most Arduino boards,
// 53 on the Mega) must be left as an output or the SD library
// functions will not work.
const int chipSelect = 10;

File dataFile;
RTC_DS1307 rtc;
//LiquidCrystal lcd(7, 6, 5, A6, 3, 2);
const int sensor1_pin = A6;
const int sensor2_pin = A5;

bool lcd_online = false;

void setupLCD() {
  return;
  /*
  lcd.begin(16, 2);
  lcd_online = true;
  display("Data Logger :", "");
  */
}

DateTime timestamp;
void GetTime() {
  timestamp = rtc.now();
}

String CreateString(int v, int width = 2) {
  String rval(v, DEC);
  while (rval.length() < width) {
    rval = String("0") + rval;
  }
  return rval;
}

void display(String r1, String r2) {
  if (!lcd_online)
    return;
    /*
  lcd.clear();
  lcd.print(r1);
  lcd.setCursor(0,1);
  lcd.print(r2);
  */
}

void setupRTClock() {
  Wire.begin();
  rtc.begin();
  if (! rtc.isrunning()) {
    display("Data Logger :", "Setting RTC");
    Serial.println("Setting RTC!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(__DATE__, __TIME__));
    delay(500);
  }
}

void setupSD() {
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  display("Data Logger :", "Init SD card...");
  Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  Serial.println(SS);
  pinMode(SS, OUTPUT);
  delay(1000);
  
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    display("Data Logger :", "SD init failed");
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1) delay(5000);
  }
  display("Data Logger :", "card initialized.");
  Serial.println("card initialized.");
  delay(1000);
}

void setupSensors() {
  display("Data Logger :", "set up sensors...");
  delay(2000);
}

void OpenDataFile() {
  Serial.println("OpenDataFile");
  String filename("data");
  char buffer[128];
  /*
  GetTime();
  filename += CreateString(timestamp.year());
  filename += CreateString(timestamp.month());
  filename += CreateString(timestamp.day());
  filename += CreateString(timestamp.hour());
  filename += CreateString(timestamp.minute());
  filename += CreateString(timestamp.second());
  */
  filename += ".txt";
  filename = "datalog.txt";
  filename.toCharArray(buffer, 128);
  // Open up the file we're going to log to!
  dataFile = SD.open("datalog.txt", FILE_WRITE);
  Serial.print("Opening file: ");
  Serial.println(buffer);
  if (! dataFile) {
    display("Data Logger :", "file open failed");
    Serial.print("error opening ");
    Serial.println(buffer);
    // Wait forever since we cant write data
    while (1) delay(5000);
  }
  display("Data Logger :", "File Opened");
  delay(1000);
  Serial.println("File Opened");
}

void setup()
{
 // Open serial communications and wait for port to open:
  Serial.begin(57600);

  setupLCD();
  setupRTClock();
  setupSD();
  OpenDataFile();
  setupSensors();
}

void makeTimeString(DateTime& dt, String& s) {
  int hour = dt.hour();
  //while(hour > 12)
  //  hour -= 12;
  s += CreateString(hour);
  s += String(':');
  s += CreateString(dt.minute());
  s += String(':');
  s += CreateString(dt.second());
}

void makeDateString(DateTime& dt, String& s) {
  String p;
  int year = dt.year();
  s = CreateString(year);
  s += String('/');
  s += CreateString(dt.month());
  s += String('/');
  s += CreateString(dt.day());
}

void makeTimestampString(DateTime& dt, String& s) {
  String timestr;
  String datestr;
  s = String("");
  makeDateString(dt, datestr);
  s += timestr;
  s += String(' ');
  makeTimeString(dt, timestr);
  s += timestr;

  Serial.println(s);
}

String dateString;
String timeString;
String timestampString;
String record;
String TempStr[2];
String HumStr[2];
float hum[2];
float temp[2];

char tmp[100];

void ReadSensor(int n) {
  int therm;
  int pin = n ? sensor2_pin : sensor1_pin;
  therm=analogRead(pin)-238;
  therm=pgm_read_word(&temps[therm]);  
  temp[n] = therm;
  hum[n] = 0;
  /*
  DHT22_Err_t result;
  
  int pin = n ? sensor2_pin : sensor1_pin;
  
  result = getDHT22(pin, &temp[n], &hum[n]);  
  if (result != DHT22_ERR_NONE)
  {
    delay(5000);
    return;
  }
  */
}
void ReadSensor1() {
  ReadSensor(0);
}
void ReadSensor2() {
  ReadSensor(1);
}

void makeRecordString(DateTime& dt, String& s) {
  char buffer[16];
  s = dateString;
  s += " ";
  s += timeString;
  s += ",";

  s += makeFloatString(temp[0]);
  s += ",";
  s += makeFloatString(hum[0]);
  s += ",";
  s += makeFloatString(temp[1]);
  s += ",";
  s += makeFloatString(hum[1]);
}

void WriteData() {
  makeRecordString(timestamp, record);
  Serial.println(record);
  dataFile.println(record);
  dataFile.flush();
}

void UpdateDisplay() {
  if (!lcd_online)
    return;
  String line1;
  String line2;
  line1 += timeString;
  line1 += "  H: ";
  line1 += String(round(hum[0]));
  line1 += "%";
  
  line2 += "Pot:";
  line2 += String(round(temp[1]));
  line2 += "C  Air:";
  line2 += String(round(temp[0]));
  line2 += "C";
  display(line1, line2);
}

char* makeFloatString(float v) {
  static char buffer[16];
  dtostrf(v, 4, 1, buffer);
  return buffer;
}

void loop()
{
  GetTime();
  timestampString = "";
  dateString = "";
  timeString = "";
  makeDateString(timestamp, dateString);
  makeTimeString(timestamp, timeString);
  ReadSensor1();
  ReadSensor2();
  WriteData();
  UpdateDisplay();
  // Take 1 measurement every 5 seconds
  delay(5000);

}









