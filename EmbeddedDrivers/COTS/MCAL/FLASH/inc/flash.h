#ifndef FLASH_H_
#define FLASH_H_
#include "std_types.h"

typedef enum{
    FLASH_enuOK,
    FLASH_enuINVALID_SECTOR,
    FLASH_enuFLASH_BUSY,
    FLASH_enuNOK
}FLASH_enuErrorStatus_t;

typedef enum{
    FLASH_enuSector0,
    FLASH_enuSector1 = 0x00000008,
    FLASH_enuSector2,
    FLASH_enuSector3,
    FLASH_enuSector4,
    FLASH_enuSector5,
    FLASH_enuSector6,
    FLASH_enuSector7
}FLASH_enuSectors_t;

typedef enum{
    FLASH_enuEOP,
    FLASH_enuOPERR,
    FLASH_enuWRPERR,
    FLASH_enuPGAERR,
    FLASH_enuPGPERR,
    FLASH_enuPGSERR,
    FLASH_enuRDERR
}FLASH_enuEvents_t;

typedef void (*FLASH_CB_t)(FLASH_enuEvents_t);

extern void FLASH_vUnlock(void);

extern void FLASH_vLock(void);

extern void FLASH_vOPTUnlock(void);

extern void FLASH_vOPTLock(void);

#endif