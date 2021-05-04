#include <Arduino.h>
#include <ArduinoHttpClient.h>
#include <WiFi.h>

#include "knock/knock.h"

#define API_KEY "utMUhnw-S766fII7g6ZbqCdl7dtWBZJpGcu3LgWNAYM"

WiFiClient wifi_client;
HttpClient http_client = HttpClient(wifi_client, "worldtimeapi.org", 80);

bool LED_ON = false;
bool REQUEST_MADE = false;

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
        // Blink LED to show we're connected.
        digitalWrite(LED_BUILTIN, LED_ON ? HIGH : LOW);
        LED_ON = !LED_ON;

        // Wifi should now be connected, run query!
        if (!REQUEST_MADE) {
            http_client.get("/api/ip");
            int statusCode = http_client.responseStatusCode();
            String response = http_client.responseBody();

            // Print out response to console
            Serial.println("---");
            Serial.print("Status code: ");
            Serial.println(statusCode);
            Serial.print("Response: ");
            Serial.println(response);
            Serial.println("---");
            REQUEST_MADE = true;
        }
    }

    Knock.notify();
    delay(1000);

}