#include "PushButton.h"
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/timers.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <DHTesp.h>
#include <LiquidCrystal_I2C.h>
#include <WiFiManager.h>

// MQTT broker
static const char mqtt_broker_ip[]       = "52.194.211.190";
static const int  mqtt_broker_port       = 1883;
static const char mqtt_broker_username[] = "admin";
static const char mqtt_broker_password[] = "452124@lansy";

// MQTT topic
static const char topic_temp[]      = "HA-ESP32-01/temp/state";
static const char topic_humidity[]  = "HA-ESP32-01/humidity/state";
static const char topic_subscribe[] = "ESP32/command";

#define LED_INDICATOR 15  //  LED indicator is connected to GPIO 15

#define RESET_BUTTON_PIN 26
#define LONG_PRESS_TIME 5000

PushButton prevButton(32);
PushButton upButton(33);
PushButton downButton(25);
PushButton nextButton(26);

DHTesp dht;
LiquidCrystal_I2C lcd(0x27, 16, 2);

WiFiClient espWiFiClient;
PubSubClient mqttClient(espWiFiClient);

TimerHandle_t dhtTimer;
TimerHandle_t blinkTimer;
TimerHandle_t buttonTimer;
TimerHandle_t wifiReconnectTimer;

void publishToMQTT(const char* topic, float value) {
    if (mqttClient.publish(topic, String(value).c_str())) {
        Serial.println("Published to MQTT successfully");
    } else {
        Serial.println("Failed to publish to MQTT");
    }
}

void updateLCD(int row, const char* format, float value) {
    lcd.setCursor(0, row);
    lcd.printf(format, value);
}

void dhtTimerCallback(TimerHandle_t xTimer) {
    static float lastTemp = 0;
    static float lastHumidity = 0;
    
    TempAndHumidity values = dht.getTempAndHumidity();
    if (dht.getStatus() == DHTesp::ERROR_NONE) {
        if (values.temperature != lastTemp) {
            updateLCD(0, "Temp: %.1fC    ", values.temperature);
            publishToMQTT(topic_temp, values.temperature);
            lastTemp = values.temperature;
        }

        if (values.humidity != lastHumidity) {
            updateLCD(1, "Humidity: %.1f%%", values.humidity);
            publishToMQTT(topic_humidity, values.humidity);
            lastHumidity = values.humidity;
        }
    } else {
        updateLCD(0, "DHT Read Error!", 0);
        updateLCD(1, "                ", 0);
    }
}

void blinkTimerCallback(TimerHandle_t xTimer) {
    static bool ledState = false;
    ledState = !ledState;
    digitalWrite(LED_INDICATOR, ledState);
    Serial.println("Blink timer fired");
}

void buttonTimerCallback(TimerHandle_t xTimer) {
    prevButton.Run();
    upButton.Run();
    downButton.Run();
    nextButton.Run();
}

void MQTTCallback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (unsigned int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
    }
    Serial.println();
}

unsigned long resetButtonPressStartTime = 0;
bool isResetButtonPressed = false;

void checkResetButton() {
    if (digitalRead(RESET_BUTTON_PIN) == LOW) {  // The button has been pressed
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


void setup() {
    Serial.begin(115200);
    Serial.println("FreeRTOS LED blinking task initialization");

    // Initialize LED_INDICATOR pin
    pinMode(LED_INDICATOR, OUTPUT);

    // Initialize resetButton
    pinMode(RESET_BUTTON_PIN, INPUT_PULLUP);  // Use internal pull-up resistor

    while (digitalRead(RESET_BUTTON_PIN) == LOW) {
        checkResetButton();
        delay(100); // Small delay to avoid too frequent checks
    }

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

    WiFiManager wifiManager;
    wifiManager.autoConnect("AutoConnectAP");

    // Subscribe the topic
    mqttClient.setServer(mqtt_broker_ip, mqtt_broker_port);

    mqttClient.subscribe(topic_subscribe);
    mqttClient.setCallback(MQTTCallback);

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