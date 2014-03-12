#include "DHT22.h"
#include <Arduino.h>

static byte _read_dht22_dat(volatile uint8_t *in, uint8_t bitmask)
{
  byte i = 0;
  byte result=0;
  for(i=0; i< 8; i++){
    while(!(*in & bitmask));  // wait for 50us
    delayMicroseconds(30);
 
    if(*in & bitmask) 
      result |=(1<<(7-i));
    while((*in & bitmask));  // wait '1' finish
  }
  return result;
}

DHT22_Err_t getDHT22(int pin, float *temperature, float *humidity)
{
  uint8_t dht22_dat[5];
  uint8_t dht22_in;
  uint8_t i;

  uint8_t bitmask = digitalPinToBitMask(pin);
  volatile uint8_t port = digitalPinToPort(pin);
  volatile uint8_t *reg, *out, *in;

  // setup registers and pins
  reg = portModeRegister(port);
  out = portOutputRegister(port);
  in =  portInputRegister(port);
  *reg |= bitmask;
  *out |= bitmask;

  // start condition
  // 1. pull-down i/o pin from 18ms
  *out &= ~bitmask;
  delay(18);
  *out |= bitmask;
  delayMicroseconds(40);
 
  *reg &= ~bitmask;
  delayMicroseconds(40);
 
  dht22_in = *in & bitmask;
 
  if(dht22_in){
    Serial.println("dht22 start condition 1 not met");
    Serial.print(" - pin: ");
    Serial.println(pin);
    return DHT22_ERR_HUNG;
  }
  delayMicroseconds(80);
 
  dht22_in = *in & bitmask;
 
  if(!dht22_in)
    return DHT22_ERR_SYNC;

  delayMicroseconds(80);
  // now ready for data reception
  for (i=0; i<5; i++)
    dht22_dat[i] = _read_dht22_dat(in, bitmask);
 
  *reg |= bitmask;
  *out |= bitmask;
 
  byte dht22_check_sum = dht22_dat[0]+dht22_dat[1]+dht22_dat[2]+dht22_dat[3];
  // check check_sum
  if(dht22_dat[4] != dht22_check_sum)
    return DHT22_ERR_CHKSUM;
  
  int h = dht22_dat[0] << 8 | dht22_dat[1];
  *humidity = (float)h/10.0;
  int t = dht22_dat[2] << 8 | dht22_dat[3];
  *temperature = (float)t/10.0;
 
  return DHT22_ERR_NONE; 
}
