#ifndef SWITCH_H_
#define SWITCH_H_
#include "switch_cfg.h"

typedef enum{
    SWITCH_enu_OK,
    SWITCH_enu_INVALID_SW,
    SWITCH_enu_NULL_POINTER,
    SWITCH_enu_NOK
}SWITCH_enuErrorStatus_t;

extern SWITCH_enuErrorStatus_t SWITCH_enuSetCallBack(uint8_t Loc_u8Switch,
SWITCH_callBack_t Add_CallBack);

extern void SWITCH_vInit(void);

#endif