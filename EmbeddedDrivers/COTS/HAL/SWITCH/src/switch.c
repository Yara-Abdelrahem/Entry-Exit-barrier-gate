#include "switch.h"

NVIC_enuIRQn_t arrIRQnMap[7] = {
    NVIC_enuEXTI0,
    NVIC_enuEXTI1,
    NVIC_enuEXTI2,
    NVIC_enuEXTI3,
    NVIC_enuEXTI4,
    NVIC_enuEXTI9_5,
    NVIC_enuEXTI15_10
};


SWITCH_enuErrorStatus_t SWITCH_enuSetCallBack(uint8_t Loc_u8Switch,
SWITCH_callBack_t Add_CallBack){
    
    SWITCH_enuErrorStatus_t Loc_enuStatus = SWITCH_enu_OK;
   
    if(Add_CallBack==NULL){
        Loc_enuStatus = SWITCH_enu_NULL_POINTER;
    }
    else if(Loc_u8Switch>15){
        Loc_enuStatus = SWITCH_enu_INVALID_SW;
    }
    else{
        EXTI_enuSetCallBack((EXTI_enuCallBackLine_t)SWITCH_arrCfg[Loc_u8Switch].Pin,Add_CallBack);
    }
    return Loc_enuStatus;
}


void SWITCH_vInit(void){
    
    uint8_t i = 0;
    uint64_t Loc_u8GPIOclkMask = 0;
    GPIO_stPinCFG_t Loc_stPinCfg = {0};
    uint8_t Loc_u8IRQn =0;
    for(;i<NO_OF_SWITCHES;i++){
        if(SWITCH_arrCfg[i].Port == GPIO_enu_GPIOH){
            Loc_u8GPIOclkMask |= RCC_GPIOH;
        }
        else{
            Loc_u8GPIOclkMask |= (1<<SWITCH_arrCfg[i].Port);
        }
    }
    
    RCC_enuCtlPeripheral(Loc_u8GPIOclkMask,RCC_PER_ON);
    RCC_enuCtlPeripheral(RCC_SYSCFG,RCC_PER_ON);
    
    for(i=0;i<NO_OF_SWITCHES;i++){
        Loc_stPinCfg.port = SWITCH_arrCfg[i].Port;
        Loc_stPinCfg.pin = SWITCH_arrCfg[i].Pin;
        Loc_stPinCfg.mode = GPIO_enu_INPUT;
        Loc_stPinCfg.InputType = SWITCH_arrCfg[i].InputType;
        Loc_stPinCfg.OutputType = GPIO_enu_PUSH_PULL;
        Loc_stPinCfg.AltFn = GPIO_enu_NoAF;
        Loc_stPinCfg.speed = GPIO_enu_low_speed;
        GPIO_enuCfgPin(&Loc_stPinCfg);

        SYSCFG_enuConnectEXTILine((SYSCFG_enuEXTILine_t)SWITCH_arrCfg[i].Pin,
        (SYSCFG_enuPORT_t)SWITCH_arrCfg[i].Port);

        EXTI_enuCtlLine(1<<SWITCH_arrCfg[i].Pin,EXTI_LINE_STATE_EN,
        EXTI_LINE_INT);  

        EXTI_enuSetTrigMode(1<<SWITCH_arrCfg[i].Pin,
        SWITCH_arrCfg[i].TriggerMode);

        if(SWITCH_arrCfg[i].Pin<5){
            Loc_u8IRQn = arrIRQnMap[SWITCH_arrCfg[i].Pin];
        }
        else if(SWITCH_arrCfg[i].Pin<10){
            Loc_u8IRQn = arrIRQnMap[5];
        }
        else{
            Loc_u8IRQn = arrIRQnMap[6];
        }
        NVIC_enuClearPendingIRQn(Loc_u8IRQn);
        NVIC_enuEnableIRQn(Loc_u8IRQn);
        NVIC_enuSetPriorityIRQn(Loc_u8IRQn,SWITCH_arrCfg[i].IrqPriority);   

        SWITCH_enuSetCallBack(SWITCH_arrCfg[i].Pin,
        SWITCH_arrCfg[i].CallBack);  
    }
}