#ifndef SYSCFG_H_
#define SYSCFG_H_
#include "std_types.h"

typedef enum{
    SYSCFG_enu_OK,
    SYSCFG_enu_INVALID_MEM,
    SYSCFG_enu_INVALID_LINE,
    SYSCFG_enu_INVALID_PORT,
    SYSCFG_enu_NULL_POINTER,
    SYSCFG_enu_NOK
}SYSCFG_enuErrorStatus_t;

typedef enum{
    SYSCFG_enu_FLASH_RMP,
    SYSCFG_enu_SYS_MEM_RMP,
    SYSCFG_enu_SRAM_RMP
}SYSCFG_enuMemoryRMP_t;

typedef enum{
    SYSCFG_enu_EXTILine0,    
    SYSCFG_enu_EXTILine1,
    SYSCFG_enu_EXTILine2,
    SYSCFG_enu_EXTILine3,
    SYSCFG_enu_EXTILine4,
    SYSCFG_enu_EXTILine5,
    SYSCFG_enu_EXTILine6,
    SYSCFG_enu_EXTILine7,
    SYSCFG_enu_EXTILine8,
    SYSCFG_enu_EXTILine9,
    SYSCFG_enu_EXTILine10,
    SYSCFG_enu_EXTILine11,
    SYSCFG_enu_EXTILine12,
    SYSCFG_enu_EXTILine13,    
    SYSCFG_enu_EXTILine14,    
    SYSCFG_enu_EXTILine15    
}SYSCFG_enuEXTILine_t;

typedef enum{
    SYSCFG_enu_PORTA,
    SYSCFG_enu_PORTB,
    SYSCFG_enu_PORTC,
    SYSCFG_enu_PORTD,
    SYSCFG_enu_PORTE,
    SYSCFG_enu_PORTH
}SYSCFG_enuPORT_t;

extern SYSCFG_enuErrorStatus_t SYSCFG_enuRemapRegion(SYSCFG_enuMemoryRMP_t Copy_enuRegion);

extern SYSCFG_enuErrorStatus_t SYSCFG_enuConnectEXTILine(SYSCFG_enuEXTILine_t Loc_enuLine,
SYSCFG_enuPORT_t Loc_enuPort);

extern SYSCFG_enuErrorStatus_t SYSCFG_enuGetConnectedPort(SYSCFG_enuEXTILine_t Loc_enuLine,
uint32_t* Add_enuRet);


#endif