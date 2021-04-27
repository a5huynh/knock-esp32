#ifndef KNOCK_KNOCK_H
#define KNOCK_KNOCK_H
#include "BLEDevice.h"

class KnockClass: public BLECharacteristicCallbacks, BLEServerCallbacks {
private:
    bool _is_connected;
    static void init();
    static void setup_wifi();
public:
    KnockClass();

    int setup(char* api_key);
    bool is_connected();

    // BLECharacteristicCallbacks
    void onWrite(BLECharacteristic *pCharacteristic);

    // BLEServerCallbacks
    void onConnect(BLEServer*);
    void onDisconnect(BLEServer*);
};

extern KnockClass Knock;

#endif