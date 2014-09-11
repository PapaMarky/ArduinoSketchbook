
// States
#define ST_POWER_UP 1
#define ST_LASER_READY 2
#define ST_READY 3

// Events
#define EV_BUTTON 1
#define EV_LASER_AQUIRED 2
#define EV_LASER_LOST 3

struct Transition {
  int state;
  int event;
  int next_state;
};

Transition TRANSITIONS [] = {
  {ST_POWER_UP, EV_LASER_AQUIRED, ST_LASER_READY},
  {ST_LASER_READY, EV_LASER_LOST, ST_POWER_UP},
  {ST_LASER_READY, EV_BUTTON, ST_READY}
};
