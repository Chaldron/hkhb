#include "control.h"

#include <Arduino.h>

void rc_off() {
  digitalWrite(PIN_OFF, LOW);
  delay(300);
  digitalWrite(PIN_OFF, HIGH);
}
void rc_low() {
  digitalWrite(PIN_LOW, LOW);
  delay(300);
  digitalWrite(PIN_LOW, HIGH);
}
void rc_medium() {
  digitalWrite(PIN_MEDIUM, LOW);
  delay(300);
  digitalWrite(PIN_MEDIUM, HIGH);
}
void rc_high() {
  digitalWrite(PIN_HIGH, LOW);
  delay(300);
  digitalWrite(PIN_HIGH, HIGH);
}
void led_on() { digitalWrite(PIN_LED, LOW); }
void led_off() { digitalWrite(PIN_LED, HIGH); }
void blink_led(unsigned long time) {
  led_on();
  delay(time);
  led_off();
}