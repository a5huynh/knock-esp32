#include <Arduino.h>
#include <SPI.h>
#include <WiFI.h>
#include "BLEDevice.h"
#include "BLEServer.h"
#include "BLEUUID.h"
#include "Preferences.h"
#include "nvs.h"
#include "nvs_flash.h"

#include "knock.h"

#define PREF_WIFI_SSID      "WIFI_SSID"
#define PREF_WIFI_PASS      "WIFI_PASS"

// 0x1824 is the GATT service id for "Transport Discovery"
#define SERVICE_WIFI_SETUP  BLEUUID((uint16_t)0x1824)
#define CHAR_WIFI_SSID      BLEUUID("beefcafe-36e1-4688-b7f5-000000000001")
#define CHAR_WIFI_PASS      BLEUUID("beefcafe-36e1-4688-b7f5-000000000002")
#define CHAR_WIFI_STATUS    BLEUUID("beefcafe-36e1-4688-b7f5-000000000003")
#define CHAR_WIFI_START     BLEUUID("beefcafe-36e1-4688-b7f5-000000000004")

BLECharacteristic *chr_wifi_ssid = nullptr;
BLECharacteristic *chr_wifi_pass = nullptr;
BLECharacteristic *chr_wifi_status = nullptr;
BLECharacteristic *chr_wifi_start = nullptr;

Preferences preferences;

static String WIFI_SSID;
static String WIFI_PASS;

KnockClass::KnockClass() {
    _is_ble_connected = false;
    _is_wifi_connected = false;
}

void KnockClass::init() {
    preferences.begin("knockiot", false);

    WIFI_SSID = preferences.getString(PREF_WIFI_SSID);
    WIFI_PASS = preferences.getString(PREF_WIFI_PASS);
    // Print out stored SSID/PASS for debugging purposes.
    Serial.print("SSID: "); Serial.println(WIFI_SSID);
    Serial.print("PASS: "); Serial.println(WIFI_PASS);
}

void KnockClass::setup_ble() {
    BLEServer *pServer = BLEDevice::createServer();
    pServer->setCallbacks(this);

    BLEService *pService = pServer->createService(SERVICE_WIFI_SETUP);
    chr_wifi_ssid = pService->createCharacteristic(
        CHAR_WIFI_SSID,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
    );
    chr_wifi_ssid->setCallbacks(this);
    chr_wifi_ssid->setValue(WIFI_SSID.c_str());

    chr_wifi_pass = pService->createCharacteristic(
        CHAR_WIFI_PASS,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
    );
    chr_wifi_pass->setCallbacks(this);
    chr_wifi_status = pService->createCharacteristic(
        CHAR_WIFI_STATUS,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE |
        BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_INDICATE
    );
    update_wifi_status();

    chr_wifi_start = pService->createCharacteristic(
        CHAR_WIFI_START,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
    );
    chr_wifi_start->setCallbacks(this);

    pService->start();

    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_WIFI_SETUP);
    pAdvertising->setScanResponse(true);
    BLEDevice::startAdvertising();
    Serial.println("ble advertising start!");
}

void KnockClass::setup_wifi() {
    Serial.println("Connecting to wifi");
    WiFi.begin(WIFI_SSID.c_str(), WIFI_PASS.c_str());
    while (WiFi.status() != WL_CONNECTED) {
        update_wifi_status();
        delay(500);
        Serial.print(".");
    }

    update_wifi_status();
    Serial.println("WiFI connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    this->_is_wifi_connected = true;
}

int KnockClass::setup(char * api_key, char * device_name) {
    Serial.println("init knock");
    this->init();
    Serial.println("init ble stack");

    BLEDevice::init(device_name);
    this->setup_ble();

    return 0;
}

bool KnockClass::is_connected() {
    return this->_is_wifi_connected;
}

void KnockClass::update_wifi_status() {
    if (chr_wifi_status) {
        uint8_t status[1];
        status[0] = WiFi.status();
        char buffer[32];
        sprintf(buffer, "Notifying w/ value: %d", status[0]);
        Serial.println(buffer);
        chr_wifi_status->setValue((uint8_t*)status, 1);
        // Only notify if we're connected
        if (this->_is_ble_connected) {
            chr_wifi_status->notify();
        }
    }
}

void KnockClass::_print_value(const char* uuid, std::string value) {
    Serial.println("-----");
    Serial.print("Data for: "); Serial.println(uuid);

    Serial.print("Received: ");
    for (int i = 0; i < value.length(); i++) {
        Serial.print(value[i]);
    }
    Serial.println();
    Serial.println("-----");
}

// Callbacks for BLECharacteristics & BLEServer
void KnockClass::onWrite(BLECharacteristic *pCharacteristic) {
    std::string value = pCharacteristic->getValue();
    this->_print_value(
        pCharacteristic->getUUID().toString().c_str(),
        value
    );

    if (pCharacteristic->getUUID().equals(CHAR_WIFI_SSID)) {
        preferences.putString(PREF_WIFI_SSID, value.c_str());
    } else if (pCharacteristic->getUUID().equals(CHAR_WIFI_PASS)) {
        preferences.putString(PREF_WIFI_PASS, value.c_str());
    } else if (pCharacteristic->getUUID().equals(CHAR_WIFI_START)) {
        Serial.println("Attempting to connect to WiFi");
        this->setup_wifi();
    }
}

void KnockClass::onConnect(BLEServer *pServer) {
    Serial.println("onConnect");
    BLEDevice::stopAdvertising();

    this->_is_ble_connected = true;
    // Let client know what we have stored. We don't emit the password to keep
    // it safe.
    chr_wifi_ssid->setValue(WIFI_SSID.c_str());
    update_wifi_status();
}

void KnockClass::onDisconnect(BLEServer *pServer) {
    Serial.println("onDisconnect");
    BLEDevice::startAdvertising();
    this->_is_ble_connected = false;
}

KnockClass Knock;