#ifndef UART_INT_CFG_H_
#define UART_INT_CFG_H_
#include "usart.h"
#include "GPIO.h"

#define UART_INT_NO_OF_UARTS        1
#define UART_1                      0
#define MAX_UART_INDEX              0

typedef enum{
    UartInt_enuOnTransmit,
    UartInt_enuOnReceive
}UartInt_enuEvents_t;

typedef struct{
    GPIO_enuPorts_t Port;
    GPIO_enuPins_t Pin;
}UartInt_stPinNo;


typedef struct{
    UartInt_stPinNo TxPin;
    UartInt_stPinNo RxPin;
    Usart_enuUsarts_t UART;
    uint32_t baudRate;
    Usart_enuWordLength_t wordLength;
    Usart_enuStopBit_t StopBit;
    Usart_enuParityMode_t ParityMode;
    Usart_enuMode_t Mode;
}UartInt_Cfg_t;

extern UartInt_Cfg_t UartInt_arrCfgs[UART_INT_NO_OF_UARTS];

#endif