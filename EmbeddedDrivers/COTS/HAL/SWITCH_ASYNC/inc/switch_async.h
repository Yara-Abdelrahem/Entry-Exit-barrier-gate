#ifndef SWITCH_ASYNC_H_
#define SWITCH_ASYNC_H_
#include "switch_async_cfg.h"

typedef enum{
    SwitchAsync_enu_OK,
    SwitchAsync_enu_INVALID_SWITCH,
    SwitchAsync_enu_NULL_POINTER,
    SwitchAsync_enu_NOK
}SwitchAsync_enuErrorStatus_t;

extern void SwitchAsync_vInit(void);

SwitchAsync_enuErrorStatus_t SwitchAsync_enuGetState(uint32_t Copy_u32Switch,
uint32_t* Add_u32Ret);


#endif