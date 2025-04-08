#include "nvic.h"

#define NVIC_BASE       0xE000E100UL

typedef struct{
    volatile uint32_t ISER[8];   
    volatile const uint32_t RESERVED0[24]; 
    volatile uint32_t ICER[8];    
    volatile const uint32_t RESERVED1[24]; 
    volatile uint32_t ISPR[8];   
    volatile const uint32_t RESERVED2[24];
    volatile uint32_t ICPR[8];   
    volatile const uint32_t RESERVED3[24];
    volatile uint32_t IABR[8];    
    volatile const uint32_t RESERVED4[56];
    volatile uint32_t IPR[60];    
    volatile const uint32_t RESERVED5[644];
    volatile uint32_t STIR;       
}NVIC_stRegs_t;



NVIC_enuErrorStatus_t NVIC_enuEnableIRQn(NVIC_enuIRQn_t Copy_enuIRQn){

    NVIC_enuErrorStatus_t Loc_enuStatus = NVIC_enu_OK;
    NVIC_stRegs_t* NVIC = (NVIC_stRegs_t*)NVIC_BASE;
    uint8_t Loc_u8Bit = Copy_enuIRQn%32;
    uint8_t Loc_u8RegNo = Copy_enuIRQn/32;
    if(Copy_enuIRQn>NVIC_MAX_IRQN){
        Loc_enuStatus = NVIC_enu_INVALID_IRQN;
    }   
    else{
        NVIC->ISER[Loc_u8RegNo] = (1<<Loc_u8Bit);
    }
    return Loc_enuStatus;
};

NVIC_enuErrorStatus_t NVIC_enuDisableIRQn(NVIC_enuIRQn_t Copy_enuIRQn){

    NVIC_enuErrorStatus_t Loc_enuStatus = NVIC_enu_OK;
    NVIC_stRegs_t* NVIC = (NVIC_stRegs_t*)NVIC_BASE;
    uint8_t Loc_u8Bit = Copy_enuIRQn%32;
    uint8_t Loc_u8RegNo = Copy_enuIRQn/32;
    if(Copy_enuIRQn>NVIC_MAX_IRQN){
        Loc_enuStatus = NVIC_enu_INVALID_IRQN;
    }   
    else{
        NVIC->ICER[Loc_u8RegNo] = (1<<Loc_u8Bit);
    }
    return Loc_enuStatus;
}

NVIC_enuErrorStatus_t NVIC_enuSetPendingIRQn(NVIC_enuIRQn_t Copy_enuIRQn){

    NVIC_enuErrorStatus_t Loc_enuStatus = NVIC_enu_OK;
    NVIC_stRegs_t* NVIC = (NVIC_stRegs_t*)NVIC_BASE;
    uint8_t Loc_u8Bit = Copy_enuIRQn%32;
    uint8_t Loc_u8RegNo = Copy_enuIRQn/32;
    if(Copy_enuIRQn>NVIC_MAX_IRQN){
        Loc_enuStatus = NVIC_enu_INVALID_IRQN;
    }   
    else{
        NVIC->ISPR[Loc_u8RegNo] = (1<<Loc_u8Bit);
    }
    return Loc_enuStatus;

}

NVIC_enuErrorStatus_t NVIC_enuClearPendingIRQn(NVIC_enuIRQn_t Copy_enuIRQn){

    NVIC_enuErrorStatus_t Loc_enuStatus = NVIC_enu_OK;
    NVIC_stRegs_t* NVIC = (NVIC_stRegs_t*)NVIC_BASE;
    uint8_t Loc_u8Bit = Copy_enuIRQn%32;
    uint8_t Loc_u8RegNo = Copy_enuIRQn/32;
    if(Copy_enuIRQn>NVIC_MAX_IRQN){
        Loc_enuStatus = NVIC_enu_INVALID_IRQN;
    }   
    else{
        NVIC->ICPR[Loc_u8RegNo] = (1<<Loc_u8Bit);
    }
    return Loc_enuStatus;

}


NVIC_enuErrorStatus_t NVIC_enuGetPendingIRQn(NVIC_enuIRQn_t Copy_enuIRQn,
uint32_t* Add_u32Ret){

    NVIC_enuErrorStatus_t Loc_enuStatus = NVIC_enu_OK;
    NVIC_stRegs_t* NVIC = (NVIC_stRegs_t*)NVIC_BASE;
    uint8_t Loc_u8Bit = Copy_enuIRQn%32;
    uint8_t Loc_u8RegNo = Copy_enuIRQn/32;
    if(Add_u32Ret == NULL){
        Loc_enuStatus = NIVC_enu_NULL_POINTER;
    }
    else if(Copy_enuIRQn>NVIC_MAX_IRQN){
        Loc_enuStatus = NVIC_enu_INVALID_IRQN;
    }
    else{
        *Add_u32Ret = (NVIC->ISER[Loc_u8RegNo]>>Loc_u8Bit) & 0x01UL;
    }

    return Loc_enuStatus;
}

NVIC_enuErrorStatus_t NVIC_enuSetPriorityIRQn(NVIC_enuIRQn_t Copy_enuIRQn,
uint8_t Copy_u8Priority){
    
    NVIC_enuErrorStatus_t Loc_enuStatus = NVIC_enu_OK;
    NVIC_stRegs_t* NVIC = (NVIC_stRegs_t*)NVIC_BASE;
    uint8_t Loc_u8RegNo = Copy_enuIRQn/4;
    uint8_t Loc_8RegOffset = (Copy_enuIRQn%4)*8;
    if(Copy_enuIRQn>NVIC_MAX_IRQN){
        Loc_enuStatus = NVIC_enu_INVALID_IRQN;
    }
    else{
        NVIC->IPR[Loc_u8RegNo] = (NVIC->IPR[Loc_u8RegNo] & (~(0xFF<<Loc_8RegOffset))) |
        (Copy_u8Priority<<Loc_8RegOffset);
    }
    return Loc_enuStatus;
}

NVIC_enuErrorStatus_t NVIC_enuGetPriorityIRQn(NVIC_enuIRQn_t Copy_enuIRQn,
uint32_t* Add_u32Ret){
    
    NVIC_enuErrorStatus_t Loc_enuStatus = NVIC_enu_OK;
    NVIC_stRegs_t* NVIC = (NVIC_stRegs_t*)NVIC_BASE;
    uint8_t Loc_u8RegNo = Copy_enuIRQn/4;
    uint8_t Loc_8RegOffset = (Copy_enuIRQn%4)*8;
    if(Add_u32Ret==NULL){
        Loc_enuStatus = NIVC_enu_NULL_POINTER;
    }
    else if(Copy_enuIRQn>NVIC_MAX_IRQN){
        Loc_enuStatus = NVIC_enu_INVALID_IRQN;
    }
    else{
        *Add_u32Ret = (NVIC->IPR[Loc_u8RegNo]>>Loc_8RegOffset)&0xFF;
    }
    return Loc_enuStatus;
}

NVIC_enuErrorStatus_t NVIC_enuSetGroupPriority(NVIC_enuIRQn_t Copy_enuIRQn,
uint8_t Copy_u8Priority){

    NVIC_enuErrorStatus_t Loc_enuStatus = NVIC_enu_OK;
    NVIC_stRegs_t* NVIC = (NVIC_stRegs_t*)NVIC_BASE;
    uint8_t Loc_u8RegNo = Copy_enuIRQn/4;
    uint8_t Loc_8RegOffset = (Copy_enuIRQn%4)*8 + 4;
    if(Copy_enuIRQn>NVIC_MAX_IRQN){
        Loc_enuStatus = NVIC_enu_INVALID_IRQN;
    }
    else if(Copy_u8Priority>NVIC_MAX_GROUP_PRIO){
        Loc_enuStatus = NVIC_enu_INVALID_PRIORITY;
    }
    else{
        NVIC->IPR[Loc_u8RegNo] = (NVIC->IPR[Loc_u8RegNo] & (~(0xF<<Loc_8RegOffset))) |
        (Copy_u8Priority<<Loc_8RegOffset);
    }

    return Loc_enuStatus;
}

NVIC_enuErrorStatus_t NVIC_enuGetGroupPriorityIRQn(NVIC_enuIRQn_t Copy_enuIRQn,
uint32_t* Add_u32Ret){
    
    NVIC_enuErrorStatus_t Loc_enuStatus = NVIC_enu_OK;
    NVIC_stRegs_t* NVIC = (NVIC_stRegs_t*)NVIC_BASE;
    uint8_t Loc_u8RegNo = Copy_enuIRQn/4;
    uint8_t Loc_8RegOffset = (Copy_enuIRQn%4)*8+4;
    if(Add_u32Ret==NULL){
        Loc_enuStatus = NIVC_enu_NULL_POINTER;
    }
    else if(Copy_enuIRQn>NVIC_MAX_IRQN){
        Loc_enuStatus = NVIC_enu_INVALID_IRQN;
    }
    else{
        *Add_u32Ret = (NVIC->IPR[Loc_u8RegNo]>>Loc_8RegOffset)&0xF;
    }
    return Loc_enuStatus;
}


NVIC_enuErrorStatus_t NVIC_enuSetSubGroupPriority(NVIC_enuIRQn_t Copy_enuIRQn,
uint8_t Copy_u8Priority){

    NVIC_enuErrorStatus_t Loc_enuStatus = NVIC_enu_OK;
    NVIC_stRegs_t* NVIC = (NVIC_stRegs_t*)NVIC_BASE;
    uint8_t Loc_u8RegNo = Copy_enuIRQn/4;
    uint8_t Loc_8RegOffset = (Copy_enuIRQn%4)*8;
    if(Copy_enuIRQn>NVIC_MAX_IRQN){
        Loc_enuStatus = NVIC_enu_INVALID_IRQN;
    }
    else if(Copy_u8Priority>NVIC_MAX_SUB_GROUP_PRIO){
        Loc_enuStatus = NVIC_enu_INVALID_PRIORITY;
    }
    else{
        NVIC->IPR[Loc_u8RegNo] = (NVIC->IPR[Loc_u8RegNo] & (~(0xF<<Loc_8RegOffset))) |
        (Copy_u8Priority<<Loc_8RegOffset);
    }

    return Loc_enuStatus;
}

NVIC_enuErrorStatus_t NVIC_enuGetSubGroupPriorityIRQn(NVIC_enuIRQn_t Copy_enuIRQn,
uint32_t* Add_u32Ret){
    
    NVIC_enuErrorStatus_t Loc_enuStatus = NVIC_enu_OK;
    NVIC_stRegs_t* NVIC = (NVIC_stRegs_t*)NVIC_BASE;
    uint8_t Loc_u8RegNo = Copy_enuIRQn/4;
    uint8_t Loc_8RegOffset = (Copy_enuIRQn%4)*8;
    if(Add_u32Ret==NULL){
        Loc_enuStatus = NIVC_enu_NULL_POINTER;
    }
    else if(Copy_enuIRQn>NVIC_MAX_IRQN){
        Loc_enuStatus = NVIC_enu_INVALID_IRQN;
    }
    else{
        *Add_u32Ret = (NVIC->IPR[Loc_u8RegNo]>>Loc_8RegOffset)&0xF;
    }
    return Loc_enuStatus;
}

NVIC_enuErrorStatus_t NVIC_enuTriggerInterrupt(NVIC_enuIRQn_t Copy_enuIRQn){
    
    NVIC_enuErrorStatus_t Loc_enuStatus = NVIC_enu_OK;
    NVIC_stRegs_t* NVIC = (NVIC_stRegs_t*)NVIC_BASE;
    if(Copy_enuIRQn>NVIC_MAX_IRQN){
        Loc_enuStatus = NVIC_enu_INVALID_IRQN;
    }
    else{
        NVIC->STIR = Copy_enuIRQn;
    }
    return Loc_enuStatus;
}