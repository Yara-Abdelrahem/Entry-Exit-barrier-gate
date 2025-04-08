#include "exti.h"

#define EXTI_BASE               0x40013C00UL

#define EXTI_LINE_VRF_MASK      0xFF980000UL

#define EXTI_CALLBACK_ARR_SIZE          23

#define EXTI_MAX_CALLBACK_INDEX         22

typedef struct{
    volatile uint32_t IMR;
    volatile uint32_t EMR;
    volatile uint32_t RTSR;
    volatile uint32_t FTSR;
    volatile uint32_t SWIER;
    volatile uint32_t PR;
}EXTI_stRegs_t;

static EXTI_stRegs_t* EXTI = (EXTI_stRegs_t*)EXTI_BASE;


static EXTI_CallBack_t EXTI_arrCallBack[EXTI_CALLBACK_ARR_SIZE] = {
    NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
    NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
    NULL,NULL,NULL,NULL,NULL,NULL,NULL
};


EXTI_enuErrorStatus_t EXTI_enuCtlLine(uint32_t Copy_u32Line,uint32_t Copy_u32State,
uint8_t Copy_u8INT_Event){
    
    EXTI_enuErrorStatus_t Loc_enuStatus = EXTI_enu_OK;
    
    if(Copy_u32Line&EXTI_LINE_VRF_MASK){
        Loc_enuStatus = EXTI_enu_INVALID_LINE;
    }
    else if(Copy_u32State&&(~Copy_u32State)){
        Loc_enuStatus = EXTI_enu_INVALID_STATE;
    }
    else{
        switch (Copy_u8INT_Event){
            case EXTI_LINE_INT:
                EXTI->IMR = (EXTI->IMR & ~Copy_u32Line) | 
                (Copy_u32State & Copy_u32Line); 
                break;

            case EXTI_LINE_EVENT:
                EXTI->EMR = (EXTI->EMR & ~Copy_u32Line) | 
                (Copy_u32State & Copy_u32Line); 
                break;

            default:
                Loc_enuStatus = EXTI_enu_INVALID_IN;
                break;
        }
     }
    
    return Loc_enuStatus;
}

EXTI_enuErrorStatus_t EXTI_enuSetTrigMode(uint32_t Copy_u32Line,uint8_t Copy_u8Mode){

    EXTI_enuErrorStatus_t Loc_enuStatus = EXTI_enu_OK;

    if(Copy_u32Line&EXTI_LINE_VRF_MASK){
        Loc_enuStatus = EXTI_enu_INVALID_LINE;
    }
    else{
        switch (Copy_u8Mode){

            case EXTI_MODE_NONE:
                EXTI->FTSR &= ~Copy_u32Line;
                EXTI->RTSR &= ~Copy_u32Line; 
                break;

            case EXTI_MODE_FALLING_EDGE:
                EXTI->FTSR |= Copy_u32Line;
                EXTI->RTSR &= ~Copy_u32Line; 
                break;

            case EXTI_MODE_RISING_EDGE:
                EXTI->FTSR &= ~Copy_u32Line;
                EXTI->RTSR |= Copy_u32Line; 
                break;

            case EXTI_MODE_RF_EDGES:
                EXTI->FTSR |= Copy_u32Line;
                EXTI->FTSR |= Copy_u32Line; 
                break;
        
            default:
                Loc_enuStatus = EXTI_enu_INVALID_MODE;
                break;
        }
    }

    return Loc_enuStatus;
}

EXTI_enuErrorStatus_t EXTI_enuSetPending(uint32_t Copy_u32Line){

    EXTI_enuErrorStatus_t Loc_enuStatus = EXTI_enu_OK;

    if(Copy_u32Line&EXTI_LINE_VRF_MASK){
        Loc_enuStatus = EXTI_enu_INVALID_LINE;
    }
    else{
        EXTI->SWIER |=Copy_u32Line;
    }
    return Loc_enuStatus;
}

EXTI_enuErrorStatus_t EXTI_enuClearPending(uint32_t Copy_u32Line){

    EXTI_enuErrorStatus_t Loc_enuStatus = EXTI_enu_OK;

    if(Copy_u32Line&EXTI_LINE_VRF_MASK){
        Loc_enuStatus = EXTI_enu_INVALID_LINE;
    }
    else{
        EXTI->PR |= Copy_u32Line;
    }
    return Loc_enuStatus;
}

EXTI_enuErrorStatus_t EXTI_enuGetPending(uint32_t Copy_u32Line,uint8_t* Add_u8Ret){

    EXTI_enuErrorStatus_t Loc_enuStatus = EXTI_enu_OK;

    if(Add_u8Ret == NULL){
        Loc_enuStatus  = EXTI_enu_NULL_POINTER;
    }
    else if(Copy_u32Line&EXTI_LINE_VRF_MASK){
        Loc_enuStatus = EXTI_enu_INVALID_LINE;
    }
    else{
        *Add_u8Ret = (EXTI->PR & Copy_u32Line) != 0;
    }
    return Loc_enuStatus;
}

EXTI_enuErrorStatus_t EXTI_enuSetCallBack(EXTI_enuCallBackLine_t Copy_enuLine ,
EXTI_CallBack_t add_CallBack){
    
    EXTI_enuErrorStatus_t Loc_enuStatus = EXTI_enu_OK;

    if(add_CallBack==NULL){
        Loc_enuStatus = EXTI_enu_NULL_POINTER;
    }
    else if((1<<Copy_enuLine)&EXTI_LINE_VRF_MASK){
        Loc_enuStatus = EXTI_enu_INVALID_LINE;
    }
    else{
        EXTI_arrCallBack[Copy_enuLine] = add_CallBack;
    }

    return Loc_enuStatus;
}

void EXTI0_IRQHandler(void){
    if(EXTI->PR & EXTI_LINE0_MASK){
        EXTI->PR |=EXTI_LINE0_MASK;
        if(EXTI_arrCallBack[0]!=NULL){
            EXTI_arrCallBack[0]();
        }
    }
}

void EXTI1_IRQHandler(void){
    if(EXTI->PR & EXTI_LINE1_MASK){
        EXTI->PR |=EXTI_LINE1_MASK;
        if(EXTI_arrCallBack[1]!=NULL){
            EXTI_arrCallBack[1]();
        }
    }
}

void EXTI2_IRQHandler(void){
    if(EXTI->PR & EXTI_LINE2_MASK){
        EXTI->PR |=EXTI_LINE2_MASK;
        if(EXTI_arrCallBack[2]!=NULL){
            EXTI_arrCallBack[2]();
        }
    }
}

void EXTI3_IRQHandler(void){
    if(EXTI->PR & EXTI_LINE3_MASK){
        EXTI->PR |=EXTI_LINE3_MASK;
        if(EXTI_arrCallBack[3]!=NULL){
            EXTI_arrCallBack[3]();
        }
    }
}

void EXTI4_IRQHandler(void){
    if(EXTI->PR & EXTI_LINE4_MASK){
        EXTI->PR |=EXTI_LINE4_MASK;
        if(EXTI_arrCallBack[4]!=NULL){
            EXTI_arrCallBack[4]();
        }
    }
}

void EXTI9_5_IRQHandler(void){

    if(EXTI->PR & EXTI_LINE5_MASK){
        EXTI->PR |=EXTI_LINE5_MASK;
        if(EXTI_arrCallBack[5]!=NULL){
            EXTI_arrCallBack[5]();
        }
    }

    if(EXTI->PR & EXTI_LINE6_MASK){
        EXTI->PR |=EXTI_LINE6_MASK;
        if(EXTI_arrCallBack[6]!=NULL){
            EXTI_arrCallBack[6]();
        }
    }

    if(EXTI->PR & EXTI_LINE7_MASK){
        EXTI->PR |=EXTI_LINE7_MASK;
        if(EXTI_arrCallBack[7]!=NULL){
            EXTI_arrCallBack[7]();
        }
    }

    if(EXTI->PR & EXTI_LINE8_MASK){
        EXTI->PR |=EXTI_LINE8_MASK;
        if(EXTI_arrCallBack[8]!=NULL){
            EXTI_arrCallBack[8]();
        }
    }

    if(EXTI->PR & EXTI_LINE9_MASK){
        EXTI->PR |=EXTI_LINE9_MASK;
        if(EXTI_arrCallBack[9]!=NULL){
            EXTI_arrCallBack[9]();
        }
    }
}

void EXTI15_10_IRQHandler(void){

    if(EXTI->PR & EXTI_LINE10_MASK){
        EXTI->PR |=EXTI_LINE10_MASK;
        if(EXTI_arrCallBack[10]!=NULL){
            EXTI_arrCallBack[10]();
        }
    }

    if(EXTI->PR & EXTI_LINE11_MASK){
        EXTI->PR |=EXTI_LINE11_MASK;
        if(EXTI_arrCallBack[11]!=NULL){
            EXTI_arrCallBack[11]();
        }
    }

    if(EXTI->PR & EXTI_LINE12_MASK){
        EXTI->PR |=EXTI_LINE12_MASK;
        if(EXTI_arrCallBack[12]!=NULL){
            EXTI_arrCallBack[12]();
        }
    }

    if(EXTI->PR & EXTI_LINE13_MASK){
        EXTI->PR |=EXTI_LINE13_MASK;
        if(EXTI_arrCallBack[13]!=NULL){
            EXTI_arrCallBack[13]();
        }
    }

    if(EXTI->PR & EXTI_LINE14_MASK){
        EXTI->PR |=EXTI_LINE14_MASK;
        if(EXTI_arrCallBack[14]!=NULL){
            EXTI_arrCallBack[14]();
        }
    }

    if(EXTI->PR & EXTI_LINE15_MASK){
        EXTI->PR |=EXTI_LINE15_MASK;
        if(EXTI_arrCallBack[15]!=NULL){
            EXTI_arrCallBack[15]();
        }
    }
}