#include "systick.h"
#include "rcc.h"

#define SYSTICK_BASE                    0xE000E010UL

#define SYSTICK_PERSCALER_VRF_MASK      0xFFFFFFF6UL
#define SYSTICK_MAX_LOAD_VAL            0x00FFFFFFUL

#define SYSTICK_EN_MASK                 0x00000001UL
#define SYSTICK_DIS_MASK                0xFFFFFFFEUL
#define SYSTICK_PRE_8_MASK              0xFFFFFFFBUL
#define SYSTICK_PRE_1_MASK              0x00000004UL
#define SYSTICK_INT_MASK                0x00000002UL

typedef struct{
    volatile uint32_t CTRL;
    volatile uint32_t LOAD;
    volatile uint32_t VAL;
    volatile const uint32_t CALIB;
}SYSTICK_stRegs_t;

typedef struct{
    SYSTICK_CallBack_t CallBack;
    uint32_t currentCount;
    uint32_t LoadCount;
}SYSTICK_stCallBackCFG_t;

static volatile uint32_t SYSTICK_u32DelayCount = 0;

static SYSTICK_stRegs_t* SYSTICK = (SYSTICK_stRegs_t*)(SYSTICK_BASE);

SYSTICK_stCallBackCFG_t SYSTICK_stCB;

void Systick_vStart(void){
    SYSTICK->CTRL |= SYSTICK_EN_MASK;
}

void Systick_vStop(void){
    SYSTICK->CTRL &= SYSTICK_DIS_MASK;
}

SYSTICK_enuErrorStatus_t Systick_enuInit1Ms(SYSTICK_enuInputPrescaler_t Copy_enuPrescaler){
    
    uint32_t Loc_u32Ticks = 0;
    uint32_t Loc_u32Freq = 0; 
    SYSTICK_enuErrorStatus_t Loc_enuStatus =SYSTICK_enu_OK;

    if(Copy_enuPrescaler&SYSTICK_PERSCALER_VRF_MASK){
        Loc_enuStatus = SYSTICK_enu_INVALID_PRESCALER;
    }
    else{
        RCC_getBusFreq(RCC_enu_AHB,&Loc_u32Freq);

        
        SYSTICK->VAL = 0;
        SYSTICK_stCB.CallBack = NULL;
        SYSTICK_stCB.currentCount=0;
        SYSTICK_stCB.LoadCount = 0;
        if(Copy_enuPrescaler == SYSTICK_enu_PRESCALER_1){
            SYSTICK->CTRL |= SYSTICK_PRE_1_MASK;
        }
        else{
            SYSTICK->CTRL &= SYSTICK_PRE_8_MASK;
            Loc_u32Freq /= SYSTICK_enu_PRESCALER_8;
        }
        Loc_u32Ticks = (Loc_u32Freq/1000) +1;
        SYSTICK->LOAD = Loc_u32Ticks;
        SYSTICK->CTRL |= SYSTICK_INT_MASK;
    }
    
    return Loc_enuStatus;
}

void Systick_vDelayMs(uint32_t Copy_u32Delay){
    SYSTICK_u32DelayCount = Copy_u32Delay;
    while(SYSTICK_u32DelayCount){}
}

SYSTICK_enuErrorStatus_t Systick_enuSetCyclicFnMs(SYSTICK_CallBack_t Add_pCB,
uint32_t delayCycle){
    
    SYSTICK_enuErrorStatus_t Loc_enuStatus = SYSTICK_enu_OK;
    if(Add_pCB == NULL){
        Loc_enuStatus = SYSTICK_enu_NULL_POINTER;
    }

    else{
        SYSTICK_stCB.CallBack = Add_pCB;
        SYSTICK_stCB.LoadCount = delayCycle;
    }
    
    return Loc_enuStatus;
}

void SysTick_Handler(void){
    
    SYSTICK_stCB.currentCount++;
    if (SYSTICK_u32DelayCount > 0) {
        SYSTICK_u32DelayCount--;
    } else {
        SYSTICK_u32DelayCount = 0;
    }   

    if(SYSTICK_stCB.CallBack!=NULL){
        if(SYSTICK_stCB.currentCount==SYSTICK_stCB.LoadCount){
            SYSTICK_stCB.CallBack();
            SYSTICK_stCB.currentCount = 0;
        }
    }   
}