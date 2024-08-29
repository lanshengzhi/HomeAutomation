#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define LED_PIN 15  // Assume LED is connected to GPIO 2

void blinkTask(void *pvParameters) {
    pinMode(LED_PIN, OUTPUT);
    while (1) {
        digitalWrite(LED_PIN, HIGH);
        vTaskDelay(pdMS_TO_TICKS(1000));
        digitalWrite(LED_PIN, LOW);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void setup() {
    Serial.begin(115200);
    Serial.println("FreeRTOS LED blinking task initialization");

    xTaskCreate(
        blinkTask,    // Task function
        "LED Task", // Task name
        1024,       // Stack size
        NULL,       // Task parameters
        1,          // Task priority
        NULL        // Task handle
    );
}

void loop() {
    // Main loop is empty, all work is done in FreeRTOS tasks
    vTaskDelay(pdMS_TO_TICKS(1000));
}