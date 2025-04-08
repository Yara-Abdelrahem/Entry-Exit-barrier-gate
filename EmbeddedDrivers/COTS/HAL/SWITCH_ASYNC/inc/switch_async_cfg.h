#ifndef SWITCH_ASYNC_CFG_H_ 
#define SWITCH_ASYNC_CFG_H_
#include "scheduler.h"
#include "rcc.h"
#include "GPIO.h"

#define NO_OF_SWITCHES      3

#define SWITCH1             0
#define SWITCH2             1
#define SWITCH3             2

typedef struct{
    GPIO_enuPorts_t Port;
    GPIO_enuPins_t Pin;
    GPIO_enuInputType_t InputType;
}SwitchAsync_stSwitchCfg_t;

extern SwitchAsync_stSwitchCfg_t SwitchAsync_arr_SWitchCfgs[NO_OF_SWITCHES];

#endif