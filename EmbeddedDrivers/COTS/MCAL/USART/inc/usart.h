#ifndef USART_H_
#define USART_H_
#include "std_types.h"

typedef enum{
    Usart_enu_OK,
    Usart_enu_INVALID_USART,
    Usart_enu_TX_BUSY,
    Usart_enu_RX_Not_RDY,
    Usart_enu_INVALID_CLK_TYPE,
    Usart_enu_INVALID_WORD_LENGTH,
    Usart_enu_INVALID_PARITY_MODE,
    Usart_enu_INVALID_MODE,
    Usart_enu_INVALID_STOP_BIT,
    Usart_enu_INVALID_EVENT,
    Usart_enu_INVALID_INT,
    Usart_enu_INVALID_STATE,
    Usart_enu_NULL_POINTER,
    Usart_enu_NOK
}Usart_enuErrorStatus_t;

typedef enum{   
    Usart_enu_USART1,
    Usart_enu_USART2,
    Usart_enu_USART6    
}Usart_enuUsarts_t;

typedef enum{
    Usart_enu_async,
    Usart_enu_sync = 0x00000800
}Usart_enuClkType_t;

typedef enum{
    Usart_enu_word8,
    Usart_enu_word9 = 0x00001000
}Usart_enuWordLength_t;

typedef enum{
    Usart_enu_1StopBit,
    Usart_enu_05Stopbit,
    Usart_enu_2StopBit,
    Usart_enu_15StopBit
}Usart_enuStopBit_t;

typedef enum{
    Usart_enu_Tx = 0x00000008,
    Usart_enu_Rx = 0x00000004,
    Usart_enu_Tx_Rx = 0x0000000C
}Usart_enuMode_t;

typedef enum{
    Usart_enu_noParity,
    Usart_enu_evenParity=0x00000400,
    Usart_enu_oddParity=0x00000600
}Usart_enuParityMode_t;

typedef enum{
    Usart_enu_onTC,
    Usart_enu_onRXN
}Usart_enuEvent_t;

typedef void (*Usart_pCallBack)(void);

typedef struct{
    Usart_enuUsarts_t Usart;
    Usart_enuClkType_t clkType;
    Usart_enuWordLength_t WordLength;
    Usart_enuStopBit_t StopBit;
    Usart_enuMode_t Mode;
    Usart_enuParityMode_t ParityMode;
    uint32_t BaudRate;
}Usart_stCfg_t;

typedef enum{
    USART_enuTXE_INT,
    USART_enuTXC_INT,
    USART_enuRXNE_INT
}USART_enuInterrupts_t;


extern Usart_enuErrorStatus_t Usart_enuInit(Usart_stCfg_t* Add_stCfg);

extern Usart_enuErrorStatus_t Usart_enuRegisterCallBack(Usart_enuUsarts_t Copy_enuUsart,
Usart_pCallBack Add_pCB,Usart_enuEvent_t Copy_enuEvent);

extern Usart_enuErrorStatus_t Usart_enuSendByte(Usart_enuUsarts_t Copy_enuUsart,
uint8_t Copy_u8Byte);

extern Usart_enuErrorStatus_t  Usart_sendByteBlocking(Usart_enuUsarts_t Copy_enuUsart,
uint8_t Copy_u8Byte);

extern Usart_enuErrorStatus_t Usart_enuGetByte(Usart_enuUsarts_t Copy_enuUsart,
uint8_t* Add_u8Byte);

extern Usart_enuErrorStatus_t USART_enuEnableInt(Usart_enuUsarts_t Copy_enuUart,
USART_enuInterrupts_t Copy_enuInt);

extern Usart_enuErrorStatus_t USART_enuDisableInt(Usart_enuUsarts_t Copy_enuUart,
USART_enuInterrupts_t Copy_enuInt);

extern Usart_enuErrorStatus_t USART_enuEnableDMA_TX(Usart_enuUsarts_t Copy_enuUsart);

extern Usart_enuErrorStatus_t USART_enuDisableDMA_TX(Usart_enuUsarts_t Copy_enuUsart);

extern Usart_enuErrorStatus_t USART_enuEnableDMA_RX(Usart_enuUsarts_t Copy_enuUsart);

extern Usart_enuErrorStatus_t USART_enuDisableDMA_RX(Usart_enuUsarts_t Copy_enuUsart);

extern Usart_enuErrorStatus_t USART_enuGetTXCVal(Usart_enuUsarts_t Copy_enuUsart,
uint8_t* Add_u8State);

extern Usart_enuErrorStatus_t USART_enuGetTXEVal(Usart_enuUsarts_t Copy_enuUsart,
uint8_t* Add_u8State);

extern Usart_enuErrorStatus_t USART_enuGetRXNEval(Usart_enuUsarts_t Copy_enuUsart,
uint8_t* Add_u8State);

extern Usart_enuErrorStatus_t USART_enuSetTCVal(Usart_enuUsarts_t Copy_enuUsart,
uint8_t Add_u8State);

extern Usart_enuErrorStatus_t USART_enuSetRXNEval(Usart_enuUsarts_t Copy_enuUsart,
uint8_t Add_u8State);

#endif