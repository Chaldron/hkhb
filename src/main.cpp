#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <arduino_homekit_server.h>

#include "callback.h"
#include "control.h"
#include "wifi.h"

extern "C" homekit_server_config_t config;

// HomeKit broadcast timer
static uint32_t next_broadcast_timer = 0;

void setup() {
  // Start serial logging
  Serial.begin(115200);

  // Connect to the internet
  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.begin(ssid, password);
  while (!WiFi.isConnected()) {
    delay(100);
  }

  // Run HomeKit setup
  arduino_homekit_setup(&config);

  // Setup GPIO pins

  // HomeKit identification LED
  pinMode(PIN_LED, OUTPUT);
  led_off();

  // Turn on and off all fan settings, ending with off
  pinMode(PIN_OFF, OUTPUT);
  pinMode(PIN_LOW, OUTPUT);
  pinMode(PIN_MEDIUM, OUTPUT);
  pinMode(PIN_HIGH, OUTPUT);
  rc_high();
  rc_medium();
  rc_low();
  rc_off();
}

void loop() {
  arduino_homekit_loop();
  const uint32_t t = millis();

  // Keep-alive
  if (t > next_broadcast_timer) {
    next_broadcast_timer = t + 1500;
    // notify();
    MDNS.announce();
  }

  delay(10);
}
