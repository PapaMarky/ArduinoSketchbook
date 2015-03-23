#ifndef GLOBALS_H
#define GLOBALS_H

class StateMachine;
class LaserAssembly;
class LcdComponent;
class ButtonComponent;

extern LaserAssembly* g_laser;
extern LcdComponent* g_lcd;
extern ButtonComponent* g_go_button;
extern StateMachine *g_stateMachine;

#endif /// GLOBALS_H
