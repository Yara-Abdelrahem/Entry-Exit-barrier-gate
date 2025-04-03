#include "uart_int.h"
#include "nvic.h"
#include "rcc.h"

#define UART_INT_NO_OF_USARTS           3

void UartInt_vOnTC_CallBack(void);
void UartInt_vOnRXNE_CallBack(void);

static UsartInt_CB UartInt_arrCBS[UART_INT_NO_OF_UARTS] = {NULL};

static volatile uint8_t UartInt_arrTxBuffer[UART_INT_MAX_BUFF_SIZE] = {0};
static uint8_t* UartInt_pRxBuffer = NULL;

static volatile uint8_t UartInt_u8TxBufferSize = 0;
static volatile uint8_t UartInt_u8RxBufferSize = 0;

static volatile uint8_t UartInt_u8TxIndex = 0;
static volatile uint8_t UartInt_u8RxIndex = 0;

static volatile Usart_enuUsarts_t UartInt_enuUart = Usart_enu_USART2;

static const uint64_t UartInt_arrUsartClkMap[UART_INT_NO_OF_USARTS] = {
    RCC_USART1,
    RCC_USART2,
    RCC_USART6
};

static const NVIC_enuIRQn_t UartInt_arrUsartIrqMap[UART_INT_NO_OF_USARTS] = {
    NVIC_enuUSART1,
    NVIC_enuUSART2,
    NVIC_enuUSART6
};

void UsartInt_vInit(void){
    
    Usart_stCfg_t Loc_stUsartCfg;
    uint8_t i = 0;
    uint64_t Loc_u64UsartClkMask = 0;
    uint64_t Loc_u64GpioClkMask = 0;
    NVIC_enuIRQn_t Loc_enuIrqn = 0;
    GPIO_stPinCFG_t Loc_stGpioTxCfg = {0};
    GPIO_stPinCFG_t Loc_stGpioRxCfg = {0};

    for(i=0;i<UART_INT_NO_OF_UARTS;i++){        

        Loc_u64UsartClkMask = UartInt_arrUsartClkMap[UartInt_arrCfgs[i].UART];
        
        if(UartInt_arrCfgs[i].TxPin.Port == GPIO_enu_GPIOH){
            Loc_u64GpioClkMask |= RCC_GPIOH;
        }
        else{
            Loc_u64GpioClkMask |= (1 << UartInt_arrCfgs[i].TxPin.Port);
        }

        if(UartInt_arrCfgs[i].RxPin.Port == GPIO_enu_GPIOH){
            Loc_u64GpioClkMask |= RCC_GPIOH;
        }
        else{
            Loc_u64GpioClkMask |= (1 << UartInt_arrCfgs[i].RxPin.Port);
        }
        RCC_enuCtlPeripheral(Loc_u64UsartClkMask,RCC_PER_ON);
    }

    RCC_enuCtlPeripheral(Loc_u64GpioClkMask,RCC_PER_ON);

    for(i=0;i<UART_INT_NO_OF_UARTS;i++){

        Loc_u64UsartClkMask = UartInt_arrUsartClkMap[UartInt_arrCfgs[i].UART];
        Loc_enuIrqn = UartInt_arrUsartIrqMap[UartInt_arrCfgs[i].UART];
        
        Loc_stGpioRxCfg.port = UartInt_arrCfgs[i].RxPin.Port;
        Loc_stGpioRxCfg.pin  = UartInt_arrCfgs[i].RxPin.Pin;
        Loc_stGpioRxCfg.mode = GPIO_enu_ALT_FN;
        Loc_stGpioRxCfg.speed = GPIO_enu_high_speed;
        Loc_stGpioRxCfg.OutputType = GPIO_enu_PUSH_PULL;
        Loc_stGpioRxCfg.InputType = GPIO_enu_NO_PU_PD;
        
        if(UartInt_arrCfgs[i].UART<Usart_enu_USART6){
            Loc_stGpioRxCfg.AltFn = GPIO_enu_AltFn7;
        }
        else{
            Loc_stGpioRxCfg.AltFn = GPIO_enu_AltFn8;
        }

        Loc_stGpioTxCfg = Loc_stGpioRxCfg;
        Loc_stGpioTxCfg.port = UartInt_arrCfgs[i].TxPin.Port;
        Loc_stGpioTxCfg.pin = UartInt_arrCfgs[i].TxPin.Pin;

        Loc_stUsartCfg.Mode = UartInt_arrCfgs[i].Mode;
        Loc_stUsartCfg.BaudRate = UartInt_arrCfgs[i].baudRate;
        Loc_stUsartCfg.ParityMode = UartInt_arrCfgs[i].ParityMode;
        Loc_stUsartCfg.StopBit = UartInt_arrCfgs[i].StopBit;
        Loc_stUsartCfg.WordLength = UartInt_arrCfgs[i].wordLength;
        Loc_stUsartCfg.Usart = UartInt_arrCfgs[i].UART;
        Loc_stUsartCfg.clkType = Usart_enu_sync;

        GPIO_enuCfgPin(&Loc_stGpioRxCfg);
        GPIO_enuCfgPin(&Loc_stGpioTxCfg);

        Usart_enuInit(&Loc_stUsartCfg);

        USART_enuSetTCVal(UartInt_arrCfgs[i].UART,LOW);
        USART_enuSetRXNEval(UartInt_arrCfgs[i].UART,LOW);

        NVIC_enuClearPendingIRQn(Loc_enuIrqn);
        NVIC_enuEnableIRQn(Loc_enuIrqn);

        switch(UartInt_arrCfgs[i].Mode){
            case Usart_enu_Tx:
            Usart_enuRegisterCallBack(
                UartInt_arrCfgs[i].UART,
                UartInt_vOnTC_CallBack,
                Usart_enu_onTC    
            );
            USART_enuEnableInt(UartInt_arrCfgs[i].UART,USART_enuTXC_INT);
            break;

            case Usart_enu_Rx:
            Usart_enuRegisterCallBack(
                UartInt_arrCfgs[i].UART,
                UartInt_vOnRXNE_CallBack,
                Usart_enu_onRXN    
            );
            USART_enuEnableInt(UartInt_arrCfgs[i].UART,USART_enuRXNE_INT);
            break;

            case Usart_enu_Tx_Rx:
            Usart_enuRegisterCallBack(
                UartInt_arrCfgs[i].UART,
                UartInt_vOnRXNE_CallBack,
                Usart_enu_onRXN    
            );
            Usart_enuRegisterCallBack(
                UartInt_arrCfgs[i].UART,
                UartInt_vOnTC_CallBack,
                Usart_enu_onTC    
            );
            USART_enuEnableInt(UartInt_arrCfgs[i].UART,USART_enuRXNE_INT);
            USART_enuEnableInt(UartInt_arrCfgs[i].UART,USART_enuTXC_INT);
            break;
            
            default:
            break;
        }
    }
} 


UartInt_enuErrorStatus_t UartInt_enuSendBuffer(uint8_t Copy_u8Uart,
const uint8_t* Add_u8Buff,uint8_t Copy_u8BuffSize){
    
    UartInt_enuErrorStatus_t Loc_enuStatus = UartInt_enu_OK;
    uint8_t i=0;

    if(Add_u8Buff == NULL){
        Loc_enuStatus = UartInt_enu_NULL_POINTER;
    }
    else if(Copy_u8Uart > MAX_UART_INDEX){
        Loc_enuStatus = UartInt_enu_INVALID_UART;
    }
    else if(Copy_u8BuffSize > UART_INT_MAX_BUFF_SIZE){
        Loc_enuStatus = UartInt_enu_INVALID_BUFF_SIZE;
    }
    else{
        UartInt_u8TxBufferSize = Copy_u8BuffSize;
        UartInt_enuUart = UartInt_arrCfgs[Copy_u8Uart].UART;
        for(;i<Copy_u8BuffSize;i++){
            UartInt_arrTxBuffer[i] = Add_u8Buff[i];
        }
        Usart_enuSendByte(UartInt_enuUart,UartInt_arrTxBuffer[UartInt_u8TxIndex++]);
    }

    return Loc_enuStatus;
}


UartInt_enuErrorStatus_t UartInt_enuReceiveBuffer(uint8_t Copy_u8Uart,
uint8_t* Add_u8Buff,uint8_t Copy_u8BuffSize){

    UartInt_enuErrorStatus_t Loc_enuStatus = UartInt_enu_OK;

    if(Add_u8Buff == NULL){
        Loc_enuStatus = UartInt_enu_NULL_POINTER;
    }
    else if(Copy_u8Uart > MAX_UART_INDEX){
        Loc_enuStatus = UartInt_enu_INVALID_UART;
    }
    else{
        UartInt_pRxBuffer = Add_u8Buff; 
        UartInt_u8RxBufferSize = Copy_u8BuffSize; 
        UartInt_u8RxIndex = 0; 
    }
    return Loc_enuStatus;
}


UartInt_enuErrorStatus_t UartInt_enuSendString(uint8_t Copy_u8Uart,
const int8_t* Add_u8Str){
    
    UartInt_enuErrorStatus_t Loc_enuStatus = UartInt_enu_OK;
    volatile uint8_t i = 0;

    for(;(Add_u8Str[i]) && (i < UART_INT_MAX_BUFF_SIZE);i++){}

    Loc_enuStatus = UartInt_enuSendBuffer(Copy_u8Uart,(uint8_t*)Add_u8Str,i);

    return Loc_enuStatus;
}



void UartInt_vOnTC_CallBack(void){
    if(UartInt_u8TxIndex < UartInt_u8TxBufferSize){
        Usart_enuSendByte(UartInt_enuUart,UartInt_arrTxBuffer[UartInt_u8TxIndex++]);
    }
    else{
        UartInt_u8TxIndex = 0;
        if(UartInt_arrCBS[UART_1]!= NULL){
            UartInt_arrCBS[UART_1](UartInt_enuOnTransmit);
        }
    }
}

void UartInt_vOnRXNE_CallBack(void){
    if(UartInt_pRxBuffer != NULL) {
        if(UartInt_u8RxIndex < UartInt_u8RxBufferSize) {
            Usart_enuGetByte(UartInt_enuUart, &UartInt_pRxBuffer[UartInt_u8RxIndex++]);
        }
        else {
            UartInt_pRxBuffer = NULL;
            UartInt_u8RxIndex = 0;
            if(UartInt_arrCBS[UART_1]!= NULL){
                UartInt_arrCBS[UART_1](UartInt_enuOnReceive);
            }
        }
    }
}

UartInt_enuErrorStatus_t UartInt_enuRegisterCallBack(uint8_t Copy_u8Uart,
UsartInt_CB Add_CB){

    UartInt_enuErrorStatus_t Loc_enuStatus = UartInt_enu_OK;
    if(Copy_u8Uart >= UART_INT_NO_OF_UARTS){
        Loc_enuStatus = UartInt_enu_INVALID_UART;
    }
    else{
        UartInt_arrCBS[Copy_u8Uart] = Add_CB;
    }
    return Loc_enuStatus;
}

