#include "switch_cfg.h"

SWITCH_stCfg_t SWITCH_arrCfg[NO_OF_SWITCHES] = {
    [SWITCH_1] = {.Port = GPIO_enu_GPIOA 
    , .Pin = GPIO_enu_PIN0,
    .InputType=GPIO_enu_PU,
    .TriggerMode=SWITCH_TRIG_MODE_FE,
    .CallBack = NULL,
    .IrqPriority = 0xF0
    },

    [SWITCH_2] = {.Port = GPIO_enu_GPIOA, 
    .Pin = GPIO_enu_PIN1,
    .InputType=GPIO_enu_PD,
    .TriggerMode=SWITCH_TRIG_MODE_RE,
    .CallBack = NULL,
    .IrqPriority= 0xF0
    },
    
    [SWITCH_3] = {.Port = GPIO_enu_GPIOA, 
    .Pin = GPIO_enu_PIN2,
    .InputType=GPIO_enu_NO_PU_PD,
    .TriggerMode=SWITCH_TRIG_MODE_FE,
    .CallBack = NULL,
    .IrqPriority= 0xF0
    }
};