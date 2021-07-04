#include <Arduino.h>
#include <ArduinoLog.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <aREST.h>

#include "control.h"
#include "wifi.h"

// Web server and REST client
WiFiServer server(80);
aREST rest = aREST();

// Timer for mDNS broadcast
unsigned long mDNS_timer = millis();

// Current status of the fan
extern bool active;
extern uint8_t speed;

void setup() {
  // Start serial logging
  Serial.begin(115200);
  Log.begin(LOG_LEVEL_INFO, &Serial);

  Log.info("Initializing HomeBreeze HomeBridge firmware" CR);

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
  rest.function("set_speed", set_speed);

  // Start the server
  Log.info("Starting the web server" CR);
  server.begin();
}

void loop() {
  // Broadcast mDNS if needed
  unsigned long time = millis();
  if (time - mDNS_timer > 1500) {
    Log.trace("Broadcasting mDNS" CR);
    MDNS.announce();
    MDNS.update();
    mDNS_timer = time;
  }

  // Handle client, if available
  WiFiClient client = server.available();
  if (!client) {
    // Log.info("No client found!")
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