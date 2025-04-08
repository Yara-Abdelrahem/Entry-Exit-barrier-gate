#ifndef UART_INT_H_
#define UART_INT_H_
#include "uart_int_cfg.h"

#define UART_INT_MAX_BUFF_SIZE         100

typedef void (*UsartInt_CB) (UartInt_enuEvents_t);

typedef enum{
    UartInt_enu_OK,
    UartInt_enu_INVALID_UART,
    UartInt_enu_INVALID_BUFF_SIZE,
    UartInt_enu_NULL_POINTER,
    UartInt_enu_NOK
}UartInt_enuErrorStatus_t;



extern void UsartInt_vInit(void);

extern UartInt_enuErrorStatus_t UartInt_enuSendBuffer(uint8_t Copy_u8Uart,
const uint8_t* Add_u8Buff,uint8_t Copy_u8BuffSize);

extern UartInt_enuErrorStatus_t UartInt_enuSendString(uint8_t Copy_u8Uart,
const int8_t* Add_u8Str);

extern UartInt_enuErrorStatus_t UartInt_enuReceiveBuffer(uint8_t Copy_u8Uart,
uint8_t* Add_u8Buff,uint8_t Copy_u8BuffSize);


#endif