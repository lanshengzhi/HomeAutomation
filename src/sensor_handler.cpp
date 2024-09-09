#include "sensor_handler.h"
#include "dashboard_handler.h"
#include "config.h"
#include "DHTesp.h"
#include <Arduino.h>

static DHTesp dht;
static float lastTemp = 0;
static float lastHumidity = 0;
static unsigned long lastSensorCheckTime = 0;
static unsigned long sensorCheckInterval = 5000;

void setupSensors() {
    dht.setup(DHT_PIN, DHTesp::DHT11);
    lastSensorCheckTime = millis();
}

void loopSensors() {
    
    unsigned long currentTime = millis();
    
    if (currentTime - lastSensorCheckTime < sensorCheckInterval) {
        return;
    }

    lastSensorCheckTime = currentTime;

    TempAndHumidity values = dht.getTempAndHumidity();
    if (dht.getStatus() != DHTesp::ERROR_NONE) {
        Serial.println("DHT Read Error!");
        SetDeviceInfo()->deviceState.bits.dht = 0;
        return;
    }

    if (values.temperature != lastTemp) {
        lastTemp = values.temperature;
        SetDeviceInfo()->temperature = values.temperature;
        SetDeviceInfo()->deviceState.bits.dht = 1;
        Serial.println(values.temperature);
    }

    if (values.humidity != lastHumidity) {
        lastHumidity = values.humidity;
        SetDeviceInfo()->humidity = values.humidity;
        SetDeviceInfo()->deviceState.bits.dht = 1;
        Serial.println(values.humidity);
    }
}
