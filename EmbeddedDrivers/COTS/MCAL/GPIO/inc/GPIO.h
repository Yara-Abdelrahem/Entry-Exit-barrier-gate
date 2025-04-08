#ifndef GPIO_H_
#define GPIO_H_
#include "std_types.h"

#define GPIO_HIGH_STATE     ((uint8_t)0x1)
#define GPIO_LOW_STATE      ((uint8_t)0x0)

typedef enum{
    GPIO_enu_OK,
    GPIO_enu_INVALID_PORT,
    GPIO_enu_INVALID_PIN,
    GPIO_enu_INVALID_AF,
    GPIO_enu_INVALID_OUTPUT_TYPE,
    GPIO_enu_INVALID_INPUT_TYPE,
    GPIO_enu_INVALID_MODE,
    GPIO_enu_INVALID_STATE,
    GPIO_enu_NULL_POINTER,
    GPIO_enu_NOK
}GPIO_enuErrorStatus_t;

typedef enum{
    GPIO_enu_GPIOA,
    GPIO_enu_GPIOB,
    GPIO_enu_GPIOC,
    GPIO_enu_GPIOD,
    GPIO_enu_GPIOE,
    GPIO_enu_GPIOH,
}GPIO_enuPorts_t;

typedef enum{
    GPIO_enu_PIN0,
    GPIO_enu_PIN1,
    GPIO_enu_PIN2,
    GPIO_enu_PIN3,
    GPIO_enu_PIN4,
    GPIO_enu_PIN5,
    GPIO_enu_PIN6,
    GPIO_enu_PIN7,
    GPIO_enu_PIN8,
    GPIO_enu_PIN9,
    GPIO_enu_PIN10,
    GPIO_enu_PIN11,
    GPIO_enu_PIN12,
    GPIO_enu_PIN13,
    GPIO_enu_PIN14,
    GPIO_enu_PIN15
}GPIO_enuPins_t;

typedef enum{
    GPIO_enu_AltFn0,
    GPIO_enu_AltFn1,
    GPIO_enu_AltFn2,
    GPIO_enu_AltFn3,
    GPIO_enu_AltFn4,
    GPIO_enu_AltFn5,
    GPIO_enu_AltFn6,
    GPIO_enu_AltFn7,
    GPIO_enu_AltFn8,
    GPIO_enu_AltFn9,
    GPIO_enu_AltFn10,
    GPIO_enu_AltFn11,
    GPIO_enu_AltFn12,
    GPIO_enu_AltFn13,
    GPIO_enu_AltFn14,
    GPIO_enu_AltFn15,
    GPIO_enu_NoAF
}GPIO_enuAltFn_t;

typedef enum{
    GPIO_enu_INPUT,
    GPIO_enu_OUTPUT,
    GPIO_enu_ALT_FN,
    GPIO_enu_ANALOG
}GPIO_enuMode_t; 

typedef enum{
    GPIO_enu_PUSH_PULL,
    GPIO_enu_OPEN_DRAIN
}GPIO_enuOutputType_t;

typedef enum{
    GPIO_enu_NO_PU_PD,
    GPIO_enu_PU,
    GPIO_enu_PD
}GPIO_enuInputType_t;

typedef enum{
    GPIO_enu_low_speed,
    GPIO_enu_med_speed,
    GPIO_enu_high_speed,
    GPIO_enu_vHigh_speed
}GPIO_enuOutputSpeed_t;

typedef struct{
    GPIO_enuPorts_t port;
    GPIO_enuPins_t pin;
    GPIO_enuMode_t mode;
    GPIO_enuAltFn_t AltFn;
    GPIO_enuOutputType_t OutputType;
    GPIO_enuInputType_t InputType;
    GPIO_enuOutputSpeed_t speed;
}GPIO_stPinCFG_t;

extern GPIO_enuErrorStatus_t GPIO_enuCfgPin(GPIO_stPinCFG_t* Add_stPinCFG);

extern GPIO_enuErrorStatus_t GPIO_enuSetPinState(GPIO_enuPorts_t Copy_enuPort,
GPIO_enuPins_t Copy_enuPin,uint8_t Copy_u8State);

extern GPIO_enuErrorStatus_t GPIO_enuGetPinState(GPIO_enuPorts_t Copy_enuPort,
GPIO_enuPins_t Copy_enuPin,uint8_t* Add_u8State);

extern GPIO_enuErrorStatus_t GPIO_enuSetPortVal(GPIO_enuPorts_t Copy_enuPort,
uint16_t Copy_u16val);

extern GPIO_enuErrorStatus_t GPIO_enuGetPortVal(GPIO_enuPorts_t Copy_enuPort,
uint16_t* Add_u16val);


#endif