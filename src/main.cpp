#include "task_scheduler.h"
#include "config.h"
#include "dashboard_handler.h"
#include "wifi_handler.h"
#include "mqtt_handler.h"
#include "sensor_handler.h"
#include "display_handler.h"
#include "button_handler.h"
#include "Indicator_handler.h"
#include <Arduino.h>

TaskScheduler scheduler;

void setup() {
    Serial.begin(115200);

    setupDashboard();
    setupIndicator();
    setupWiFi();
    setupMQTT();
    setupSensor();
    setupDisplay();
    setupButtons();

    scheduler.addTask(loopIndicator, 1000);  // Every 1 second
    scheduler.addTask(loopButtons, 20);      // Every 20ms
    scheduler.addTask(loopSensor, 5000);    // Every 5 seconds
    scheduler.addTask(loopMQTT, 10);        // Every 10 ms
    scheduler.addTask(loopDisplay, 10);    // Every 10 ms
    scheduler.addTask(loopDashboard, 10);  // Every 10 ms
}

void loop() {
    scheduler.run();
}