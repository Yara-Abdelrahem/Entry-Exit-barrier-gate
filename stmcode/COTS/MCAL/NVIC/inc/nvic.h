#ifndef NVIC_H_
#define NVIC_H_
#include "std_types.h"

#define NVIC_MAX_IRQN                   239
#define NVIC_MAX_GROUP_PRIO             15
#define NVIC_MAX_SUB_GROUP_PRIO         15

typedef enum{
    NVIC_enu_OK,
    NVIC_enu_INVALID_IRQN,
    NIVC_enu_NULL_POINTER,
    NVIC_enu_INVALID_PRIORITY,
    NVIC_enu_NOK

}NVIC_enuErrorStatus_t;

typedef enum {
    NVIC_enuWWDG,
    NVIC_enuEXTI16_PVD,
    NVIC_enuEXTI21_TAMP_STAMP,
    NVIC_enuEXTI22_RTC_WKUP,
    NVIC_enuFLASH,
    NVIC_enuRCC,
    NVIC_enuEXTI0,
    NVIC_enuEXTI1,
    NVIC_enuEXTI2,
    NVIC_enuEXTI3,
    NVIC_enuEXTI4,
    NVIC_enuDMA1_Stream0,
    NVIC_enuDMA1_Stream1,
    NVIC_enuDMA1_Stream2,
    NVIC_enuDMA1_Stream3,
    NVIC_enuDMA1_Stream4,
    NVIC_enuDMA1_Stream5,
    NVIC_enuDMA1_Stream6,
    NVIC_enuADC ,
    NVIC_enuEXTI9_5=23,
    NVIC_enuTIM1_BRK_TIM9,
    NVIC_enuTIM1_UP_TIM10,
    NVIC_enuTIM1_TRG_COM_TIM11,
    NVIC_enuTIM1_CC,
    NVIC_enuTIM2,
    NVIC_enuTIM3,
    NVIC_enuTIM4,
    NVIC_enuI2C1_EV,
    NVIC_enuI2C1_ER,
    NVIC_enuI2C2_EV,
    NVIC_enuI2C2_ER,
    NVIC_enuSPI1,
    NVIC_enuSPI2,
    NVIC_enuUSART1,
    NVIC_enuUSART2,
    NVIC_enuEXTI15_10=40,
    NVIC_enuEXTI17_RTC_Alarm,
    NVIC_enuEXTI18_OTG_FS_WKUP,
    NVIC_enuDMA1_Stream7=47,
    NVIC_enuSDIO=49,
    NVIC_enuTIM5,
    NVIC_enuSPI3,
    NVIC_enuDMA2_Stream0=56,
    NVIC_enuDMA2_Stream1,
    NVIC_enuDMA2_Stream2,
    NVIC_enuDMA2_Stream3,
    NVIC_enuDMA2_Stream4,
    NVIC_enuOTG_FS=67,
    NVIC_enuDMA2_Stream5,
    NVIC_enuDMA2_Stream6,
    NVIC_enuDMA2_Stream7,
    NVIC_enuUSART6,
    NVIC_enuI2C3_EV ,
    NVIC_enuI2C3_ER,
    NVIC_enuFPU=81,
    NVIC_enuSPI4=84 
} NVIC_enuIRQn_t;

extern NVIC_enuErrorStatus_t NVIC_enuEnableIRQn(NVIC_enuIRQn_t Copy_enuIRQn);

extern NVIC_enuErrorStatus_t NVIC_enuDisableIRQn(NVIC_enuIRQn_t Copy_enuIRQn);

extern NVIC_enuErrorStatus_t NVIC_enuSetPendingIRQn(NVIC_enuIRQn_t Copy_enuIRQn);

extern NVIC_enuErrorStatus_t NVIC_enuClearPendingIRQn(NVIC_enuIRQn_t Copy_enuIRQn);

extern NVIC_enuErrorStatus_t NVIC_enuGetPendingIRQn(NVIC_enuIRQn_t Copy_enuIRQn,
uint32_t* Add_u32Ret);

extern NVIC_enuErrorStatus_t NVIC_enuSetPriorityIRQn(NVIC_enuIRQn_t Copy_enuIRQn,
uint8_t Copy_u8Priority);

extern NVIC_enuErrorStatus_t NVIC_enuSetGroupPriority(NVIC_enuIRQn_t Copy_enuIRQn,
uint8_t Copy_u8Priority);

extern NVIC_enuErrorStatus_t NVIC_enuGetGroupPriorityIRQn(NVIC_enuIRQn_t Copy_enuIRQn,
uint32_t* Add_u32Ret);

extern NVIC_enuErrorStatus_t NVIC_enuGetGroupPriorityIRQn(NVIC_enuIRQn_t Copy_enuIRQn,
uint32_t* Add_u32Ret);

extern NVIC_enuErrorStatus_t NVIC_enuSetSubGroupPriority(NVIC_enuIRQn_t Copy_enuIRQn,
uint8_t Copy_u8Priority);

extern NVIC_enuErrorStatus_t NVIC_enuTriggerInterrupt(NVIC_enuIRQn_t Copy_enuIRQn);
#endif