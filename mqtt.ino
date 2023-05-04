#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#include "led.h"
#include "wifi.h"
#include "http.h"
#include "at.h"

/*============================================================
    LED
============================================================*/

static struct LED led = {
    .pin = 2,
    .state = LED_OFF,
    .mode = OUTPUT,
};

/*============================================================
    Serial
============================================================*/

String serialInput = "";
bool serialInputComplete = false;

/*============================================================
    Wi-Fi
============================================================*/

String ssid = "XXX";
String password = "XXX";

/*============================================================
    HTTP
============================================================*/

static String ftqq_SendKey = "XXX";
static struct Http_Https_Data ftqq_Https = {
    .type = "GET",
    .host = "sctapi.ftqq.com",
    .url = "https://" + ftqq_Https.host + "/" + ftqq_SendKey + ".send",
};

static String OneNet_Device_id = "XXX";
static struct Http_Https_Data OneNet_Http = {
    .type = "POST",
    .host = "api.heclouds.com",
    .url = String("http://") + OneNet_Http.host + "/devices/" + OneNet_Device_id + "/datapoints?type=3",
    .payload = "{\"Tem\":0}",
    .header = "XXX",
};

/*============================================================
    at
============================================================*/

At_Err_t at_At_NULL(int argc, char *argv[]);
At_Err_t at_At(int argc, char *argv[]);
At_Err_t at_At_Post(int argc, char *argv[]);
At_Err_t at_AT_WiFi_Connect(int argc, char *argv[]);
At_Err_t at_AT_WiFi_Get_IP(int argc, char *argv[]);

struct At_State atTable[] = {
    { "AT",             AT_TYPE_CMD,        at_At },
    { "AT+GET",         AT_TYPE_NULL,       NULL },
    { "AT+POST",        AT_TYPE_CMD,        at_At_Post },
    { "AT+WIFI",        AT_TYPE_CMD,        at_AT_WiFi_Connect },
    { "AT+IP",          AT_TYPE_CMD,        at_AT_WiFi_Get_IP },
    { AT_LABLE_TAIL,    AT_TYPE_NULL,       at_At_NULL },
};
At_Class at(atTable);

/*============================================================
    main
============================================================*/

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200UL);
    pinMode(led.pin, led.mode);
    digitalWrite(led.pin, led.state);

    // Wi-Fi
    ESP8266_WiFi_STA_Init(ssid, password);

    // https 方糖
    String title = "title=ESP-01S";
    String desp = "desp=" + WiFi.localIP().toString();
    ftqq_Https.url += "?" + title + "&" + desp;
    while (ESP8266_Https_GET_Ftqq(ftqq_Https.url) != HTTP_CODE_OK) delay(5000);
}

bool en_led_flash_task = false;
bool en_OneNet_task = false;

void loop() {
    // put your main code here, to run repeatedly:
    // LED
    if (en_led_flash_task && (millis() % 500 == 0)) {  // T = 0.5s
        en_led_flash_task = false;
        if (led.state == LED_OFF) led.state = LED_ON;
        else led.state = LED_OFF;
        digitalWrite(led.pin, led.state);
    } else if (millis() % 500 != 0) en_led_flash_task = true;

    // Serial AT
    if (serialInputComplete) {
        serialInputComplete = false;
        At_Err_t ret = at.At_Handle(serialInput);
        if (ret != AT_OK) {
            Serial.println();
            Serial.println("[AT] error: " + at.At_ErrorToString(ret));
        }
        serialInput = "";
    }
}

/*============================================================
    Serial
============================================================*/

void serialEvent(void)
{
    while (Serial.available()) {
        char chr = (char)Serial.read();
        if (chr == '\n') {
            serialInputComplete = true;
            continue;
        }
        serialInput += chr;
    }
}

/*============================================================
    at
============================================================*/

At_Err_t at_At_NULL(int argc, char *argv[])
{
    return AT_OK;
}

At_Err_t at_At(int argc, char *argv[])
{
    Serial.println();
    Serial.println("[AT] OK");

    return AT_OK;
}

At_Err_t at_At_Post(int argc, char *argv[])
{
    Serial.println();
    Serial.println("[AT] POST...");
    if (argc != 2) {
        Serial.println();
        Serial.println("[AT] param num don't enough!");
        return AT_ERROR;
    }
    String key = argv[0];
    String val = argv[1];
    OneNet_Http.payload = "{\"" + key + "\":" + val + "}";
    ESP8266_Http_POST_OneNet(OneNet_Http.url, OneNet_Http.payload, OneNet_Http.header);
    Serial.println("[AT] OK");

    return AT_OK;
}

At_Err_t at_AT_WiFi_Connect(int argc, char *argv[])
{
    if (argc != 2) {
        Serial.println();
        Serial.println("[AT] param num don't enough!");
        return AT_ERROR;
    }
    int ret = ESP8266_WiFi_STA_Init(argv[0], argv[1]);
    if (ret) return AT_ERROR;

    return AT_OK;
}

At_Err_t at_AT_WiFi_Get_IP(int argc, char *argv[])
{
    Serial.println();
    Serial.print("[Wi-Fi STA] IP: ");
    Serial.println(WiFi.localIP());

    return AT_OK;
}
