#include "indicator_handler.h"
#include <Arduino.h>
#include "config.h"

void setupIndicator() {
    pinMode(LED_INDICATOR, OUTPUT);
}

void loopIndicator() {
    static bool ledState = false;
    ledState = !ledState;
    digitalWrite(LED_INDICATOR, ledState);
}