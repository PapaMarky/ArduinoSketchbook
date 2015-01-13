#include <Adafruit_NeoPixel.h>

#define PIN1 6
#define L1 126
#define L2 162

float pct1[] = {
  0.0000, 0.0080, 0.0160, 0.0240, 0.0320, 0.0400, 0.0480, 0.0560, 0.0640, 0.0720, 
  0.0800, 0.0880, 0.0960, 0.1040, 0.1120, 0.1200, 0.1280, 0.1360, 0.1440, 0.1520, 
  0.1600, 0.1680, 0.1760, 0.1840, 0.1920, 0.2000, 0.2080, 0.2160, 0.2240, 0.2320, 
  0.2400, 0.2480, 0.2560, 0.2640, 0.2720, 0.2800, 0.2880, 0.2960, 0.3040, 0.3120, 
  0.3200, 0.3280, 0.3360, 0.3440, 0.3520, 0.3600, 0.3680, 0.3760, 0.3840, 0.3920, 
  0.4000, 0.4080, 0.4160, 0.4240, 0.4320, 0.4400, 0.4480, 0.4560, 0.4640, 0.4720, 
  0.4800, 0.4880, 0.4960, 0.5040, 0.5120, 0.5200, 0.5280, 0.5360, 0.5440, 0.5520, 
  0.5600, 0.5680, 0.5760, 0.5840, 0.5920, 0.6000, 0.6080, 0.6160, 0.6240, 0.6320, 
  0.6400, 0.6480, 0.6560, 0.6640, 0.6720, 0.6800, 0.6880, 0.6960, 0.7040, 0.7120, 
  0.7200, 0.7280, 0.7360, 0.7440, 0.7520, 0.7600, 0.7680, 0.7760, 0.7840, 0.7920, 
  0.8000, 0.8080, 0.8160, 0.8240, 0.8320, 0.8400, 0.8480, 0.8560, 0.8640, 0.8720, 
  0.8800, 0.8880, 0.8960, 0.9040, 0.9120, 0.9200, 0.9280, 0.9360, 0.9440, 0.9520, 
  0.9600, 0.9680, 0.9760, 0.9840, 0.9920, 1.0000, 
};

float pct2[] = {
  0.0000, 0.0062, 0.0124, 0.0186, 0.0248, 0.0311, 0.0373, 0.0435, 0.0497, 0.0559, 
  0.0621, 0.0683, 0.0745, 0.0807, 0.0870, 0.0932, 0.0994, 0.1056, 0.1118, 0.1180, 
  0.1242, 0.1304, 0.1366, 0.1429, 0.1491, 0.1553, 0.1615, 0.1677, 0.1739, 0.1801, 
  0.1863, 0.1925, 0.1988, 0.2050, 0.2112, 0.2174, 0.2236, 0.2298, 0.2360, 0.2422, 
  0.2484, 0.2547, 0.2609, 0.2671, 0.2733, 0.2795, 0.2857, 0.2919, 0.2981, 0.3043, 
  0.3106, 0.3168, 0.3230, 0.3292, 0.3354, 0.3416, 0.3478, 0.3540, 0.3602, 0.3665, 
  0.3727, 0.3789, 0.3851, 0.3913, 0.3975, 0.4037, 0.4099, 0.4161, 0.4224, 0.4286, 
  0.4348, 0.4410, 0.4472, 0.4534, 0.4596, 0.4658, 0.4720, 0.4783, 0.4845, 0.4907, 
  0.4969, 0.5031, 0.5093, 0.5155, 0.5217, 0.5280, 0.5342, 0.5404, 0.5466, 0.5528, 
  0.5590, 0.5652, 0.5714, 0.5776, 0.5839, 0.5901, 0.5963, 0.6025, 0.6087, 0.6149, 
  0.6211, 0.6273, 0.6335, 0.6398, 0.6460, 0.6522, 0.6584, 0.6646, 0.6708, 0.6770, 
  0.6832, 0.6894, 0.6957, 0.7019, 0.7081, 0.7143, 0.7205, 0.7267, 0.7329, 0.7391, 
  0.7453, 0.7516, 0.7578, 0.7640, 0.7702, 0.7764, 0.7826, 0.7888, 0.7950, 0.8012, 
  0.8075, 0.8137, 0.8199, 0.8261, 0.8323, 0.8385, 0.8447, 0.8509, 0.8571, 0.8634, 
  0.8696, 0.8758, 0.8820, 0.8882, 0.8944, 0.9006, 0.9068, 0.9130, 0.9193, 0.9255, 
  0.9317, 0.9379, 0.9441, 0.9503, 0.9565, 0.9627, 0.9689, 0.9752, 0.9814, 0.9876, 
  0.9938, 1.0000, 
};

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(L1 + L2, PIN1, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  strip1.begin();
  strip1.show(); // Initialize all pixels to 'off'
  
  Serial.begin(9600);
  
  int totalbytes = (L1 + L2) * sizeof(float) + (L1 + L2) * 3;
  Serial.print("NEED TO ALLOC "); Serial.print(totalbytes); Serial.println(" BYTES");
  Serial.println("start");
  // initialize pct arrays
  /*
  Serial.println("float pct1[] = {");
  int n = 0;
  Serial.print("  ");
  for (int i = 0; i < L1; i++) {
    Serial.print((float)i / (float)(L1 - 1), 4); Serial.print(", ");
    n++;
    if (n == 10) {
      Serial.println("");
      Serial.print("  ");
      n = 0;
    }
  }
  Serial.println("\n};");
  Serial.println("float pct2[] = {");
  n = 0;
  Serial.print("  ");
  for (int i = 0; i < L2; i++) {
    Serial.print((float)i / (float)(L2 - 1), 4); Serial.print(", ");
    n++;
    if (n == 10) {
      Serial.println("");
      Serial.print("  ");
      n = 0;
    }
  }
  Serial.println("\n};");
  Serial.println("pct2 done");
  */
}

void loop() {
  // Some example procedures showing how to display to the pixels:
  colorWipe(strip1.Color(255, 0, 0), 50); // Red
  colorWipe(strip1.Color(0, 255, 0), 50); // Green
  colorWipe(strip1.Color(0, 0, 255), 50); // Blue
  // Send a theater pixel chase in...
  theaterChase(strip1.Color(127, 127, 127), 50); // White
  theaterChase(strip1.Color(127,   0,   0), 50); // Red
  theaterChase(strip1.Color(  0,   0, 127), 50); // Blue

  rainbow(20);
  rainbowCycle(20);
  theaterChaseRainbow(50);
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  int j = 0;
  for(uint16_t i=0; i<L2; i++) {
      Serial.println(i);
      strip1.setPixelColor(L1 + i, c);
      /*
      if (i < L1) {
        strip1.setPixelColor(i, c);
        strip1.show();
      }
      */
      float pct = (float)i / (float)(L2 - 1);
      Serial.print(i); Serial.print(" / "); Serial.print(L2-1);Serial.print(", ");
      Serial.print("PCT: "); Serial.println(pct, 4);

      while (pct1[j] <= pct && j < L1) {
        strip1.setPixelColor(j, c);
        j++;
      }

      strip1.show();

      delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<L1; i++) {
      strip1.setPixelColor(i, Wheel((i+j) & 255));
    }
    for(i=0; i<L2; i++) {
      strip1.setPixelColor(L1 + i, Wheel((i+j) & 255));
    }
    strip1.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< L1; i++) {
      strip1.setPixelColor(i, Wheel(((i * 256 / L1) + j) & 255));
    }
    for(i=0; i< L2; i++) {
      strip1.setPixelColor(L1 + i, Wheel(((i * 256 / L2) + j) & 255));
    }
    strip1.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < L1; i=i+3) {
        strip1.setPixelColor(i+q, c);    //turn every third pixel on
      }
      for (int i=0; i < L2; i=i+3) {
        strip1.setPixelColor((L1 + i)+q, c);    //turn every third pixel on
      }
      strip1.show();
     
      delay(wait);
     
      for (int i=0; i < L1; i=i+3) {
        strip1.setPixelColor(i+q, 0);        //turn every third pixel off
      }
      for (int i=0; i < L2; i=i+3) {
        strip1.setPixelColor(L1+i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
        for (int i=0; i < L1; i=i+3) {
          strip1.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
        }
        for (int i=0; i < L2; i=i+3) {
          strip1.setPixelColor(L1+i+q, Wheel( (i+j) % 255));    //turn every third pixel on
        }
        strip1.show();
       
        delay(wait);
       
        for (int i=0; i < L1; i=i+3) {
          strip1.setPixelColor(i+q, 0);        //turn every third pixel off
        }
        for (int i=0; i < L2; i=i+3) {
          strip1.setPixelColor(L1+i+q, 0);        //turn every third pixel off
        }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip1.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip1.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip1.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

