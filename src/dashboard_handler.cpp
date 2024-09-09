#include "dashboard_handler.h"
#include <Arduino.h>

DeviceInfo deviceInfo;
bool deviceInfoChanged = false;

void setupDashboard() {
    Serial.println("Dashboard setup");
    memset(&deviceInfo, 0, sizeof(DeviceInfo));
}

DeviceInfo * GetDeviceInfo() {
    return &deviceInfo;
}

DeviceInfo * SetDeviceInfo() {
    deviceInfoChanged = true;
    Serial.println("Device info changed");
    return &deviceInfo;
}

void loopDashboard() {
    deviceInfoChanged = false;
}

bool isDeviceInfoChanged() {
    return deviceInfoChanged;
}
