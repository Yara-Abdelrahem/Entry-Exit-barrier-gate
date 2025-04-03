#ifndef TIM_2_5_H_
#define TIM_2_5_H_
#include "std_types.h"

typedef void (*TIM2_5_ICU_CB)(uint32_t);


typedef enum{
    TIM2_5_enu_OK,
    TIM2_5_enu_INVALID_TIMER,
    TIM2_5_enu_INVALID_CHANNEL,
    TIM2_5_enu_INVALID_DUTY_CYLCE,
    TIM2_5_enu_INVALID_ICU_PRE,
    TIM2_5_enu_INVALID_ICU_FILTER,
    TIM2_5_enu_INVALID_ICU_MODE,
    TIM2_5_enu_ICU_NOT_CONFIGURED,
    TIM2_5_enu_NULL_POINTER,
    TIM2_5_enu_NOK
}TIM2_5_enuErrorStatus_t;

typedef enum{
    TIM2_5_enuTIM2,
    TIM2_5_enuTIM3,
    TIM2_5_enuTIM4,
    TIM2_5_enuTIM5
}TIM2_5_enuTimers_t;

typedef enum{
    TIM2_5_enuCH1,
    TIM2_5_enuCH2,
    TIM2_5_enuCH3,
    TIM2_5_enuCH4
}TIM2_5_enuChannels_t;

typedef enum{
    TIM2_5_enuICURisingEdge,
    TIM2_5_enuICUFallingEdge,
    TIM2_5_enuICUBothEdges
}TIM2_5_enuICUModes_t;

typedef enum{
    TIM2_5_enuICUNoPre,
    TIM2_5_enuICUPre2,
    TIM2_5_enuICUPre4,
    TIM2_5_enuICUPre8
}TIM2_5_enuICUprescaler_t;

typedef struct{
    TIM2_5_enuTimers_t Timer;
    TIM2_5_enuChannels_t Channel;
    float32_t DutyCycle;
    uint32_t Frequency;
    uint32_t SystemClkFreq;
}TIM2_5_stPWMCfg_t;

typedef struct{
    TIM2_5_enuTimers_t Timer;
    TIM2_5_enuChannels_t Channel;
    TIM2_5_enuICUModes_t Mode;
    TIM2_5_enuICUprescaler_t Prescaler;
    uint8_t Filter;
    TIM2_5_ICU_CB CallBack;
}TIM2_5_stICUCfg_t;

typedef struct{
    TIM2_5_enuTimers_t Timer;
    TIM2_5_enuChannels_t Channel;
    uint32_t PulseWidthUs;    
    uint32_t SystemClockFreq;
}TIM2_5_stPulseCfg_t;

extern TIM2_5_enuErrorStatus_t TIM2_5_enuCfgPWM(TIM2_5_stPWMCfg_t* Add_stCfg);

extern TIM2_5_enuErrorStatus_t TIM2_5_enuSetDutyCycle(TIM2_5_enuTimers_t Timer, 
TIM2_5_enuChannels_t Copy_enuChannel, float32_t Copy_f32DutyCycle);

extern TIM2_5_enuErrorStatus_t TIM2_5_enuICUInit(
TIM2_5_stICUCfg_t* Add_stCfg);

extern TIM2_5_enuErrorStatus_t TIM2_5_enuStartICU(TIM2_5_enuTimers_t Copy_enuTimer,
TIM2_5_enuChannels_t Copy_enuChannel);

extern TIM2_5_enuErrorStatus_t TIM2_5_enuStopICU(TIM2_5_enuTimers_t Copy_enuTimer,
TIM2_5_enuChannels_t Copy_enuChannel);

extern TIM2_5_enuErrorStatus_t TIM2_5_enuGetICUVal(TIM2_5_enuTimers_t Copy_enuTimer,
TIM2_5_enuChannels_t Copy_enuChannel,uint32_t* Add_u32Val);

extern TIM2_5_enuErrorStatus_t TIM2_5_enuICUenableRisingEdge(TIM2_5_enuTimers_t Copy_enuTimer,
TIM2_5_enuChannels_t Copy_enuChannel);

extern TIM2_5_enuErrorStatus_t TIM2_5_enuICUenableFallingEdge(TIM2_5_enuTimers_t Copy_enuTimer,
TIM2_5_enuChannels_t Copy_enuChannel);

extern TIM2_5_enuErrorStatus_t TIM_enuInitPulse(TIM2_5_stPulseCfg_t* Add_stCfg);

extern TIM2_5_enuErrorStatus_t TIM_enuGeneratePulse(TIM2_5_enuTimers_t Copy_enuTimer);

extern TIM2_5_enuErrorStatus_t TIM2_5_enuSetPrescalerVal(TIM2_5_enuTimers_t Copy_enuTimer,
uint32_t Copy_u32Prescaler);

extern TIM2_5_enuErrorStatus_t TIM2_5_enuSetARRVal(TIM2_5_enuTimers_t Copy_enuTimer,
uint32_t Copy_u32ARR);

extern TIM2_5_enuErrorStatus_t TIM2_5_enuUpdateRegs(TIM2_5_enuTimers_t Copy_enuTimer);

extern TIM2_5_enuErrorStatus_t TIM2_5_enuSetCountVal(TIM2_5_enuTimers_t Copy_enuTimer,
uint16_t Copy_u16Val);

extern TIM2_5_enuErrorStatus_t TIM2_5_enuEnableTimer(TIM2_5_enuTimers_t Copy_enuTimer);

extern TIM2_5_enuErrorStatus_t TIM2_5_enuDisableTimer(TIM2_5_enuTimers_t Copy_enuTimer);

#endif