#include <Arduino.h>


void setup() {
    Serial.begin(115200);
    Serial.println("S");  // ��ʼ�����ڲ���ӡ"Hello World"
}

void loop() {
    Serial.println("Hello World");
    delay(1000);
}