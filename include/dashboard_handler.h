#ifndef DASHBOARD_HANDLER_H
#define DASHBOARD_HANDLER_H

#include <stdint.h>

#define NEXT_BUTTON_MASK  ((uint16_t)0x01)
#define UP_BUTTON_MASK    ((uint16_t)0x02)
#define DOWN_BUTTON_MASK  ((uint16_t)0x04)
#define PREV_BUTTON_MASK  ((uint16_t)0x08)

union DeviceState {
    struct {
        uint16_t dht:1;
        uint16_t mqtt:1;
        uint16_t wifi:1;
        uint16_t reserved:12;  // Reserved bits, ensuring a total size of 16 bits
    } bits;
    uint16_t all;
};

union ButtonState {
    struct {
        uint16_t prev:1;
        uint16_t next:1;
        uint16_t up:1;
        uint16_t down:1;
        uint16_t reserved:12;  // Reserved bits, ensuring a total size of 16 bits
    } bits;
    uint16_t all;
};

struct DeviceInfo {
    DeviceState deviceState;
    ButtonState buttonState;
    float temperature;
    float humidity;
};

void setupDashboard();
DeviceInfo * GetDeviceInfo();
DeviceInfo * SetDeviceInfo();
bool isDeviceInfoChanged();
void loopDashboard();

#endif