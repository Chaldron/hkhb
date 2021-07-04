#include "control.h"

#include <Arduino.h>
#include <ArduinoLog.h>

bool active;
uint8_t speed;
uint8_t last_active_speed;

void init_control() {
  // Initialize active and speed
  set_active("0");
  set_speed("0");
  last_active_speed = 0;
  // Initialize all pins
  for (size_t i = 0; i < sizeof(pins) / sizeof(pins[0]); i++) {
    Log.info("Initializing pin %d to high" CR, pins[i]);
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
  // Recover previous speed on becoming active
  if (active && !speed) {
    Log.info("Recovering last active speed %d" CR, last_active_speed);
    return set_speed(String(last_active_speed));
  }
  // Set speed to 0 if we're going inactive
  if (!active) return set_speed("0");
  return 200;
}

int set_speed(String s) {
  Log.info("Setting rotation speed to %s" CR, s.c_str());
  uint8_t new_speed = s.toInt();
  // Hold last active speed if we're turning off
  if (!new_speed) {
    last_active_speed = speed;
    Log.info("Last active speed is %d" CR, last_active_speed);
  }
  speed = new_speed;
  if (speed > 3) speed = 3;
  Log.info("Effective rotation speed is %d" CR, speed);

  Log.info("Writing pin %d low" CR, pins[speed]);
  digitalWrite(pins[speed], LOW);
  delay(IR_CONTROL_TIME);
  digitalWrite(pins[speed], HIGH);
  Log.info("Writing pin %d high" CR, pins[speed]);

  // Set inactive/active if needed
  if (!speed && active) return set_active("0");
  if (speed && !active) return set_active("1");

  return 200;
}
