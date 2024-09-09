#include "wifi_handler.h"
#include "config.h"
#include "WiFiManager.h"
#include "WiFi.h"
#include "dashboard_handler.h"

static unsigned long resetButtonPressStartTime = 0;
static bool isResetButtonPressed = false;

void checkResetButton() {
    if (digitalRead(WIFI_RESET_BUTTON_PIN) == LOW) {  // The button has been pressed
        if (!isResetButtonPressed) {  // The button has been pressed
            isResetButtonPressed = true;
            resetButtonPressStartTime = millis();
        } else if ((millis() - resetButtonPressStartTime) > LONG_PRESS_TIME) {
            // The button has been pressed for more than 5 seconds
            Serial.println("Reset button long-pressed. Resetting WiFi settings...");
            WiFiManager wifiManager;
            wifiManager.resetSettings();
            Serial.println("WiFi settings reset. Restarting...");
            delay(1000);  // Wait for one second to allow serial messages to output
            ESP.restart();  // Restart ESP32
        }
    } else {
        // The button has been released
        isResetButtonPressed = false;
    }
}

void onWiFiEventHandler(WiFiEvent_t event) {
    DeviceInfo* deviceInfo = SetDeviceInfo();
    switch(event) {
        case SYSTEM_EVENT_STA_GOT_IP:
            deviceInfo->deviceState.bits.wifi = 1;
            Serial.println("WiFi connected");
            break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
            deviceInfo->deviceState.bits.wifi = 0;
            Serial.println("WiFi disconnected");
            break;
        default:
            break;
    }
}

void setupWiFi() {
    // Initialize resetButton
    pinMode(WIFI_RESET_BUTTON_PIN, INPUT_PULLUP);  // Use internal pull-up resistor

    while (digitalRead(WIFI_RESET_BUTTON_PIN) == LOW) {
        checkResetButton();
        delay(100); // Small delay to avoid too frequent checks
    }

    WiFi.onEvent(onWiFiEventHandler);

    WiFiManager wifiManager;
    wifiManager.autoConnect("AutoConnectAP");
}