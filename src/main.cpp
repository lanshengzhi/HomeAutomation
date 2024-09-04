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
LiquidCrystal_I2C lcd(0x27, 16, 2);

TimerHandle_t dhtTimer;
TimerHandle_t blinkTimer;
TimerHandle_t buttonTimer;

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

void blinkTimerCallback(TimerHandle_t xTimer) {
    static bool ledState = false;
    ledState = !ledState;
    digitalWrite(LED_INDICATOR, ledState);
}

void buttonTimerCallback(TimerHandle_t xTimer) {
    prevButton.Run();
    upButton.Run();
    downButton.Run();
    nextButton.Run();
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

    // Create DHT timer (5 seconds interval)
    dhtTimer = xTimerCreate(
        "DHT Timer",
        pdMS_TO_TICKS(5000),
        pdTRUE,
        (void*)0,
        dhtTimerCallback
    );

    // Create blink timer (1 second interval)
    blinkTimer = xTimerCreate(
        "Blink Timer",
        pdMS_TO_TICKS(1000),
        pdTRUE,
        (void*)0,
        blinkTimerCallback
    );

    // Create button timer (10 milliseconds interval)
    buttonTimer = xTimerCreate(
        "Button Timer",
        pdMS_TO_TICKS(10),
        pdTRUE,
        (void*)0,
        buttonTimerCallback
    );

    // Start all timers
    if (dhtTimer != NULL) {
        xTimerStart(dhtTimer, 0);
    }
    if (blinkTimer != NULL) {
        xTimerStart(blinkTimer, 0);
    }
    if (buttonTimer != NULL) {
        xTimerStart(buttonTimer, 0);
    }
}

void loop() {
    // add delay
    vTaskDelay(pdMS_TO_TICKS(5000));
}