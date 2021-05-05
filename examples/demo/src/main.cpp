#include <Arduino.h>
#include <ArduinoHttpClient.h>
#include <WiFi.h>
// Turn on debug statements from the knock library
#define KNOCK_DEBUG
#include "knock.h"

WiFiClient wifi_client;
HttpClient http_client = HttpClient(wifi_client, "worldtimeapi.org", 80);

bool LED_ON = false;

void setup() {
    Serial.begin(115200);
    Serial.println("start");

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    // Advertise over BLE and wait for a connection.
    Knock.setup("Knock Demo ESP32");
}

void loop() {
    if (Knock.is_connected()) {
        // Blink LED to show we're connected.
        digitalWrite(LED_BUILTIN, LED_ON ? HIGH : LOW);
        LED_ON = !LED_ON;

        // Wifi should now be connected, run query!
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
    }

    // Only need to do this if we want to broadcase the WiFi status of this device.
    Knock.notify();
    delay(1000);

}