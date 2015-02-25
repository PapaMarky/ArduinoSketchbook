#ifndef SCOOTER_DATA_H
#define SCOOTER_DATA_H
// Copyright 2015 Mark Dyer
/*
 * Data base for Scooter Time Trials
 */
class SoftwareSerial;

#define DISK_BAUD_RATE 9600
class ScooterData {
 public:
 ScooterData(int reset, SoftwareSerial* disk) : _reset(reset), _disk(disk) {}
  void begin();
  void loop();

  bool start_command_mode();
  bool is_ready() {return _ready;}

 private:
  int _reset;
  SoftwareSerial* _disk;
  bool _ready;
};

#endif // SCOOTER_DATA_H
