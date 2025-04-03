#include "uart_dma.h"

#define UART_DMA_NO_OF_UARTS            3
#define UART_DMA_NO_OF_DMAS             2

#define UART_DMA_NO_OF_STATES           2
#define UART_DMA_ON_TX                  0
#define UART_DMA_ON_RX                  1

typedef struct{
    DMA_enuDMA_t DMA;
    DMA_enuStream_t stream[UART_DMA_NO_OF_STATES];
    DMA_enuChannelSel_t channel;
    NVIC_enuIRQn_t IRQn[UART_DMA_NO_OF_STATES];
}UartDMA_stDMAdata_t;

typedef struct{
    void* address;
    uint16_t size;
}UartDMA_stBuffdata_t;

static const UartDMA_stDMAdata_t UartDMA_arrDMAdata[UART_DMA_NO_OF_UARTS] = {
    [Usart_enu_USART1] = {
        .DMA = DMA_enuDMA2,
        .stream[UART_DMA_ON_TX] = DMA_enuStream7,
        .stream[UART_DMA_ON_RX] = DMA_enuStream5,
        .channel = DMA_enuCH4,
        .IRQn[UART_DMA_ON_TX] = NVIC_enuDMA2_Stream7,
        .IRQn[UART_DMA_ON_RX] = NVIC_enuDMA2_Stream5
    },
    [Usart_enu_USART2] = {
        .DMA = DMA_enuDMA1,
        .stream[UART_DMA_ON_TX] = DMA_enuStream6,
        .stream[UART_DMA_ON_RX] = DMA_enuStream5,
        .channel = DMA_enuCH4,
        .IRQn[UART_DMA_ON_TX] = NVIC_enuDMA1_Stream6,
        .IRQn[UART_DMA_ON_RX] = NVIC_enuDMA1_Stream5
    },
    [Usart_enu_USART6] = {
        .DMA = DMA_enuDMA2,
        .stream[UART_DMA_ON_TX] = DMA_enuStream7,
        .stream[UART_DMA_ON_RX] = DMA_enuStream2,
        .channel = DMA_enuCH4,
        .IRQn[UART_DMA_ON_TX] = NVIC_enuDMA2_Stream7,
        .IRQn[UART_DMA_ON_RX] = NVIC_enuDMA2_Stream2
    }
};

static const uint64_t UartDMA_arrUsartCLkMasks[UART_DMA_NO_OF_UARTS] = {
    RCC_USART1,
    RCC_USART2,
    RCC_USART6
};

static void* const UartDMA_arrUSART_DR_Add[UART_DMA_NO_OF_UARTS] = {
    (void*)0x40011004UL,
    (void*)0x40004404UL,
    (void*)0x40011404UL 
};

static const uint64_t UartDMA_arrDMAClkMasks[UART_DMA_NO_OF_DMAS] = {
    RCC_DMA1,
    RCC_DMA2
};


static const uint8_t UartDMA_arrDefault[2] = {0}; 

static UartDMA_stBuffdata_t UartDMA_arrBuffData[UART_DMA_NO_OF_LINES] = {{0}};

void UartDMA_vInit(void){

    uint8_t i=0;
    uint64_t Loc_u64GpioClkMask = 0;
    DMA_enuDMA_t Loc_enuDma = 0;
    GPIO_stPinCFG_t Loc_stGpioCfg = {0};
    Usart_stCfg_t Loc_stUsartCfg = {0};
    DMA_stCfg_t Loc_stDmaCfg = {0};
    Usart_enuUsarts_t Loc_enuUart = 0;
    uint8_t Loc_arrUart[UART_DMA_NO_OF_UARTS] = {0};


    for(;i<UART_DMA_NO_OF_LINES;i++){
        if(UartDMA_arrCfgs[i].Port < GPIO_enu_GPIOH){
            Loc_u64GpioClkMask |= (1<<UartDMA_arrCfgs[i].Port);
        }     
        else{
            Loc_u64GpioClkMask |= RCC_GPIOH;
        }
        RCC_enuCtlPeripheral(
            UartDMA_arrUsartCLkMasks[UartDMA_arrCfgs[i].UART],
            RCC_PER_ON
        );
        Loc_enuDma = UartDMA_arrDMAdata[UartDMA_arrCfgs[i].UART].DMA;

        RCC_enuCtlPeripheral(
            UartDMA_arrDMAClkMasks[Loc_enuDma],
            RCC_PER_ON    
        );
    }
    RCC_enuCtlPeripheral(Loc_u64GpioClkMask,RCC_PER_ON);

    for(i=0;i<UART_DMA_NO_OF_DMAS;i++){
        Loc_stGpioCfg.port = UartDMA_arrCfgs[i].Port;
        Loc_stGpioCfg.pin = UartDMA_arrCfgs[i].Pin;
        if(UartDMA_arrCfgs[i].UART < Usart_enu_USART6){
            Loc_stGpioCfg.AltFn = GPIO_enu_AltFn7;
        }
        else{
            Loc_stGpioCfg.AltFn = GPIO_enu_AltFn8;
        }
        Loc_stGpioCfg.mode = GPIO_enu_ALT_FN;
        Loc_stGpioCfg.OutputType = GPIO_enu_PUSH_PULL;
        Loc_stGpioCfg.InputType = GPIO_enu_NO_PU_PD;
        Loc_stGpioCfg.speed = GPIO_enu_high_speed;
        GPIO_enuCfgPin(&Loc_stGpioCfg);

        Loc_stUsartCfg.BaudRate = UartDMA_arrCfgs[i].baudRate;
        Loc_stUsartCfg.clkType = Usart_enu_async;
        Loc_stUsartCfg.ParityMode = UartDMA_arrCfgs[i].ParityMode;
        Loc_stUsartCfg.StopBit = UartDMA_arrCfgs[i].StopBit;
        Loc_stUsartCfg.Usart = UartDMA_arrCfgs[i].UART;
        Loc_stUsartCfg.WordLength = Usart_enu_word8;
        if(Loc_arrUart[UartDMA_arrCfgs[i].UART]){
            Loc_stUsartCfg.Mode = Usart_enu_Tx_Rx;
        }
        else{
            Loc_stUsartCfg.Mode = UartDMA_arrCfgs[i].Mode;
            Loc_arrUart[UartDMA_arrCfgs[i].UART]++;
        }
        Usart_enuInit(&Loc_stUsartCfg);

        Loc_enuUart = UartDMA_arrCfgs[i].UART;
        Loc_enuDma = UartDMA_arrDMAdata[Loc_enuUart].DMA;
        Loc_stDmaCfg.DMA = Loc_enuDma;
        switch(UartDMA_arrCfgs[i].Mode){
            case Usart_enu_Tx:
            USART_enuEnableDMA_TX(Loc_enuUart);
            Loc_stDmaCfg.TransferType = DMA_enuMEM2PER;
            Loc_stDmaCfg.Stream = UartDMA_arrDMAdata[Loc_enuUart].stream[UART_DMA_ON_TX];
            break;

            case Usart_enu_Rx:
            USART_enuEnableDMA_RX(Loc_enuUart);
            Loc_stDmaCfg.TransferType = DMA_enuPER2MEM;
            Loc_stDmaCfg.Stream = UartDMA_arrDMAdata[Loc_enuUart].stream[UART_DMA_ON_RX];
            break;

            default:
            break;
        }
        Loc_stDmaCfg.Channel = UartDMA_arrDMAdata[Loc_enuUart].channel;
        Loc_stDmaCfg.BuffSize = 0;
        Loc_stDmaCfg.BuffType = DMA_enuSingleBuff;
        Loc_stDmaCfg.Circularity = DMA_enuNonCirc;
        Loc_stDmaCfg.memIncState =  DMA_enuIncEn;
        Loc_stDmaCfg.perIncState = DMA_enuIncDis;
        Loc_stDmaCfg.Priority = DMA_enuLOW_p;
        Loc_stDmaCfg.wordSize = DMA_enu8bit;
        /*Dummy adresses*/
        Loc_stDmaCfg.srcAdd = (void*)UartDMA_arrDefault;
        Loc_stDmaCfg.dstAdd1 = (void*)UartDMA_arrDefault;
        DMA_enuInit(&Loc_stDmaCfg);
 
        switch(UartDMA_arrCfgs[i].Mode){
            case Usart_enu_Tx:
            DMA_enuChangePerAddr(
                UartDMA_arrDMAdata[Loc_enuUart].DMA,
                UartDMA_arrDMAdata[Loc_enuUart].stream[UART_DMA_ON_TX],
                UartDMA_arrUSART_DR_Add[Loc_enuUart]
            );
            break;

            case Usart_enu_Rx:
            DMA_enuChangePerAddr(
                UartDMA_arrDMAdata[Loc_enuUart].DMA,
                UartDMA_arrDMAdata[Loc_enuUart].stream[UART_DMA_ON_RX],
                UartDMA_arrUSART_DR_Add[Loc_enuUart]
            );
            break;

            default:
            break;
        }
    }
}

UartDMA_enuErrorStatus_t UartDMA_enuRegisterCallBack(uint8_t Copy_u8Line,
DMA_CB Add_cb){

    UartDMA_enuErrorStatus_t Loc_enuStatus = UartDMA_enu_OK;
    DMA_enuStream_t Loc_enuStream = 0;
    NVIC_enuIRQn_t Loc_enuIRQn = 0;
    Usart_enuUsarts_t Loc_enuUart = UartDMA_arrCfgs[Copy_u8Line].UART;

    if(UartDMA_arrCfgs[Copy_u8Line].Mode == Usart_enu_Tx){
        Loc_enuStream = UartDMA_arrDMAdata[Loc_enuUart].stream[UART_DMA_ON_TX];
        Loc_enuIRQn = UartDMA_arrDMAdata[Loc_enuUart].IRQn[UART_DMA_ON_TX];
    }
    else{
        Loc_enuStream = UartDMA_arrDMAdata[Loc_enuUart].stream[UART_DMA_ON_RX];
        Loc_enuIRQn = UartDMA_arrDMAdata[Loc_enuUart].IRQn[UART_DMA_ON_RX];

    }
    
    if(Copy_u8Line >= UART_DMA_NO_OF_LINES){
        Loc_enuStatus = UartDMA_enu_INVALID_LINE;
    }
    else{
        DMA_enuRegisterCb(
            UartDMA_arrDMAdata[Loc_enuUart].DMA,
            Loc_enuStream,
            Add_cb,
            DMA_enuOnFullTransfer
        );
        NVIC_enuClearPendingIRQn(Loc_enuIRQn);
        NVIC_enuEnableIRQn(Loc_enuIRQn);
    }
    return Loc_enuStatus;
}

UartDMA_enuErrorStatus_t UartDMA_enuStartTransfer(uint8_t Copy_u8Line){
    
    UartDMA_enuErrorStatus_t Loc_enuStatus = UartDMA_enu_OK;
    DMA_enuStream_t Loc_enuStream = 0;
    Usart_enuUsarts_t Loc_enuUart = UartDMA_arrCfgs[Copy_u8Line].UART;

    if(UartDMA_arrCfgs[Copy_u8Line].Mode == Usart_enu_Tx){
        Loc_enuStream = UartDMA_arrDMAdata[Loc_enuUart].stream[UART_DMA_ON_TX];
    }
    else{
        Loc_enuStream = UartDMA_arrDMAdata[Loc_enuUart].stream[UART_DMA_ON_RX];
    }

    if(Copy_u8Line >= UART_DMA_NO_OF_LINES){
        Loc_enuStatus = UartDMA_enu_INVALID_LINE;
    }
    else if(UartDMA_arrBuffData[Copy_u8Line].address == NULL){
        Loc_enuStatus = UartDMA_enu_NULL_POINTER;
    }
    else{
        DMA_enuStopTransfer(
            UartDMA_arrDMAdata[Loc_enuUart].DMA,
            Loc_enuStream
        );
        Dma_enuClearFlags(
            UartDMA_arrDMAdata[Loc_enuUart].DMA,
            Loc_enuStream
        );
        DMA_enuChangeM0Addr(
            UartDMA_arrDMAdata[Loc_enuUart].DMA,
            Loc_enuStream,
            UartDMA_arrBuffData[Copy_u8Line].address
        );
        DMA_enuResizeBuffer(
            UartDMA_arrDMAdata[Loc_enuUart].DMA,
            Loc_enuStream,
            UartDMA_arrBuffData[Copy_u8Line].size
        );
        DMA_enuStartTransfer(
            UartDMA_arrDMAdata[Loc_enuUart].DMA,
            Loc_enuStream
        );
    }
    return Loc_enuStatus;
}

UartDMA_enuErrorStatus_t UartDMA_enuStopTransfer(uint8_t Copy_u8Line){
    
    UartDMA_enuErrorStatus_t Loc_enuStatus = UartDMA_enu_OK;
    DMA_enuStream_t Loc_enuStream = 0;
    Usart_enuUsarts_t Loc_enuUart = UartDMA_arrCfgs[Copy_u8Line].UART;

    if(UartDMA_arrCfgs[Copy_u8Line].Mode == Usart_enu_Tx){
        Loc_enuStream = UartDMA_arrDMAdata[Loc_enuUart].stream[UART_DMA_ON_TX];
    }
    else{
        Loc_enuStream = UartDMA_arrDMAdata[Loc_enuUart].stream[UART_DMA_ON_RX];
    }

    if(Copy_u8Line >= UART_DMA_NO_OF_LINES){
        Loc_enuStatus = UartDMA_enu_INVALID_LINE;
    }
    else if(UartDMA_arrBuffData[Copy_u8Line].address == NULL){
        Loc_enuStatus = UartDMA_enu_NULL_POINTER;
    }
    else{
        DMA_enuStopTransfer(
            UartDMA_arrDMAdata[Loc_enuUart].DMA,
            Loc_enuStream
        );   
    }
    return Loc_enuStatus;
}

UartDMA_enuErrorStatus_t UartDma_enuResumeTransfer(uint8_t Copy_u8Line){

    UartDMA_enuErrorStatus_t Loc_enuStatus = UartDMA_enu_OK;
    DMA_enuStream_t Loc_enuStream = 0;
    Usart_enuUsarts_t Loc_enuUart = UartDMA_arrCfgs[Copy_u8Line].UART;

    if(UartDMA_arrCfgs[Copy_u8Line].Mode == Usart_enu_Tx){
        Loc_enuStream = UartDMA_arrDMAdata[Loc_enuUart].stream[UART_DMA_ON_TX];
    }
    else{
        Loc_enuStream = UartDMA_arrDMAdata[Loc_enuUart].stream[UART_DMA_ON_RX];
    }

    if(Copy_u8Line >= UART_DMA_NO_OF_LINES){
        Loc_enuStatus = UartDMA_enu_INVALID_LINE;
    }
    else if(UartDMA_arrBuffData[Copy_u8Line].address == NULL){
        Loc_enuStatus = UartDMA_enu_NULL_POINTER;
    }
    else{
        DMA_enuStartTransfer(
            UartDMA_arrDMAdata[Loc_enuUart].DMA,
            Loc_enuStream
        );   
    }
    return Loc_enuStatus;
}

UartDMA_enuErrorStatus_t UartDMA_enuRegisterBuff(uint8_t Copy_u8Line,
void* Add_vBuff,uint16_t Copy_u16Size){

    UartDMA_enuErrorStatus_t Loc_enuStatus = UartDMA_enu_OK;
    Usart_enuUsarts_t Loc_enuUart = 0;

    if(Copy_u8Line >= UART_DMA_NO_OF_LINES){
        Loc_enuStatus = UartDMA_enu_INVALID_LINE;
    }
    else if(Add_vBuff == NULL){
        Loc_enuStatus = UartDMA_enu_NULL_POINTER;
    }
    else{
        Loc_enuUart = UartDMA_arrCfgs[Copy_u8Line].UART;
        UartDMA_arrBuffData[Copy_u8Line].address = Add_vBuff;
        UartDMA_arrBuffData[Copy_u8Line].size = Copy_u16Size;
        DMA_enuStream_t Loc_enuStream = 0;
       
        if(UartDMA_arrCfgs[Copy_u8Line].Mode == Usart_enu_Tx){
            Loc_enuStream = UartDMA_arrDMAdata[Loc_enuUart].stream[UART_DMA_ON_TX];
        }   
        else{
            Loc_enuStream = UartDMA_arrDMAdata[Loc_enuUart].stream[UART_DMA_ON_RX];
        }

        DMA_enuStopTransfer(
            UartDMA_arrDMAdata[Loc_enuUart].DMA,
            Loc_enuStream
        );
        Dma_enuClearFlags(
            UartDMA_arrDMAdata[Loc_enuUart].DMA,
            Loc_enuStream    
        );
        DMA_enuChangeM0Addr(
            UartDMA_arrDMAdata[Loc_enuUart].DMA,
            Loc_enuStream,
            Add_vBuff       
        );
        DMA_enuResizeBuffer(
            UartDMA_arrDMAdata[Loc_enuUart].DMA,
            Loc_enuStream,
            Copy_u16Size  
        );
    }
    return Loc_enuStatus;
}   