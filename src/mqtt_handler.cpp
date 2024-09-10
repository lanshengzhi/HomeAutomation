#include "mqtt_handler.h"
#include "config.h"
#include "WiFi.h"
#include "PubSubClient.h"
#include "dashboard_handler.h"

unsigned long lastReconnectAttempt = 0;
static bool lastMqttConnectionState = false;

static WiFiClient espWiFiClient;
static PubSubClient mqttClient(espWiFiClient);

void onMqttConnectionChange(bool connected) {
    DeviceInfo* deviceInfo = SetDeviceInfo();
    deviceInfo->deviceState.bits.mqtt = connected ? 1 : 0;
    Serial.println(connected ? "MQTT Connected" : "MQTT Disconnected");
}

bool reconnectMQTT() {
    bool connected = mqttClient.connect(MQTT_CLIENT_ID, MQTT_BROKER_USERNAME, MQTT_BROKER_PASSWORD);
    if (connected != lastMqttConnectionState) {
        onMqttConnectionChange(connected);  // Update the connection state so the dashboard knows
        lastMqttConnectionState = connected;
    }
    if (connected) {
        mqttClient.subscribe(TOPIC_SUBSCRIBE);
    }
    return connected;
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

void publishDeviceInfo() {
    DeviceInfo* deviceInfo = GetDeviceInfo();
    char tempStr[10], humidityStr[10];
    snprintf(tempStr, sizeof(tempStr), "%.2f", deviceInfo->temperature);
    snprintf(humidityStr, sizeof(humidityStr), "%.2f", deviceInfo->humidity);
    mqttClient.publish(TOPIC_TEMP, tempStr);
    mqttClient.publish(TOPIC_HUMIDITY, humidityStr);
    Serial.println("MQTT Publish");
}

void setupMQTT() {
    mqttClient.setServer(MQTT_BROKER_IP, MQTT_BROKER_PORT);
    mqttClient.setCallback(onMQTTCallback);
    mqttClient.setKeepAlive(MQTT_KEEP_ALIVE);
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
