#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <arduino_homekit_server.h>

#include "wifi.h"

#define LOG_D(fmt, ...) printf_P(PSTR(fmt "\n"), ##__VA_ARGS__);

//==============================
// Homekit setup and loop
//==============================

// access your homekit characteristics defined in my_accessory.c
extern "C" homekit_server_config_t config;
extern "C" homekit_characteristic_t active;

static uint32_t next_heap_millis = 0;
static uint32_t next_report_millis = 0;

void my_homekit_setup() { arduino_homekit_setup(&config); }

void my_homekit_report() {
  active.value.bool_value = true;
  homekit_characteristic_notify(&active, active.value);
  // homekit_characteristic_notify(&cha_current_temperature,
  //                              cha_current_temperature.value);
}

void my_homekit_loop() {
  arduino_homekit_loop();
  const uint32_t t = millis();
  if (t > next_report_millis) {
    // report sensor values every 10 seconds
    next_report_millis = t + 10 * 1000;
    my_homekit_report();
  }
  if (t > next_heap_millis) {
    // show heap info every 5 seconds
    next_heap_millis = t + 5 * 1000;
    LOG_D("Free heap: %d, HomeKit clients: %d", ESP.getFreeHeap(),
          arduino_homekit_connected_clients_count());
  }
}

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

  my_homekit_setup();
}

void loop() {
  my_homekit_loop();
  delay(10);
}
