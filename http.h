#ifndef __HTTP_H__
#define __HTTP_H__

#include <Arduino.h>

struct Http_Https_Data{
    const char *type;
    String host;
    String url;
    String payload;
    String header;
};
typedef struct Http_Https_Data * Http_Https_Data_t;

int ESP8266_Https_GET_Ftqq(const String &url);
int ESP8266_Http_POST_OneNet(const String &url, const String &payload, const String &api_key);

#endif /* __HTTP_H__ */
