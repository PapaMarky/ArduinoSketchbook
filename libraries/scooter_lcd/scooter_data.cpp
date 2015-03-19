#include <SoftwareSerial.h>
#include <Arduino.h>

#include "scooter_data.h"

void ScooterData::begin() {
  pinMode(_reset, OUTPUT);
  _disk->begin(DISK_BAUD_RATE);

  digitalWrite(_reset, LOW);
  digitalWrite(_reset, HIGH);
}

bool ScooterData::start_command_mode() {
  // Send Three Control-Z's
  _disk->write(26);
  _disk->write(26);
  _disk->write(26);

  // Wait for '>' prompt
  // TODO: Add Timeout and/or make async via loop
  while(1)
    if(_disk->available())
      if(_disk->read() == '>')
	return true;
}

void ScooterData::loop() {
}
