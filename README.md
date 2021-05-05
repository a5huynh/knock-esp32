# knock-esp32

A library to easily handle WiFi configuration over BLE. Requires an iOS / Android
application that has BLE to send the details over.

Here is a bare bones example:
``` cpp
#include "knock.h"

void setup() {
    // This is the name that shows up to the iOS application
    Knock.init("Fancy Pants Detector");
}

void loop() {
    if (Knock.has_wifi_connection()) {
        // do your fun stuff here!
    }
}
```