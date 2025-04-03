#include "led_cfg.h"

LED_stCfg_t LED_arrCfgs[NO_OF_LEDS]={
    [LED1] = {  .port=GPIO_enu_GPIOA ,
                .pin=GPIO_enu_PIN0,
                .connection=LED_enu_ACTIVE_HIGH,
                .outputType = GPIO_enu_PUSH_PULL,
                .speed = GPIO_enu_low_speed},

    [LED2] ={   .port=GPIO_enu_GPIOA ,
                .pin=GPIO_enu_PIN1,
                .connection=LED_enu_ACTIVE_HIGH,
                .outputType=GPIO_enu_PUSH_PULL,
                .speed=GPIO_enu_low_speed},

    [LED3] = {
                .port = GPIO_enu_GPIOC,
                .pin = GPIO_enu_PIN13,
                .connection = LED_enu_ACTIVE_LOW,
                .outputType=GPIO_enu_PUSH_PULL,
                .speed=GPIO_enu_low_speed}
};

