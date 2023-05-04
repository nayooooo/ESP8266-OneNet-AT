#include "http.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>

#include "ftqq_cert.h"

/**
 * GET ftqq
 * @param url String& default: "https://api.heclouds.com/SendKey.send?title=xxx&desp=xxx"
 * @return httpCode
 */
int ESP8266_Https_GET_Ftqq(const String &url)
{
    HTTPClient https;

    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    client->setFingerprint(fingerprint_sctapi_ftqq_com);  // need a fingerprint

    Serial.println();
    Serial.print("[HTTPS] begin...\n");
    https.begin(*client, url);
    Serial.println("[HTTPS] GET...");
    int httpCode = https.GET();
    if (httpCode > 0) {
        Serial.printf("[HTTPS] GET... code: %d", httpCode);
        Serial.println();
        if (httpCode == HTTP_CODE_OK) {
            String payload = https.getString();
            Serial.println(payload);
        }
    } else {
        Serial.printf("[HTTPS] GET... failed, error: %s", https.errorToString(httpCode).c_str());
        Serial.println();
    }
    https.end();
    return httpCode;
}

/**
 * POST OneNet
 * @param url String& default: "/devices/device_id/datapoints?type=3"
 * @param payload String& default: payload
 * @param api_key String& default: api_key
 * @return httpCode
 */
int ESP8266_Http_POST_OneNet(const String &url, const String &payload, const String &api_key)
{
    WiFiClient client;
    HTTPClient http;

    Serial.println();
    Serial.println("[HTTP] begin...");
    http.begin(client, url);
    http.addHeader("api-key", api_key);
    int httpCode = http.POST(payload);
    Serial.println("[HTTP] Content-Length:" + String(payload.length()));
    Serial.println("[HTTP] " + String(payload));
    if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] POST... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK) {
            const String& inPayload = http.getString();
            Serial.println("received payload:\n<<");
            Serial.println(inPayload);
            Serial.println(">>");
        }
    } else {
        Serial.printf("[HTTP] POST... failed, error: %s\r\n", http.errorToString(httpCode).c_str());
        Serial.println();
    }
    http.end();
    return httpCode;
}
