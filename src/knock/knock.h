#ifndef KNOCK_KNOCK_H
#define KNOCK_KNOCK_H
#include "BLEDevice.h"

class KnockClass: public BLECharacteristicCallbacks, BLEServerCallbacks {
private:
    bool _is_ble_connected;
    bool _is_wifi_connected;

    static void init();

    void _print_value(const char* uuid, std::string value);
    void setup_wifi();
    void setup_ble();
    void update_wifi_status();
public:
    KnockClass();

    int setup(char* api_key, char* device_name);
    bool is_connected();

    // BLECharacteristicCallbacks
    void onWrite(BLECharacteristic *pCharacteristic);

    // BLEServerCallbacks
    void onConnect(BLEServer*);
    void onDisconnect(BLEServer*);
};

extern KnockClass Knock;

#endif