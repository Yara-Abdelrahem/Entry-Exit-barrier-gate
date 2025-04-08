#include "flash.h"

#define FLASH_BASE                      0x40023C00UL

#define FLASH_KEY_1                     0x45670123UL
#define FLASH_KEY_2                     0xCDEF89ABUL

#define FLASH_OPT_KEY_1                 0x08192A3BUL
#define FLASH_OPT_KEY_2                 0x4C5D6E7FUL 

#define FLASH_CR_LOCK_MASK              0x80000000UL
#define FLASH_CR_CLR_SEC_MASK           0xFFFFFF87UL
#define FLASH_OPTCR_LOCK_MASK           0x00000001UL
#define FLASH_P_SIZE_MASK               0x00000200UL
#define FLASH_P_SIZE_CLR_MASK           0xFFFFFCFFUL           
#define FLASH_EOPIE_MASK                0x01000000UL
#define FLASH_ERRIE_MASK                0x02000000UL
#define FLASH_CR_SECTOR_VRF_MASK        0xFFFFFF87UL
#define FLASH_CR_START_MASK             0x00010000UL
#define FLASH_CR_SEC_ERASE_EN           0x00000002UL
#define FLASH_CR_MASS_ERASE_EN          0x00000004UL        
#define FLASH_CR_PROGRAM_EN             0x00000001UL

#define FLASH_SR_EOP_MASK               0x00000001UL
#define FLASH_SR_OPERR_MASK             0x00000002UL
#define FLASH_SR_WRPERR_MASK            0x00000010UL
#define FLASH_SR_PGAERR_MASK            0x00000020UL
#define FLASH_SR_PGPERR_MASK            0x00000040UL
#define FLASH_SR_PGSERR_MASK            0x00000080UL
#define FLASH_SR_RDERR_MASK             0x00000100UL
#define FLASH_SR_BUSY_MASK              0x00010000UL
#define FLASH_SR_CLR_ERR_MASK           0x000001F2UL


typedef struct{
    volatile uint32_t ACR;
    volatile uint32_t KEYR;
    volatile uint32_t OPTKEYR;
    volatile uint32_t SR;
    volatile uint32_t CR;
    volatile uint32_t OPTCR;
}FLASH_stRegs_t;

static volatile FLASH_stRegs_t* FLASH  = (volatile FLASH_stRegs_t*)(FLASH_BASE); 

static FLASH_CB_t FLASH_pCB = NULL;

void FLASH_vUnlock(void){
    FLASH->KEYR = FLASH_KEY_1;
    FLASH->KEYR = FLASH_KEY_2;
}

void FLASH_vLock(void){
    FLASH->KEYR |= FLASH_CR_LOCK_MASK;
}

void FLASH_vOPTUnlock(void){
    FLASH->OPTKEYR = FLASH_OPT_KEY_1;
    FLASH->OPTKEYR = FLASH_OPT_KEY_2;
}

void FLASH_vOPTLock(void){
    FLASH->OPTCR |= FLASH_OPTCR_LOCK_MASK;
}

void FLASH_vRegisterCallBack(FLASH_CB_t Add_CB){
    FLASH_pCB = Add_CB;
}

void FLASH_vEnEOPInterrupt(void){
    FLASH->CR |= FLASH_EOPIE_MASK;
}

void FLASH_vEnERRInterrupt(void){
    FLASH->CR |= FLASH_ERRIE_MASK;
}

void FLASH_vClearErrorFlags(void){
    FLASH->SR |=  FLASH_SR_CLR_ERR_MASK;   
}


FLASH_enuErrorStatus_t FLASH_enuEraseSector(FLASH_enuSectors_t Copy_enuSector){
    
    FLASH_enuErrorStatus_t Loc_enuStatus = FLASH_enuOK;
    
    if(Copy_enuSector & FLASH_CR_SECTOR_VRF_MASK){
        Loc_enuStatus = FLASH_enuINVALID_SECTOR;
    }
    else if(FLASH->SR & FLASH_SR_BUSY_MASK){
        Loc_enuStatus = FLASH_enuFLASH_BUSY;    
    }
    else{
        FLASH_vUnlock();
        FLASH_vClearErrorFlags();
        FLASH->CR &= FLASH_P_SIZE_CLR_MASK;
        FLASH->CR |= FLASH_P_SIZE_MASK;

        FLASH->CR |= FLASH_CR_SEC_ERASE_EN;

        FLASH->CR &= FLASH_CR_CLR_SEC_MASK;
        FLASH->CR |= Copy_enuSector; 

        FLASH->CR |= FLASH_CR_START_MASK;
    }    
    return Loc_enuStatus;
}

void FLASH_enuMassErase(void){
    
    while(FLASH->SR & FLASH_SR_BUSY_MASK);
    FLASH_vUnlock();
    FLASH_vClearErrorFlags();
    FLASH->CR &= FLASH_P_SIZE_CLR_MASK;
    FLASH->CR |= FLASH_P_SIZE_MASK;

    FLASH->CR |= FLASH_CR_MASS_ERASE_EN; 

    FLASH->CR |= FLASH_CR_START_MASK;
}

FLASH_enuProgramWord(void* Address,uint32_t Copy_u32Word){
    FLASH_vUnlock();
    FLASH->CR &= FLASH_P_SIZE_CLR_MASK;
    FLASH->CR |= FLASH_P_SIZE_MASK;

    FLASH->CR |= FLASH_CR_PROGRAM_EN;

    *((volatile uint32_t*)Address) = Copy_u32Word;
}

void FLASH_enuEnProgramming(void){
    FLASH_vUnlock();
    FLASH->CR |= FLASH_CR_PROGRAM_EN;
    FLASH_vLock();
}

void FLASH_enuDisProgramming(void){
    FLASH_vUnlock();
    FLASH->CR &= ~FLASH_CR_PROGRAM_EN;
    FLASH_vLock();
}


void FLASH_enuEnErasing(void){
    FLASH_vUnlock();
    FLASH->CR |= FLASH_CR_SEC_ERASE_EN;
    FLASH_vLock();
}

void FLASH_enuDisErasing(void){
    FLASH_vUnlock();
    FLASH->CR &= ~FLASH_CR_SEC_ERASE_EN;
    FLASH_vLock();
}

void FLASH_IRQHandler(void){
    if(FLASH_pCB != NULL){
        if(FLASH->SR & FLASH_SR_EOP_MASK){
            FLASH->SR |= FLASH_SR_EOP_MASK;
            FLASH_vLock();
            FLASH_pCB(FLASH_enuEOP);
        }
        if(FLASH->SR & FLASH_SR_OPERR_MASK){
            FLASH->SR |= FLASH_SR_OPERR_MASK;
            FLASH_vLock();
            FLASH_pCB(FLASH_enuOPERR);
        }
        if(FLASH->SR & FLASH_SR_WRPERR_MASK){
            FLASH->SR |= FLASH_SR_WRPERR_MASK;
            FLASH_vLock();
            FLASH_pCB(FLASH_enuWRPERR);
        }
        if(FLASH->SR & FLASH_SR_PGAERR_MASK){
            FLASH->SR |= FLASH_SR_PGAERR_MASK;
            FLASH_vLock();
            FLASH_pCB(FLASH_enuPGAERR);
        }
        if(FLASH->SR & FLASH_SR_PGPERR_MASK){
            FLASH->SR |= FLASH_SR_PGPERR_MASK;
            FLASH_vLock();
            FLASH_pCB(FLASH_enuPGPERR);
        }
        if(FLASH->SR & FLASH_SR_PGSERR_MASK){
            FLASH->SR |= FLASH_SR_PGSERR_MASK;
            FLASH_vLock();
            FLASH_pCB(FLASH_enuPGSERR);
        }
        if(FLASH->SR & FLASH_SR_RDERR_MASK){
            FLASH->SR |= FLASH_SR_RDERR_MASK;
            FLASH_vLock();
            FLASH_pCB(FLASH_enuRDERR);
        }
    }
}