#ifndef LED_CFG_H_
#define LED_CFG_H_
#include "GPIO.h"
#include "rcc.h"

#define NO_OF_LEDS      3
#define LED1            0
#define LED2            1
#define LED3            2

typedef enum{
    LED_enu_ACTIVE_HIGH,
    LED_enu_ACTIVE_LOW
}LED_enuConnection_t;

typedef struct{
    GPIO_enuPorts_t port;
    GPIO_enuPins_t pin;
    GPIO_enuOutputType_t outputType;
    GPIO_enuOutputSpeed_t speed;
    LED_enuConnection_t connection;
}LED_stCfg_t;

extern LED_stCfg_t LED_arrCfgs[NO_OF_LEDS];

#endif