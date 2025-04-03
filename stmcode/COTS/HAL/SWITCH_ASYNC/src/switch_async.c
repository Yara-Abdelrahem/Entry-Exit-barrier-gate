#include "switch_async.h"
#include "rcc.h"
static uint8_t SwtichAsync_arrPinStates[NO_OF_SWITCHES] = {0};


void SwtichAsync_vRunnable(void);

void SwitchAsync_vInit(void){

    GPIO_stPinCFG_t Loc_stPinCfg;
    uint8_t i = 0;
    uint64_t Loc_u8GPIOclkMask = 0;

    for(;i<NO_OF_SWITCHES;i++){
        if(SwitchAsync_arr_SWitchCfgs[i].Port == GPIO_enu_GPIOH){
            Loc_u8GPIOclkMask |= RCC_GPIOH;
        }
        else{
            Loc_u8GPIOclkMask |= (1<<SwitchAsync_arr_SWitchCfgs[i].Port);
        }
    }
    RCC_enuCtlPeripheral(Loc_u8GPIOclkMask,RCC_PER_ON);
    for(i=0;i<NO_OF_SWITCHES;i++){
        Loc_stPinCfg.mode = GPIO_enu_INPUT;
        Loc_stPinCfg.port = SwitchAsync_arr_SWitchCfgs[i].Port;
        Loc_stPinCfg.pin = SwitchAsync_arr_SWitchCfgs[i].Pin;
        Loc_stPinCfg.InputType = SwitchAsync_arr_SWitchCfgs[i].InputType;
        Loc_stPinCfg.AltFn = GPIO_enu_NoAF;
        Loc_stPinCfg.OutputType = GPIO_enu_PUSH_PULL;
        Loc_stPinCfg.speed = GPIO_enu_low_speed;
        GPIO_enuCfgPin(&Loc_stPinCfg);
    }

    SCHED_stTaskCfg_t Loc_stRunnable = {
        .CallBack = SwtichAsync_vRunnable,
        .CycleTime = 5,
        .Priority = 80
    };

    SCHED_enuAddRunnable(&Loc_stRunnable);
}

void SwtichAsync_vRunnable(void){
    uint8_t i = 0;
    static uint8_t Loc_u8Counter[NO_OF_SWITCHES] = {0};
    static uint8_t Loc_u8PrevState[NO_OF_SWITCHES] = {0};
    static uint8_t Loc_u8CurrState[NO_OF_SWITCHES] = {0};
    for(;i<NO_OF_SWITCHES;i++){
        
        GPIO_enuGetPinState(
            SwitchAsync_arr_SWitchCfgs[i].Port,
            SwitchAsync_arr_SWitchCfgs[i].Pin,
            &Loc_u8CurrState[i]
        );
        if(Loc_u8PrevState[i]==Loc_u8CurrState[i]){
            Loc_u8Counter[i]++;
            if(Loc_u8Counter[i]==5){
                SwtichAsync_arrPinStates[i] = Loc_u8CurrState[i];
            }
            else{}
        }
        else{
            Loc_u8Counter[i] = 0;
        }
        Loc_u8PrevState[i] = Loc_u8CurrState[i];
    }    
}

SwitchAsync_enuErrorStatus_t SwitchAsync_enuGetState(uint32_t Copy_u32Switch,
uint32_t* Add_u32Ret){

    SwitchAsync_enuErrorStatus_t Loc_enuStatus = SwitchAsync_enu_OK;

    if(Add_u32Ret == NULL){
        Loc_enuStatus = SwitchAsync_enu_NULL_POINTER;
    }
    else if(Copy_u32Switch>(NO_OF_SWITCHES-1)){
        Loc_enuStatus = SwitchAsync_enu_INVALID_SWITCH;
    }
    else{
        if(SwitchAsync_arr_SWitchCfgs[Copy_u32Switch].InputType == GPIO_enu_PU){
            *Add_u32Ret = !SwtichAsync_arrPinStates[Copy_u32Switch];
        }
        else{
            *Add_u32Ret = !SwtichAsync_arrPinStates[Copy_u32Switch];
        }
    }
    return Loc_enuStatus;
}
