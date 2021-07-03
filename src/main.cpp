#include <Arduino.h>
#include <ArduinoLog.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <aREST.h>

#include "control.h"
#include "wifi.h"

WiFiServer server(80);
aREST rest = aREST();

extern bool active;
extern uint8_t speed;

void setup() {
  // Start serial logging
  Serial.begin(115200);
  Log.begin(LOG_LEVEL_INFO, &Serial);

  Log.info("HomeBreeze HomeBridge firmware initializing" CR);

  // Initialize control
  Log.info("Initializing fan control" CR);
  init_control();

  // Connect to the internet
  Log.info("Connecting to the internet" CR);
  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.hostname(domain);
  WiFi.begin(ssid, password);
  Log.info("Waiting for wifi connection");
  while (!WiFi.isConnected()) {
    delay(100);
  }
  Log.info("IP: %p, Hostname: %s" CR, WiFi.localIP(), WiFi.getHostname());

  // Start broadcasting mDNS
  Log.info("Broadcasting mDNS" CR);
  MDNS.addService("http", "tcp", 80);
  if (!MDNS.begin(domain)) {
    Log.error("mDNS setup failed!" CR);
  }

  // Setup REST API
  Log.info("Initializing REST API" CR);
  rest.variable("active", &active);
  rest.variable("speed", &speed);
  rest.function("identify", identify);
  rest.function("set_active", set_active);
  rest.function("set_rotation_speed", set_rotation_speed);

  // Start the server
  Log.info("Starting the web server" CR);
  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (!client) {
    MDNS.announce();
    MDNS.update();

    delay(10);

    return;
  }

  Log.info("Waiting for client" CR);
  while (!client.available()) {
    delay(10);
  }

  Log.info("Handling client" CR);
  rest.handle(client);
}