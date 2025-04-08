#include "uart_dma.h"

UartDMA_stCfg_t UartDMA_arrCfgs[UART_DMA_NO_OF_LINES] = {
    [UART_LINE_1] = {
        .UART = Usart_enu_USART2,
        .Port = GPIO_enu_GPIOA,
        .Pin = GPIO_enu_PIN2,
        .baudRate = 9600,
        .StopBit = Usart_enu_1StopBit,
        .ParityMode = Usart_enu_noParity,
        .Mode = Usart_enu_Tx
    },
    [UART_LINE_2] = {
        .UART = Usart_enu_USART2,
        .Port = GPIO_enu_GPIOA,
        .Pin = GPIO_enu_PIN3,
        .baudRate = 9600,
        .StopBit = Usart_enu_1StopBit,
        .ParityMode = Usart_enu_noParity,
        .Mode = Usart_enu_Rx
    } 
};