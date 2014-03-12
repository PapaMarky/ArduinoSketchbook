#ifndef DHT22_H
#define DHT22_H

enum DHT22_Err_t {
  DHT22_ERR_NONE = 0,
  DHT22_ERR_HUNG = 1,
  DHT22_ERR_SYNC = 2,
  DHT22_ERR_CHKSUM = 3,
};

DHT22_Err_t getDHT22(int pin, float *temperature, float *humidity);

#endif
