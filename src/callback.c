#include "callback.h"

#include "control.h"

extern homekit_characteristic_t ch_active;
extern homekit_characteristic_t ch_speed;

void identify(homekit_value_t _value) { blink_led(1000); }

void notify() {
  homekit_characteristic_notify(&ch_active, ch_active.value);
  homekit_characteristic_notify(&ch_speed, ch_speed.value);
}

homekit_value_t active_get() { return ch_active.value; }
void active_set(homekit_value_t value) {
  // early exit
  if (value.bool_value == ch_active.value.bool_value) return;

  // update value
  ch_active.value = value;

  // turn off if we're going inactive
  if (!value.bool_value) rc_off();

  // notify and blink
  homekit_characteristic_notify(&ch_active, ch_active.value);
  blink_led(100);
}

homekit_value_t speed_get() { return ch_speed.value; }
void speed_set(homekit_value_t value) {
  // update value
  ch_speed.value = value;

  // turn fan off, or to low/medium/high
  switch ((uint8_t)value.float_value) {
    case 0:
      rc_off();
      break;
    case 1:
      rc_low();
      break;
    case 2:
      rc_medium();
      break;
    case 3:
      rc_high();
      break;
    default:
      break;
  }

  // notify and blink
  homekit_characteristic_notify(&ch_speed, ch_speed.value);
  blink_led(100);
}