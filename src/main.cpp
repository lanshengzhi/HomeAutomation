#include "PushButton.h"
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/timers.h>
#include <DHTesp.h>
#include <LiquidCrystal_I2C.h>

#define LED_INDICATOR 15  //  LED indicator is connected to GPIO 15

PushButton prevButton(32);
PushButton upButton(33);
PushButton downButton(25);
PushButton nextButton(26);

DHTesp dht;
TimerHandle_t dhtTimer;

LiquidCrystal_I2C lcd(0x27, 16, 2);

void dhtTimerCallback(TimerHandle_t xTimer) {
    static float lastTemp = 0;
    static float lastHumidity = 0;
    
    TempAndHumidity values = dht.getTempAndHumidity();
    if (dht.getStatus() == DHTesp::ERROR_NONE) {
        if (values.temperature != lastTemp) {
            lcd.setCursor(0, 0);
            lcd.printf("Temp: %.1fC    ", values.temperature);
            lastTemp = values.temperature;
        }
        if (values.humidity != lastHumidity) {
            lcd.setCursor(0, 1);
            lcd.printf("Humidity: %.1f%%", values.humidity);
            lastHumidity = values.humidity;
        }
    } else {
        lcd.setCursor(0, 0);
        lcd.print("DHT Read Error!");
        lcd.setCursor(0, 1);
        lcd.print("                ");
    }
}

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

    dht.setup(27, DHTesp::DHT11);

    lcd.init();
    lcd.backlight();

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

    // Create a timer that triggers every 5 seconds
    dhtTimer = xTimerCreate(
        "DHT Timer",
        pdMS_TO_TICKS(5000),
        pdTRUE,
        (void*)0,
        dhtTimerCallback
    );

    if (dhtTimer != NULL) {
        xTimerStart(dhtTimer, 0);
    }
}

void loop() {
    // Main loop is empty, all work is done in FreeRTOS tasks
}