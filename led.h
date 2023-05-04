#ifndef __LED_H__
#define __LED_H__

#include <Arduino.h>

enum LED_State{
  LED_OFF = HIGH,
  LED_ON = LOW,
};
typedef enum LED_State LED_State_t;

struct LED{
    uint8_t pin;
    LED_State_t state;
    uint8_t mode;
};
typedef struct LED * LED_t;

#endif /* __LED_H__ */
