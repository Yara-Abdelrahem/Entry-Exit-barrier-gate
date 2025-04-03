#ifndef SWITCH_CFG_H_
#define SWITCH_CFG_H_
#include "GPIO.h"
#include "rcc.h"
#include "nvic.h"
#include "syscfg.h"
#include "exti.h"

#define NO_OF_SWITCHES      3
#define SWITCH_1            0
#define SWITCH_2            1
#define SWITCH_3            2

#define SWITCH_TRIG_MODE_RE     ((uint8_t)0x01) 
#define SWITCH_TRIG_MODE_FE     ((uint8_t)0x02)
typedef void(*SWITCH_callBack_t)(void);


typedef struct{
    GPIO_enuPorts_t Port;
    GPIO_enuPins_t Pin;
    GPIO_enuInputType_t InputType;
    uint8_t TriggerMode;
    SWITCH_callBack_t CallBack;
    uint32_t IrqPriority;
}SWITCH_stCfg_t;

extern SWITCH_stCfg_t SWITCH_arrCfg[NO_OF_SWITCHES];

#endif