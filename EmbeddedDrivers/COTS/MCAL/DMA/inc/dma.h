#ifndef DMA_H_
#define DMA_H_
#include "std_types.h"

typedef enum{
    DMA_enu_OK,
    DMA_enu_INVALID_DMA,
    DMA_enu_INVALID_STREAM,
    DMA_enu_INVALID_CHAN,
    DMA_enu_INVALID_TR_TYPE,
    DMA_enu_INVALID_PRIORITY,
    DMA_enu_INVALID_BUFF_SIZE,
    DMA_enu_INVALID_INC_MODE,
    DMA_enu_INVALID_BUFF_TYPE,
    DMA_enu_INVALID_EVENT,
    DMA_enu_NULL_POINTER,
    DMA_enu_NOK
}DMA_enuErrorStatus_t;

typedef enum{
    DMA_enuDMA1,
    DMA_enuDMA2
}DMA_enuDMA_t;

typedef enum{
    DMA_enuStream0,
    DMA_enuStream1,    
    DMA_enuStream2,
    DMA_enuStream3,
    DMA_enuStream4,
    DMA_enuStream5,
    DMA_enuStream6,
    DMA_enuStream7 
}DMA_enuStream_t;

typedef enum{
    DMA_enuCH0,
    DMA_enuCH1,
    DMA_enuCH2,
    DMA_enuCH3,
    DMA_enuCH4,
    DMA_enuCH5,
    DMA_enuCH6,
    DMA_enuCH7    
}DMA_enuChannelSel_t;


typedef enum{
    DMA_enuPER2MEM,
    DMA_enuMEM2PER,
    DMA_enuMEM2MEM
}DMA_enuTransferType_t;

typedef enum{
    DMA_enuLOW_p,
    DMA_enuMED_p,
    DMA_enuHIGH_p,
    DMA_enuVHIGH_p
}DMA_enuPriority_t;

typedef enum{
    DMA_enu8bit,
    DMA_enu16bit,
    DMA_enu32bit
}DMA_enuWordSize_t;

typedef enum{
    DMA_enuIncDis,
    DMA_enuIncEn
}DMA_enuIncState;

typedef enum{
    DMA_enuSingleBuff,
    DMA_enuDoublebuff
}DMA_enuBuffType_t;

typedef enum{
    DMA_enuNonCirc,
    DMA_enuCirc
}DMA_enuCirc_t;

typedef struct{
    DMA_enuDMA_t DMA;
    DMA_enuStream_t Stream;
    DMA_enuChannelSel_t Channel;
    DMA_enuTransferType_t TransferType;
    DMA_enuPriority_t Priority;
    DMA_enuWordSize_t wordSize;
    DMA_enuIncState perIncState;
    DMA_enuIncState memIncState;
    void* srcAdd;
    void* dstAdd1;
    void* dstAdd2 /*Used only in double buffer mode*/;
    uint16_t BuffSize;
    DMA_enuBuffType_t BuffType;
    DMA_enuCirc_t Circularity;
}DMA_stCfg_t;

typedef enum{
    DMA_enuOnHalfTransfer,
    DMA_enuOnFullTransfer,
    DMA_enuOnHalfAndFullTransfer
}DMA_enuEvents_t;

typedef void (*DMA_CB)(DMA_enuEvents_t);

extern DMA_enuErrorStatus_t DMA_enuInit(DMA_stCfg_t* add_stCfg);

extern DMA_enuErrorStatus_t DMA_enuStartTransfer(DMA_enuDMA_t Copy_enuDMA,
DMA_enuStream_t Copy_enuStream);

extern DMA_enuErrorStatus_t DMA_enuStopTransfer(DMA_enuDMA_t Copy_enuDMA,
DMA_enuStream_t Copy_enuStream);

extern DMA_enuErrorStatus_t DMA_enuRegisterCb(DMA_enuDMA_t copy_enuDMA,
DMA_enuStream_t Copy_enuStream,DMA_CB Add_cb,DMA_enuEvents_t Copy_enuEvent);

extern DMA_enuErrorStatus_t DMA_enuResizeBuffer(DMA_enuDMA_t Copy_enuDMA,
DMA_enuStream_t Copy_enuStream,uint16_t Copy_u16Size);

extern DMA_enuErrorStatus_t DMA_enuChangePerAddr(DMA_enuDMA_t Copy_enuDMA,
DMA_enuStream_t Copy_enuStream,void* add_Per);

extern DMA_enuErrorStatus_t DMA_enuChangeM0Addr(DMA_enuDMA_t Copy_enuDMA,
DMA_enuStream_t Copy_enuStream,void* add_M0);

extern DMA_enuErrorStatus_t DMA_enuChangeM1Addr(DMA_enuDMA_t Copy_enuDMA,
DMA_enuStream_t Copy_enuStream,void* add_M1);

extern DMA_enuErrorStatus_t DMA_enuClearALlFlags(DMA_enuDMA_t Copy_enuDMA);

extern DMA_enuErrorStatus_t Dma_enuClearFlags(DMA_enuDMA_t Copy_enuDMA,
DMA_enuStream_t Copy_enuStream);

#endif