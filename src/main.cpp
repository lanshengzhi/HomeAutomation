#include "config.h"
#include "dashboard_handler.h"
#include "wifi_handler.h"
#include "mqtt_handler.h"
#include "sensor_handler.h"
#include "display_handler.h"
#include "button_handler.h"
#include <Arduino.h>

unsigned long lastIndicatorCheckTime = 0;
unsigned long indicatorCheckInterval = 1000;

void setupIndicator() {
    pinMode(LED_INDICATOR, OUTPUT);
}

void loopIndicator() {
    static bool ledState = false;
    unsigned long currentTime = millis();
    
    if (currentTime - lastIndicatorCheckTime < indicatorCheckInterval) {
        return;
    }

    ledState = !ledState;
    digitalWrite(LED_INDICATOR, ledState);
    lastIndicatorCheckTime = currentTime;
}


void setup() {
    Serial.begin(115200);

    setupIndicator();
    setupDashboard();
    setupWiFi();
    setupMQTT();
    setupDisplay();
    setupButtons();
    setupSensors();
}

void loop() {
    loopIndicator();
    loopButtons();
    loopSensors();
    loopMQTT();

    if (isDeviceInfoChanged()) {
        
        loopDisplay();
    }

    loopDashboard(); // This should be called last to ensure the latest data is sent to the dashboard
}