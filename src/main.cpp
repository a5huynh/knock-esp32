#include <Arduino.h>
#include <ArduinoHttpClient.h>
#include <WiFi.h>

#include "knock/knock.h"

#define API_KEY "utMUhnw-S766fII7g6ZbqCdl7dtWBZJpGcu3LgWNAYM"

WiFiClient wifi_client;
HttpClient http_client = HttpClient(wifi_client, "https://knockiot.com", 80);

// const char *ssid        = "seriously 2.4";
// const char *password    = "bearrepublic";

// void connect_wifi() {
//     Serial.println("Connecting to wifi");
//     WiFi.begin(ssid, password);
//     while (WiFi.status() != WL_CONNECTED) {
//         delay(500);
//         Serial.print(".");
//     }

//     Serial.println("WiFI connected");
//     Serial.println("IP address: ");
//     chr_wifi_status->setValue("connected");
//     Serial.println(WiFi.localIP());
// }

void setup() {
    Serial.begin(115200);
    Serial.println("start");

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    // Knock advertise over BLE and wait for a connection.
    Knock.setup(API_KEY);
}

void loop() {
    if (Knock.is_connected()) {
        // Wifi should now be connected, run query!
    }
    delay(100);
}