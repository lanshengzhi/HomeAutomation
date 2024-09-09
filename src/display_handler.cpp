#include "display_handler.h"
#include "config.h"
#include "dashboard_handler.h"
#include "LiquidCrystal_I2C.h"
#include <Arduino.h>

static LiquidCrystal_I2C lcd(0x27, 16, 2);

void setupDisplay() {
    lcd.init();
    lcd.backlight();
}

void loopDisplay() {
    DeviceInfo *deviceInfo = GetDeviceInfo();
    String tempStr, humStr;

    if (deviceInfo->deviceState.bits.dht == 0) {
        tempStr = "00.00";
        humStr = "00.00";
    } else {
        char tempBuffer[6], humBuffer[6];
        snprintf(tempBuffer, sizeof(tempBuffer), "%5.2f", deviceInfo->temperature);
        snprintf(humBuffer, sizeof(humBuffer), "%5.2f", deviceInfo->humidity);
        tempStr = String(tempBuffer);
        humStr = String(humBuffer);
    }

    String line1 = "T:" + tempStr + "C";
    String line2 = "H:" + humStr + "%";

    // Pad line1 and line2 with spaces to align right side
    while (line1.length() < 12) line1 += " ";
    while (line2.length() < 12) line2 += " ";

    line1 += "WMDB";

    // Create status indicators for line2
    String statusIndicators = "";
    statusIndicators += deviceInfo->deviceState.bits.wifi ? "|" : "_";
    statusIndicators += deviceInfo->deviceState.bits.mqtt ? "|" : "_";
    statusIndicators += deviceInfo->deviceState.bits.dht ? "|" : "_";
    statusIndicators += String(deviceInfo->buttonState.all & 0x0F); // Display last 4 bits

    Serial.println(statusIndicators);

    line2 += statusIndicators;

    lcd.setCursor(0, 0);
    lcd.print(line1);
    lcd.setCursor(0, 1);
    lcd.print(line2);
}
