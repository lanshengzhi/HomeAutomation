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

bool connectMQTT() {
    bool connected = mqttClient.connect(MQTT_CLIENT_ID);
    if (connected != lastMqttConnectionState) {
        onMqttConnectionChange(connected);
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

void setupMQTT() {
    mqttClient.setServer(MQTT_BROKER_IP, MQTT_BROKER_PORT);
    mqttClient.setCallback(onMQTTCallback);
    mqttClient.setKeepAlive(MQTT_KEEP_ALIVE);
    connectMQTT();
}

void loopMQTT() {
    bool isConnected = mqttClient.connected();
    if (isConnected != lastMqttConnectionState) {
        onMqttConnectionChange(isConnected);
        lastMqttConnectionState = isConnected;
    }

    if (!isConnected) {
        unsigned long currentTime = millis();
        if (currentTime - lastReconnectAttempt > 5000) {
            lastReconnectAttempt = currentTime;
            if (connectMQTT()) {    // Reconnect to MQTT broker
                lastReconnectAttempt = 0;
            }
        }
    } else {
        mqttClient.loop();
    }
}
