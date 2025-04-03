#include "led.h"

void LED_vInit(void){

    GPIO_stPinCFG_t LED_stPinCFG;
    uint8_t Loc_arrHash[6] = {0};
    uint64_t Loc_u64ClkMask = 0;

    for(int i=0;i<NO_OF_LEDS;i++){
        if(Loc_arrHash[LED_arrCfgs[i].port]){
            continue;
        }
        else{
            Loc_arrHash[LED_arrCfgs[i].port]++;
        }
        switch(LED_arrCfgs[i].port){
            case GPIO_enu_GPIOA:
                Loc_u64ClkMask = RCC_GPIOA;
                break;
            case GPIO_enu_GPIOB:
                Loc_u64ClkMask = RCC_GPIOB;
                break;
            case GPIO_enu_GPIOC:
                Loc_u64ClkMask = RCC_GPIOC;
                break;
            case GPIO_enu_GPIOD:
                Loc_u64ClkMask = RCC_GPIOD;
                break;
            case GPIO_enu_GPIOE:
                Loc_u64ClkMask = RCC_GPIOE;
                break;
            case GPIO_enu_GPIOH:
                Loc_u64ClkMask = RCC_GPIOH;
                break;
            default:
                break;
        }
        RCC_enuCtlPeripheral(Loc_u64ClkMask,RCC_PER_ON);
    }

    for(int i=0;i<NO_OF_LEDS;i++){
        LED_stPinCFG.port = LED_arrCfgs[i].port;
        LED_stPinCFG.pin = LED_arrCfgs[i].pin;
        LED_stPinCFG.mode = GPIO_enu_OUTPUT;
        LED_stPinCFG.AltFn = GPIO_enu_NoAF;
        LED_stPinCFG.speed = LED_arrCfgs[i].speed;
        LED_stPinCFG.InputType = GPIO_enu_NO_PU_PD;
        LED_stPinCFG.OutputType = LED_arrCfgs[i].outputType;
        GPIO_enuCfgPin(&LED_stPinCFG);
    }
}

LED_enuErrorStatus_t LED_enuCtlLED(uint8_t Copy_u8LED,uint8_t Copy_u8State){

    LED_enuErrorStatus_t Loc_enuStatus = LED_enu_OK;
    GPIO_enuErrorStatus_t Loc_enuGPIOStatus = GPIO_enu_OK;
    if(Copy_u8LED>(NO_OF_LEDS-1)){
        Loc_enuStatus = LED_enu_INVALID_LED;
    }
    else if(Copy_u8State>LED_HIGH){
        Loc_enuStatus = LED_enu_INVALID_STATE;
    }
    else{
        uint32_t Loc_u32Port = LED_arrCfgs[Copy_u8LED].port;
        uint32_t Loc_u32Pin = LED_arrCfgs[Copy_u8LED].pin;
        uint8_t Loc_u8State = Copy_u8State^LED_arrCfgs[Copy_u8LED].connection;
        Loc_enuGPIOStatus = GPIO_enuSetPinState(Loc_u32Port,Loc_u32Pin,Loc_u8State);
        if(Loc_enuGPIOStatus!=GPIO_enu_OK){
            Loc_enuStatus = LED_enu_CFG_STATE_ERR;
        }
        else{}
    }
    return Loc_enuStatus;
}