#include "button_handler.h"
#include "config.h"
#include "dashboard_handler.h"
#include <PushButton.h>
#include <Arduino.h>

static PushButton prevButton(BUTTON_PREV_PIN);
static PushButton upButton(BUTTON_UP_PIN);
static PushButton downButton(BUTTON_DOWN_PIN);
static PushButton nextButton(BUTTON_NEXT_PIN);

static unsigned long lastButtonCheckTime = 0;
const unsigned long buttonCheckInterval = 20; // 20ms

void setupButtons() {
    prevButton.SetPressedCallback([]() {
        SetDeviceInfo()->buttonState.bits.prev = 1;
        Serial.println("Prev button pressed");
        Serial.println(GetDeviceInfo()->buttonState.all);
    });

    prevButton.SetReleasedCallback([]() {
        SetDeviceInfo()->buttonState.bits.prev = 0;
        Serial.println("Prev button released");
        Serial.println(GetDeviceInfo()->buttonState.all);
    });

    upButton.SetPressedCallback([]() {
        SetDeviceInfo()->buttonState.bits.up = 1;
        Serial.println("Up button pressed");
        Serial.println(GetDeviceInfo()->buttonState.all);
    });

    upButton.SetReleasedCallback([]() {
        SetDeviceInfo()->buttonState.bits.up = 0;
        Serial.println("Up button released");
        Serial.println(GetDeviceInfo()->buttonState.all);
    });
    
    downButton.SetPressedCallback([]() {
        SetDeviceInfo()->buttonState.bits.down = 1;
        Serial.println("Down button pressed");
        Serial.println(GetDeviceInfo()->buttonState.all);
    });

    downButton.SetReleasedCallback([]() {
        SetDeviceInfo()->buttonState.bits.down = 0;
        Serial.println("Down button released");
        Serial.println(GetDeviceInfo()->buttonState.all);
    });

    nextButton.SetPressedCallback([]() {
        SetDeviceInfo()->buttonState.bits.next = 1;
        Serial.println("Next button pressed");
        Serial.println(GetDeviceInfo()->buttonState.all);
    });

    nextButton.SetReleasedCallback([]() {
        SetDeviceInfo()->buttonState.bits.next = 0;
        Serial.println("Next button released");
        Serial.println(GetDeviceInfo()->buttonState.all);
    });

    lastButtonCheckTime = millis(); // Initialize lastButtonCheckTime
}

void loopButtons() {
    unsigned long currentTime = millis();
    
    if (currentTime - lastButtonCheckTime < buttonCheckInterval) {
        return;
    }

    prevButton.Run();
    upButton.Run();
    downButton.Run();
    nextButton.Run();
    
    lastButtonCheckTime = currentTime;
}
