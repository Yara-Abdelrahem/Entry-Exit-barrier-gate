#ifndef LED_H_
#define LED_H_
#include "led_cfg.h"

#define LED_HIGH    ((uint8_t)0x1)
#define LED_LOW     ((uint8_t)0x0)

typedef enum{
    LED_enu_OK,
    LED_enu_INVALID_STATE,
    LED_enu_INVALID_LED,
    LED_enu_CFG_STATE_ERR,
    LED_enu_NOK
}LED_enuErrorStatus_t;

extern void LED_vInit(void);
extern LED_enuErrorStatus_t LED_enuCtlLED(uint8_t Copy_u8LED,
uint8_t Copy_u8State);

#endif