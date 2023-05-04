#include "wifi.h"
#include <ESP8266WiFi.h>

int ESP8266_WiFi_STA_Init(const String &ssid, const String &password)
{
    uint8_t err_time = 0;
    
    Serial.println();
    Serial.println();
    Serial.print("[Wi-Fi STA] Connecting to ");
    Serial.println(ssid);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        err_time++;
        if (err_time >= 100) return -1;
        delay(100);
        Serial.print(".");
    }
    Serial.println();
    Serial.println("[Wi-Fi STA] WiFi connected!");
    Serial.print("[Wi-Fi STA] IP: ");
    Serial.println(WiFi.localIP());

    return 0;
}
