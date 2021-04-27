#include <Arduino.h>
#include <SPI.h>
#include "BLEDevice.h"
#include "BLEServer.h"
#include "BLEUUID.h"
#include <WiFI.h>

#include "knock.h"

const char *WIFI_SSID   = "ssid";
const char *WIFI_PASS   = "pass";

// 0x1824 is the GATT service id for "Transport Discovery"
#define SERVICE_UUID        BLEUUID((uint16_t)0x1824)
#define TURN_ON_CHAR        BLEUUID("beefcafe-36e1-4688-b7f5-000000000001")
#define WIFI_STATUS_CHAR    BLEUUID("beefcafe-36e1-4688-b7f5-000000000002")

BLECharacteristic *chr_toggle = nullptr;
int LED_VALUE = HIGH;

KnockClass::KnockClass() {
    _is_connected = false;
}

void KnockClass::init() {

}

void KnockClass::setup_wifi() {
    Serial.println("Connecting to wifi");
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("WiFI connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

int KnockClass::setup(char * api_key) {
    Serial.println("init ble stack");

    BLEDevice::init("Knock Knock ESP32");
    BLEServer *pServer = BLEDevice::createServer();
    pServer->setCallbacks(this);

    BLEService *pService = pServer->createService(SERVICE_UUID);
    chr_toggle = pService->createCharacteristic(
        TURN_ON_CHAR,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
    );
    chr_toggle->setCallbacks(this);
    pService->start();

    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();

    Serial.println("ble advertising start!");

    return 0;
}

bool KnockClass::is_connected() {
    return this->_is_connected;
}

void KnockClass::onWrite(BLECharacteristic *pCharacteristic) {
    std::string value = pCharacteristic->getValue();

    if (value.length() > 0) {
        if (value[0]) {
            Serial.println("received: TRUE");
            LED_VALUE = HIGH;
            setup_wifi();
        } else {
            Serial.println("received: FALSE");
            LED_VALUE = LOW;
        }

        digitalWrite(LED_BUILTIN, LED_VALUE);
    }
}

void KnockClass::onConnect(BLEServer *pServer) {
    Serial.println("onConnect");
    this->_is_connected = true;
    BLEDevice::stopAdvertising();
}

void KnockClass::onDisconnect(BLEServer *pServer) {
    Serial.println("onDisconnect");
    this->_is_connected = false;
    BLEDevice::startAdvertising();
}

KnockClass Knock;