#ifndef KNOCK_KNOCK_H
#define KNOCK_KNOCK_H
#include "BLEDevice.h"

class KnockClass: public BLECharacteristicCallbacks, BLEServerCallbacks {
private:
    bool _is_ble_connected;
    bool _is_wifi_connected;

    static void init();

    void setup_wifi();
    void setup_ble();

    void _print_value(const char* uuid, std::string value);
    void _notify_wifi_status();
public:
    KnockClass();

    int setup(std::string device_name);
    bool is_connected();
    void notify();

    // BLECharacteristicCallbacks
    void onWrite(BLECharacteristic *);

    // BLEServerCallbacks
    void onConnect(BLEServer*);
    void onDisconnect(BLEServer*);
};

extern KnockClass Knock;

#endif