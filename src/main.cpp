#include <Arduino.h>


void setup() {
    Serial.begin(115200);
    Serial.println("S");  // 初始化串口并打印"Hello World"
}

void loop() {
    Serial.println("Hello World");
    delay(1000);
}