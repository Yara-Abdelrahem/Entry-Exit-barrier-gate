#ifndef EXTI_H_
#define EXTI_H_
#include "std_types.h"

#define EXTI_LINE0_MASK             0x00000001UL
#define EXTI_LINE1_MASK             0x00000002UL
#define EXTI_LINE2_MASK             0x00000004UL
#define EXTI_LINE3_MASK             0x00000008UL
#define EXTI_LINE4_MASK             0x00000010UL
#define EXTI_LINE5_MASK             0x00000020UL
#define EXTI_LINE6_MASK             0x00000040UL
#define EXTI_LINE7_MASK             0x00000080UL
#define EXTI_LINE8_MASK             0x00000100UL
#define EXTI_LINE9_MASK             0x00000200UL
#define EXTI_LINE10_MASK            0x00000400UL
#define EXTI_LINE11_MASK            0x00000800UL
#define EXTI_LINE12_MASK            0x00001000UL
#define EXTI_LINE13_MASK            0x00002000UL
#define EXTI_LINE14_MASK            0x00004000UL
#define EXTI_LINE15_MASK            0x00008000UL
#define EXTI_LINE16_MASK            0x00010000UL
#define EXTI_LINE17_MASK            0x00020000UL
#define EXTI_LINE18_MASK            0x00040000UL
#define EXTI_LINE21_MASK            0x00200000UL
#define EXTI_LINE22_MASK            0x00400000UL

#define EXTI_LINE_STATE_EN          0xFFFFFFFFUL
#define EXTI_LINE_STATE_DIS         0x00000000UL

#define EXTI_LINE_INT               ((uint8_t)0x00)
#define EXTI_LINE_EVENT             ((uint8_t)0x01)

#define EXTI_MODE_NONE              ((uint8_t)0x00)
#define EXTI_MODE_RISING_EDGE       ((uint8_t)0x01)        
#define EXTI_MODE_FALLING_EDGE      ((uint8_t)0x02)
#define EXTI_MODE_RF_EDGES          ((uint8_t)0x03)

typedef void(*EXTI_CallBack_t)(void);

typedef enum{
    EXTI_enu_OK,
    EXTI_enu_INVALID_LINE,
    EXTI_enu_INVALID_STATE,
    EXTI_enu_INVALID_IN,
    EXTI_enu_INVALID_MODE,
    EXTI_enu_NULL_POINTER,
    EXTI_enu_NOK
}EXTI_enuErrorStatus_t;

typedef enum{
    EXTI_enu_cbLine0,    
    EXTI_enu_cbLine1,
    EXTI_enu_cbLine2,
    EXTI_enu_cbLine3,
    EXTI_enu_cbLine4,
    EXTI_enu_cbLine5,
    EXTI_enu_cbLine6,
    EXTI_enu_cbLine7,
    EXTI_enu_cbLine8,
    EXTI_enu_cbLine9,
    EXTI_enu_cbLine10,
    EXTI_enu_cbLine11,
    EXTI_enu_cbLine12,
    EXTI_enu_cbLine13,
    EXTI_enu_cbLine14,
    EXTI_enu_cbLine15,
    EXTI_enu_cbLine16,    
    EXTI_enu_cbLine17,
    EXTI_enu_cbLine18,
    EXTI_enu_cbLine21=21,
    EXTI_enu_cbLine22
}EXTI_enuCallBackLine_t;

extern EXTI_enuErrorStatus_t EXTI_enuCtlLine(uint32_t Copy_u32Line,uint32_t Copy_u32State,
uint8_t Copy_u8INT_Event);

extern EXTI_enuErrorStatus_t EXTI_enuSetTrigMode(uint32_t Copy_u32Line,
uint8_t Copy_u8Mode);

extern EXTI_enuErrorStatus_t EXTI_enuSetPending(uint32_t Copy_u32Line);

extern EXTI_enuErrorStatus_t EXTI_enuClearPending(uint32_t Copy_u32Line);

extern EXTI_enuErrorStatus_t EXTI_enuGetPending(uint32_t Copy_u32Line,
uint8_t* Add_u8Ret);

extern EXTI_enuErrorStatus_t EXTI_enuSetCallBack(EXTI_enuCallBackLine_t Copy_enuLine ,
EXTI_CallBack_t add_CallBack);

#endif