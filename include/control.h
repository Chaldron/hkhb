#pragma once

#define PIN_LED 0
#define PIN_OFF 15
#define PIN_LOW 14
#define PIN_MEDIUM 13
#define PIN_HIGH 12

#ifdef __cplusplus
extern "C" {
#endif

// Turn fan off
extern void rc_off();
// Turn fan to low, medium, or high
extern void rc_low();
extern void rc_medium();
extern void rc_high();
// Turn on the board LED
extern void led_on();
// Turn off the board LED
extern void led_off();
// Blink the board LED for a given amount of milliseconds
extern void blink_led(unsigned long);

#ifdef __cplusplus
}
#endif