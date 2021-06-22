#include "callback.h"

#include "control.h"

// HomeKit characteristics we're handling the callbacks for
extern homekit_characteristic_t ch_active;
extern homekit_characteristic_t ch_speed;

// Ideally we'd maybe flash the lights here or something,
// but for our use case we can just blink the LED on the baord
void identify(homekit_value_t value) { blink_led(1000); }

// Notify to all devices the current status of our device
void notify() {
  homekit_characteristic_notify(&ch_active, ch_active.value);
  homekit_characteristic_notify(&ch_speed, ch_speed.value);
}

// Get/set active status
homekit_value_t active_get() { return ch_active.value; }
void active_set(homekit_value_t value) {
  // early exit
  if (value.bool_value == ch_active.value.bool_value) return;

  // update value
  ch_active.value = value;

  // turn off if we're going inactive
  if (!value.bool_value) {
    speed_set(HOMEKIT_FLOAT(0));
  }
  // set speed to lowest if the stored speed was 0
  else if (!((uint8_t)ch_speed.value.int_value)) {
    speed_set(HOMEKIT_FLOAT(1));
  }
  // otherwise just set the speed to the last remembered value
  else {
    speed_set(ch_speed.value);
  }

  // notify and blink
  notify();
  blink_led(100);
}

// Get/set speed
homekit_value_t speed_get() { return ch_speed.value; }
void speed_set(homekit_value_t value) {
  // update value
  ch_speed.value = value;

  // turn fan off, or to low/medium/high
  // "fun" with function pointers
  static const void (*rc_map[4])() = {rc_off, rc_low, rc_medium, rc_high};
  uint8_t target_speed = (uint8_t)value.float_value;
  if (target_speed >= 0 && target_speed <= 3) rc_map[target_speed]();

  // notify and blink
  notify();
  blink_led(100);
}