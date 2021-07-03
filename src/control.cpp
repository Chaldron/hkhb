#include "control.h"

#include <Arduino.h>
#include <ArduinoLog.h>

bool active;
uint8_t speed;

void init_control() {
  active = false;
  speed = 0;
  for (size_t i = 0; i < sizeof(pins) / sizeof(pins[0]); i++) {
    Log.info("Initializing pin %d" CR, pins[i]);
    pinMode(pins[i], OUTPUT);
    digitalWrite(pins[i], HIGH);
  }
  Log.info("Initialized fan control" CR);
}

int identify(String _) {
  Log.info("Identifying" CR);
  digitalWrite(PIN_LED, LOW);
  delay(500);
  digitalWrite(PIN_LED, HIGH);
}

int set_active(String a) {
  Log.info("Setting active to %s" CR, a.c_str());
  active = a.toInt();
  Log.info("Active is now %d" CR, active);
  return 200;
}

int set_rotation_speed(String s) {
  Log.info("Setting rotation speed to %s" CR, s.c_str());
  speed = s.toInt();
  if (speed > 3) speed = 3;
  Log.info("Effective rotation speed is %d" CR, speed);

  Log.info("Writing pin %d low" CR, pins[speed]);
  digitalWrite(pins[speed], LOW);
  delay(IR_CONTROL_TIME);
  digitalWrite(pins[speed], HIGH);
  Log.info("Writing pin %d high" CR, pins[speed]);

  return 200;
}
