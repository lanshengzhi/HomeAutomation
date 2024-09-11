#ifndef CONFIG_H
#define CONFIG_H

// MQTT broker
#define MQTT_BROKER_IP       "52.194.211.190"
#define MQTT_BROKER_PORT     1883
#define MQTT_BROKER_USERNAME "admin"
#define MQTT_BROKER_PASSWORD "452124@lansy"

// MQTT topics
#define TOPIC_TEMP      "HA-ESP32-01/temp/state"
#define TOPIC_HUMIDITY  "HA-ESP32-01/humidity/state"
#define TOPIC_SUBSCRIBE "ESP32/command"
#define TOPIC_AVAILABILITY "HA-ESP32-01/availability"
#define TOPIC_CONFIG       "homeassistant/sensor/HA-ESP32-01/config"
#define MQTT_BUFFER_SIZE 1024

// Pin definitions
#define LED_INDICATOR           15
#define WIFI_RESET_BUTTON_PIN   26
#define DHT_PIN                 27

#define BUTTON_PREV_PIN         32
#define BUTTON_NEXT_PIN         33
#define BUTTON_UP_PIN           25
#define BUTTON_DOWN_PIN         26


// Other constants
#define LONG_PRESS_TIME 5000

#define MQTT_CLIENT_ID "HA-ESP32-01"
#define MQTT_KEEP_ALIVE 60  // 保持连接的时间（秒）

#endif