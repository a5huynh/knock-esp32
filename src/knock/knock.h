#ifndef KNOCK_KNOCK_H
#define KNOCK_KNOCK_H
#include <BLEDevice.h>

class KnockClass: public BLECharacteristicCallbacks {
private:
    bool _is_connected;
    static void init();
    static void setup_wifi();
public:
    KnockClass();

    int setup(char* api_key);
    bool is_connected();

    void onWrite(BLECharacteristic *pCharacteristic);
};

extern KnockClass Knock;

#endif