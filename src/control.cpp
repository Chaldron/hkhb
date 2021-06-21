#include "control.h"

#include <Arduino.h>

// Off, low, medium, high controls
// "Fun" with macros
#define rc(state, pin)       \
  void rc_##state() {        \
    digitalWrite(pin, LOW);  \
    delay(IR_CONTROL_TIME);  \
    digitalWrite(pin, HIGH); \
  }
rc(low, PIN_LOW) rc(medium, PIN_MEDIUM) rc(high, PIN_HIGH) rc(off, PIN_OFF)

    void led_on() {
  digitalWrite(PIN_LED, LOW);
}
void led_off() { digitalWrite(PIN_LED, HIGH); }
void blink_led(unsigned long time) {
  led_on();
  delay(time);
  led_off();
}