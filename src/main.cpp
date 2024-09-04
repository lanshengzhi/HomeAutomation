#include "PushButton.h"
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define LED_INDICATOR 15  //  LED indicator is connected to GPIO 15

PushButton prevButton(32);
PushButton upButton(33);
PushButton downButton(25);
PushButton nextButton(26);

void blinkTask(void *pvParameters) {
    pinMode(LED_INDICATOR, OUTPUT);
    while (1) {
        digitalWrite(LED_INDICATOR, HIGH);
        vTaskDelay(pdMS_TO_TICKS(1000));
        digitalWrite(LED_INDICATOR, LOW);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void buttonTask(void *pvParameters) {
    while (1) {
        prevButton.Run();
        upButton.Run();
        downButton.Run();
        nextButton.Run();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
void setup() {
    Serial.begin(115200);
    Serial.println("FreeRTOS LED blinking task initialization");

    prevButton.SetPressedCallback([]() {
        Serial.println("Previous button pressed");
    });

    prevButton.SetReleasedCallback([]() {
        Serial.println("Previous button released");
    });

    upButton.SetPressedCallback([]() {
        Serial.println("Up button pressed");
    });

    upButton.SetReleasedCallback([]() {
        Serial.println("Up button released");
    });
    
    downButton.SetPressedCallback([]() {
        Serial.println("Down button pressed");
    });

    downButton.SetReleasedCallback([]() {
        Serial.println("Down button released");
    });

    nextButton.SetPressedCallback([]() {
        Serial.println("Next button pressed");
    });

    nextButton.SetReleasedCallback([]() {
        Serial.println("Next button released");
    });

    xTaskCreate(
        blinkTask,   // Task function
        "LED Task",  // Task name
        1024,        // Stack size
        NULL,        // Task parameters
        1,           // Task priority
        NULL         // Task handle
    );

    xTaskCreate(
        buttonTask,    // Task function
        "Button Task", // Task name
        1024,          // Stack size
        NULL,          // Task parameters
        1,             // Task priority
        NULL           // Task handle
    );
}

void loop() {
    // Main loop is empty, all work is done in FreeRTOS tasks
}