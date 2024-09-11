#include "mqtt_handler.h"
#include "config.h"
#include "WiFi.h"
#include "PubSubClient.h"
#include "dashboard_handler.h"
#include <ArduinoJson.h>

unsigned long lastReconnectAttempt = 0;
static bool lastMqttConnectionState = false;

static WiFiClient espWiFiClient;
static PubSubClient mqttClient(espWiFiClient);

void onMqttConnectionChange(bool connected) {
    DeviceInfo* deviceInfo = SetDeviceInfo();
    deviceInfo->deviceState.bits.mqtt = connected ? 1 : 0;
    Serial.println(connected ? "MQTT Connected" : "MQTT Disconnected");
}

void onMQTTCallback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (unsigned int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
    }
    Serial.println();
}

void publishTemperatureConfig() {
    // Temperature Sensor Config
    JsonDocument temp_doc;
    JsonObject temp_sensor = temp_doc.to<JsonObject>();
    temp_sensor["unique_id"] = "HA-ESP32-01-temp";
    temp_sensor["name"] = "Temperature";
    temp_sensor["icon"] = "mdi:thermometer";
    temp_sensor["device_class"] = "temperature";
    temp_sensor["state_topic"] = "HA-ESP32-01/temp/state";
    temp_sensor["json_attributes_topic"] = "HA-ESP32-01/temp/attributes";
    temp_sensor["unit_of_measurement"] = "C";

    JsonObject temp_device = temp_sensor["device"].to<JsonObject>();
    temp_device["identifiers"] = "ESP32-01";
    temp_device["manufacturer"] = "Bluebird";
    temp_device["model"] = "ESP32";
    temp_device["name"] = "Sensor";
    temp_device["sw_version"] = "1.0";

    char temp_buffer[512];
    if (serializeJson(temp_doc, temp_buffer) > 0) {
        if (mqttClient.publish("homeassistant/sensor/HA/HA-ESP32-01-temp/config", temp_buffer, true)) {
            Serial.println("Temperature Sensor Config Published");
        } else {
            Serial.println("Failed to publish Temperature Sensor Config");
        }
    }

    Serial.println(temp_buffer);
}

void publishHumidityConfig() {
    // Humidity Sensor Config
    JsonDocument humidity_doc;
    JsonObject humidity_sensor = humidity_doc.to<JsonObject>();
    humidity_sensor["unique_id"] = "HA-ESP32-01-humidity";
    humidity_sensor["name"] = "Humidity";
    humidity_sensor["icon"] = "mdi:water-percent";
    humidity_sensor["device_class"] = "humidity";
    humidity_sensor["state_topic"] = "HA-ESP32-01/humidity/state";
    humidity_sensor["json_attributes_topic"] = "HA-ESP32-01/humidity/attributes";
    humidity_sensor["unit_of_measurement"] = "%";

    JsonObject humidity_device = humidity_sensor["device"].to<JsonObject>();
    humidity_device["identifiers"] = "ESP32-01";
    humidity_device["manufacturer"] = "Bluebird";
    humidity_device["model"] = "ESP32";
    humidity_device["name"] = "Sensor";
    humidity_device["sw_version"] = "1.0";

    char humidity_buffer[512];
    if (serializeJson(humidity_doc, humidity_buffer) > 0) {
        if (mqttClient.publish("homeassistant/sensor/HA/HA-ESP32-01-humidity/config", humidity_buffer, true)) {
            Serial.println("Humidity Sensor Config Published");
        } else {
            Serial.println("Failed to publish Humidity Sensor Config");
        }
    }

    Serial.println(humidity_buffer);
}

void publishDeviceInfo() {
    DeviceInfo* deviceInfo = GetDeviceInfo();
    char tempStr[10], humidityStr[10];
    snprintf(tempStr, sizeof(tempStr), "%.2f", deviceInfo->temperature);
    snprintf(humidityStr, sizeof(humidityStr), "%.2f", deviceInfo->humidity);
    mqttClient.publish(TOPIC_TEMP, tempStr);
    mqttClient.publish(TOPIC_HUMIDITY, humidityStr);
    Serial.println("MQTT Publish");
}

bool reconnectMQTT() {
    bool connected = mqttClient.connect(MQTT_CLIENT_ID, MQTT_BROKER_USERNAME, MQTT_BROKER_PASSWORD);
    if (connected != lastMqttConnectionState) {
        onMqttConnectionChange(connected);  // Update the connection state so the dashboard knows
        lastMqttConnectionState = connected;
    }
    if (connected) {
        publishTemperatureConfig();
        publishHumidityConfig();
        publishDeviceInfo();
        mqttClient.subscribe(TOPIC_SUBSCRIBE);
    }
    return connected;
}

void setupMQTT() {
    mqttClient.setServer(MQTT_BROKER_IP, MQTT_BROKER_PORT);
    mqttClient.setCallback(onMQTTCallback);
    mqttClient.setKeepAlive(MQTT_KEEP_ALIVE);
    mqttClient.setBufferSize(MQTT_BUFFER_SIZE);
}

void loopMQTT() {
    bool currentConnectionState = mqttClient.connected();
    
    if (currentConnectionState != lastMqttConnectionState) {
        onMqttConnectionChange(currentConnectionState);  // Update the connection state so the dashboard knows
        lastMqttConnectionState = currentConnectionState;
    }

    if (!currentConnectionState) {  // If MQTT is disconnected
        unsigned long currentTime = millis();
        if (currentTime - lastReconnectAttempt > 5000) {
            lastReconnectAttempt = currentTime;
            if (reconnectMQTT()) {  // Try to reconnect
                lastReconnectAttempt = 0;
            }
        }
        return;  // Exit the function early
    }

    mqttClient.loop();

    if (isDeviceInfoChanged()) {  // If device info has changed
        publishDeviceInfo();
    }
}
