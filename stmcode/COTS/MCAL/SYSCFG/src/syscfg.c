#include "syscfg.h"

#define SYSCFG_BASE             0x40013800UL

#define SYSCFG_MEM_RMP_MASK     0xFFFFFFFC

typedef struct{
    volatile uint32_t MEMRMP;
    volatile uint32_t PMC;
    volatile uint32_t EXTICR[4];
    volatile uint32_t CMPCR;
}SYSCFG_stRegs_t;

SYSCFG_enuErrorStatus_t SYSCFG_enuRemapRegion(SYSCFG_enuMemoryRMP_t Copy_enuRegion){
    
    SYSCFG_enuErrorStatus_t Loc_enuStatus = SYSCFG_enu_OK;
    SYSCFG_stRegs_t* SYSCFG = (SYSCFG_stRegs_t*)SYSCFG_BASE;
    
    if(Copy_enuRegion>SYSCFG_enu_SRAM_RMP){
        Loc_enuStatus = SYSCFG_enu_INVALID_MEM;
    }
    else{
        SYSCFG->MEMRMP = (SYSCFG->MEMRMP & SYSCFG_MEM_RMP_MASK) | Copy_enuRegion;
    }
    return Loc_enuStatus;
}

SYSCFG_enuErrorStatus_t SYSCFG_enuConnectEXTILine(SYSCFG_enuEXTILine_t Loc_enuLine,
SYSCFG_enuPORT_t Loc_enuPort){

    SYSCFG_enuErrorStatus_t Loc_enuStatus = SYSCFG_enu_OK;
    SYSCFG_stRegs_t* SYSCFG = (SYSCFG_stRegs_t*)SYSCFG_BASE;
    uint32_t Loc_u32Reg = Loc_enuLine/4;
    uint32_t Loc_u32RegOff = (Loc_enuLine%4)*4;

    if(Loc_enuLine>SYSCFG_enu_EXTILine15){
        Loc_enuStatus = SYSCFG_enu_INVALID_LINE;
    }
    else if(Loc_enuPort>SYSCFG_enu_PORTH){
        Loc_enuStatus = SYSCFG_enu_INVALID_PORT;
    }
    else{
        if(Loc_enuPort == SYSCFG_enu_PORTH){
            Loc_enuPort += 3;
        }else{}

        SYSCFG->EXTICR[Loc_u32Reg] = (SYSCFG->EXTICR[Loc_u32Reg] & ~(0xF<<Loc_u32RegOff)) |
        (Loc_enuPort << Loc_u32RegOff);
    }
    return Loc_enuStatus;
}

SYSCFG_enuErrorStatus_t SYSCFG_enuGetConnectedPort(SYSCFG_enuEXTILine_t Loc_enuLine,
uint32_t* Add_enuRet){

    SYSCFG_enuErrorStatus_t Loc_enuStatus = SYSCFG_enu_OK;
    SYSCFG_stRegs_t* SYSCFG = (SYSCFG_stRegs_t*)SYSCFG_BASE;
    uint32_t Loc_u32Reg = Loc_enuLine/4;
    uint32_t Loc_u32RegOff = (Loc_enuLine%4)*4;

    if(Add_enuRet==NULL){
        Loc_enuStatus = SYSCFG_enu_NULL_POINTER;
    }
    else if(Loc_enuLine>SYSCFG_enu_EXTILine15){
        Loc_enuStatus = SYSCFG_enu_INVALID_LINE;
    }
    else{
        *Add_enuRet = (SYSCFG->EXTICR[Loc_u32Reg]>>Loc_u32RegOff) & 0xF;
        if(*Add_enuRet==7){
            *Add_enuRet -= 3;
        }
        else{}
    }

    return Loc_enuStatus;
}