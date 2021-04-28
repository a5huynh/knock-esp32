#include <Arduino.h>
#include <ArduinoHttpClient.h>
#include <WiFi.h>

#include "knock/knock.h"

#define API_KEY "utMUhnw-S766fII7g6ZbqCdl7dtWBZJpGcu3LgWNAYM"

WiFiClient wifi_client;
HttpClient http_client = HttpClient(wifi_client, "https://knockiot.com", 80);

void setup() {
    Serial.begin(115200);
    Serial.println("start");

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    // Knock advertise over BLE and wait for a connection.
    Knock.setup(API_KEY, "Knock Knock ESP32");
}

void loop() {
    if (Knock.is_connected()) {
        // Wifi should now be connected, run query!
    }

    delay(1000);
}