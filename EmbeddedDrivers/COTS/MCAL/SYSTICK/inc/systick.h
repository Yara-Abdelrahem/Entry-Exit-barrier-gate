#ifndef SYSTICK_H_
#define SYSTICK_H_
#include "std_types.h"

#ifndef SYSTICK_NO_OF_CYCLIC_FN
#define SYSTICK_NO_OF_CYCLIC_FN     10    
#endif

#define SYSTICK_MAX_FN_POS          9

typedef void(*SYSTICK_CallBack_t)(void);

typedef enum{
    SYSTICK_enu_OK,
    SYSTICK_enu_INVALID_PRESCALER,
    SYSTICK_enu_INVALID_TICKS,
    SYSTICK_enu_INVALID_POSITION,
    SYSTICK_enu_NULL_POINTER,
    SYSTICK_enu_NOK
}SYSTICK_enuErrorStatus_t;

typedef enum{
    SYSTICK_enu_PRESCALER_1=1,
    SYSTICK_enu_PRESCALER_8=8
}SYSTICK_enuInputPrescaler_t;



extern void Systick_vStart(void);

extern void Systick_vStop(void);

extern SYSTICK_enuErrorStatus_t Systick_enuInit1Ms(SYSTICK_enuInputPrescaler_t Copy_enuPrescaler);

extern void Systick_vDelayMs(uint32_t Copy_u32Delay);

extern SYSTICK_enuErrorStatus_t Systick_enuSetCyclicFnMs(SYSTICK_CallBack_t Add_pCB,
uint32_t delayCycle);



#endif