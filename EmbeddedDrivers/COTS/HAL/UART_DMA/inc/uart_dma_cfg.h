#ifndef UART_DMA_CFG_H_
#define UART_DMA_CFG_H_

#include "GPIO.h"
#include "rcc.h"
#include "nvic.h"
#include "usart.h"
#include "dma.h"

/*void (*DMA_CB)(DMA_enuEvents_t);*/

#define UART_DMA_NO_OF_LINES    2
#define UART_LINE_1             0
#define UART_LINE_2             1



typedef struct{
    Usart_enuUsarts_t UART;
    GPIO_enuPins_t Pin;
    GPIO_enuPorts_t Port;
    uint32_t baudRate;
    Usart_enuStopBit_t StopBit;
    Usart_enuParityMode_t ParityMode;
    Usart_enuMode_t Mode;
}UartDMA_stCfg_t;

extern UartDMA_stCfg_t UartDMA_arrCfgs[UART_DMA_NO_OF_LINES];

#endif