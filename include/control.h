#pragma once

#include <Arduino.h>

// Control pins for the board LED + each fan control button
#define PIN_LED 0
#define PIN_OFF 15
#define PIN_LOW 14
#define PIN_MEDIUM 13
#define PIN_HIGH 12
static const int pins[] = {PIN_OFF, PIN_LOW, PIN_MEDIUM, PIN_HIGH};

// Time to spend with the IR control LED actively transmiting, in ms
#define IR_CONTROL_TIME 300

// Initialization
void init_control();

// HomeKit fan functions - identify, active, rotation speed
int identify(String _);
int set_active(String);
int set_speed(String);
