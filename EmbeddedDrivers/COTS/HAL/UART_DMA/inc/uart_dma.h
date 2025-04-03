#ifndef UART_DMA_H_
#define UART_DMA_H_
#include "uart_dma_cfg.h"

typedef enum{
    UartDMA_enu_OK,
    UartDMA_enu_INVALID_LINE,
    UartDMA_enu_NULL_POINTER,
    UartDMA_enu_NOK
}UartDMA_enuErrorStatus_t;

extern void UartDMA_vInit(void);

extern UartDMA_enuErrorStatus_t UartDMA_enuRegisterCallBack(uint8_t Copy_u8Line,
DMA_CB Add_cb);

extern UartDMA_enuErrorStatus_t UartDMA_enuStartTransfer(uint8_t Copy_u8Line);

extern UartDMA_enuErrorStatus_t UartDMA_enuStopTransfer(uint8_t Copy_u8Line);

extern UartDMA_enuErrorStatus_t UartDma_enuResumeTransfer(uint8_t Copy_u8Line);

extern UartDMA_enuErrorStatus_t UartDMA_enuRegisterBuff(uint8_t Copy_u8Line,
void* Add_vBuff,uint16_t Copy_u16Size);


#endif