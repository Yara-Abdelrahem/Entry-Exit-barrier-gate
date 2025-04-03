#include "dma.h"

#define DMA1_BASE                   0x40026000UL 
#define DMA2_BASE                   0x40026400UL 

#define DMA_NO_OF_STREAMS           8
#define DMA_NO_OF_DMAS              2

#define DMA_CR_CH_SEL_MASK          0xF1FFFFFFUL
#define DMA_CR_TR_MASK              0xFFFFFF3FUL
#define DMA_CR_PR_MASK              0xFFFCFFFFUL
#define DMA_CR_M_WORD_SIZE_MASK     0xFFFF9FFFUL
#define DMA_CR_P_WORD_SIZE_MASK     0xFFFFE7FFUL
#define DMA_CR_M_INC_MASK           0xFFFFFBFFUL       
#define DMA_CR_P_INC_MASK           0xFFFFFDFFUL
#define DMA_CR_DBM_MASK             0xFFFBFFFFUL
#define DMA_CR_CIRC_MASK            0xFFFFFEFFUL
#define DMA_CR_EN_MASK              0x00000001UL
#define DMA_CR_DIS_MASK             0xFFFFFFFEUL
#define DMA_CR_INT_MASK             0xFFFFFFE7UL
#define DMA_CR_FT_INT_MASK          0x00000010UL
#define DMA_CR_HT_INT_MASK          0x00000008UL

#define DMA_TCIF0_4_MASK            0x00000020UL
#define DMA_TCIF1_5_MASK            0x00000800UL
#define DMA_TCIF2_6_MASK            0x00200000UL
#define DMA_TCIF3_7_MASK            0x08000000UL

#define DMA_HTIF0_4_MASK            0x00000010UL
#define DMA_HTIF1_5_MASK            0x00000400UL
#define DMA_HTIF2_6_MASK            0x00100000UL
#define DMA_HTIF3_7_MASK            0x04000000UL

#define DMA_CLR_FLAG_S0_4           0x0000003DUL   
#define DMA_CLR_FLAG_S1_5           0x00000F40UL
#define DMA_CLR_FLAG_S2_6           0x003D0000UL
#define DMA_CLR_FLAG_S3_7           0x0F400000UL


static const uint32_t DMA_arrFTmasks[4] = {
    DMA_TCIF0_4_MASK,
    DMA_TCIF1_5_MASK,
    DMA_TCIF2_6_MASK,
    DMA_TCIF3_7_MASK
};
static const uint32_t DMA_arrHTmasks[4] = {
    DMA_HTIF0_4_MASK,
    DMA_HTIF1_5_MASK,
    DMA_HTIF2_6_MASK,
    DMA_HTIF3_7_MASK
};

static const uint32_t DMA_arrClrFlags[4] = {
    DMA_CLR_FLAG_S0_4,
    DMA_CLR_FLAG_S1_5,
    DMA_CLR_FLAG_S2_6,
    DMA_CLR_FLAG_S3_7
};

typedef struct{
    volatile uint32_t CR;
    volatile uint32_t NDTR;
    volatile uint32_t PAR;
    volatile uint32_t M0AR;
    volatile uint32_t M1AR;
    volatile uint32_t FCR;
}DMA_stStreamRegs_t;

typedef struct{
    volatile uint32_t LISR;
    volatile uint32_t HISR;
    volatile uint32_t LIFCR;
    volatile uint32_t HIFCR;
    DMA_stStreamRegs_t stream[DMA_NO_OF_STREAMS];
}DMA_stRegs_t;

static const DMA_stRegs_t* DMA_arrBases[DMA_NO_OF_DMAS] = {
    (DMA_stRegs_t*)DMA1_BASE,
    (DMA_stRegs_t*)DMA2_BASE
};

static DMA_CB DMA_arrCb[DMA_NO_OF_DMAS][DMA_NO_OF_STREAMS] = {0};

DMA_enuErrorStatus_t DMA_enuInit(DMA_stCfg_t* add_stCfg){

    DMA_enuErrorStatus_t Loc_enuStatus = DMA_enu_OK;

    if((add_stCfg == NULL) || (add_stCfg->srcAdd == NULL) || (add_stCfg->dstAdd1 == NULL)){
        Loc_enuStatus = DMA_enu_NULL_POINTER;
    }
    else if((add_stCfg->BuffType == DMA_enuDoublebuff) && 
    (add_stCfg->dstAdd2 == NULL)){
        Loc_enuStatus = DMA_enu_NULL_POINTER;
    }
    else if(add_stCfg->DMA > DMA_enuDMA2){
        Loc_enuStatus = DMA_enu_INVALID_DMA;
    }
    else if(add_stCfg->Stream > DMA_enuStream7){
        Loc_enuStatus = DMA_enu_INVALID_STREAM;
    }
    else if(add_stCfg->Channel > DMA_enuCH7){
        Loc_enuStatus = DMA_enu_INVALID_CHAN;
    }
    else if(add_stCfg->TransferType > DMA_enuMEM2MEM){
        Loc_enuStatus = DMA_enu_INVALID_TR_TYPE;
    }
    else if(add_stCfg->Priority > DMA_enuVHIGH_p){
        Loc_enuStatus = DMA_enu_INVALID_PRIORITY;
    }
    else if(add_stCfg->wordSize>DMA_enu32bit){
        Loc_enuStatus = DMA_enu_INVALID_BUFF_SIZE;
    }
    else if((add_stCfg->perIncState > DMA_enuIncEn) || 
    (add_stCfg->memIncState)>DMA_enuIncEn){
        Loc_enuStatus = DMA_enu_INVALID_INC_MODE;
    }
    else if(add_stCfg->BuffType > DMA_enuDoublebuff){
        Loc_enuStatus = DMA_enu_INVALID_BUFF_TYPE;
    }
    else{
        DMA_stRegs_t* DMA = (DMA_stRegs_t*)DMA_arrBases[add_stCfg->DMA];

        DMA->stream[add_stCfg->Stream].CR &= DMA_CR_DIS_MASK;

        DMA->stream[add_stCfg->Stream].CR &= DMA_CR_CH_SEL_MASK;
        DMA->stream[add_stCfg->Stream].CR |= add_stCfg->Channel << 25;
       
        DMA->stream[add_stCfg->Stream].CR &= DMA_CR_TR_MASK;
        DMA->stream[add_stCfg->Stream].CR |= add_stCfg->TransferType << 6;
        
        DMA->stream[add_stCfg->Stream].CR &= DMA_CR_PR_MASK;
        DMA->stream[add_stCfg->Stream].CR |= add_stCfg->Priority << 16;
        
        DMA->stream[add_stCfg->Stream].CR &= DMA_CR_M_WORD_SIZE_MASK;
        DMA->stream[add_stCfg->Stream].CR &= DMA_CR_P_WORD_SIZE_MASK;
        DMA->stream[add_stCfg->Stream].CR |= add_stCfg->wordSize << 11;
        DMA->stream[add_stCfg->Stream].CR |= add_stCfg->wordSize << 13;
        
        DMA->stream[add_stCfg->Stream].CR &= DMA_CR_M_INC_MASK;
        DMA->stream[add_stCfg->Stream].CR &= DMA_CR_P_INC_MASK;
        DMA->stream[add_stCfg->Stream].CR |= add_stCfg->memIncState << 10;
        DMA->stream[add_stCfg->Stream].CR |= add_stCfg->perIncState << 9;
        
        DMA->stream[add_stCfg->Stream].CR &= DMA_CR_DBM_MASK;
        DMA->stream[add_stCfg->Stream].CR |= add_stCfg->BuffType << 18;

        DMA->stream[add_stCfg->Stream].CR &= DMA_CR_CIRC_MASK;
        DMA->stream[add_stCfg->Stream].CR |= add_stCfg->Circularity << 8;

        DMA->stream[add_stCfg->Stream].NDTR = add_stCfg->BuffSize;

        switch(add_stCfg->TransferType){
            case DMA_enuPER2MEM:
            DMA->stream[add_stCfg->Stream].PAR = (uint32_t)add_stCfg->srcAdd;
            DMA->stream[add_stCfg->Stream].M0AR = (uint32_t)add_stCfg->dstAdd1;
            if(add_stCfg->BuffType == DMA_enuDoublebuff){
                DMA->stream[add_stCfg->Stream].M1AR = (uint32_t)add_stCfg->dstAdd2;
            }
            else{}
            break;
            
            case DMA_enuMEM2PER:
            DMA->stream[add_stCfg->Stream].PAR = (uint32_t)add_stCfg->dstAdd1;
            DMA->stream[add_stCfg->Stream].M0AR = (uint32_t)add_stCfg->srcAdd;
            break;
            
            case DMA_enuMEM2MEM:
            DMA->stream[add_stCfg->Stream].PAR = (uint32_t)add_stCfg->srcAdd;
            DMA->stream[add_stCfg->Stream].M0AR = (uint32_t)add_stCfg->dstAdd1;
            break;

            default:
            break;
        }
    }
    return Loc_enuStatus;
}

DMA_enuErrorStatus_t DMA_enuStartTransfer(DMA_enuDMA_t Copy_enuDMA,
DMA_enuStream_t Copy_enuStream){

    DMA_enuErrorStatus_t Loc_enuStatus = DMA_enu_OK;
    DMA_stRegs_t* DMA = (DMA_stRegs_t*)DMA_arrBases[Copy_enuDMA];

    if(Copy_enuDMA > DMA_enuDMA2){
        Loc_enuStatus = DMA_enu_INVALID_DMA;
    }
    else if(Copy_enuStream > DMA_enuStream7){
        Loc_enuStatus = DMA_enu_INVALID_STREAM;
    }
    else{
        DMA->stream[Copy_enuStream].CR |= DMA_CR_EN_MASK;
    }

    return Loc_enuStatus;
}

DMA_enuErrorStatus_t DMA_enuStopTransfer(DMA_enuDMA_t Copy_enuDMA,
DMA_enuStream_t Copy_enuStream){

    DMA_enuErrorStatus_t Loc_enuStatus = DMA_enu_OK;
    DMA_stRegs_t* DMA = (DMA_stRegs_t*)DMA_arrBases[Copy_enuDMA];

    if(Copy_enuDMA > DMA_enuDMA2){
        Loc_enuStatus = DMA_enu_INVALID_DMA;
    }
    else if(Copy_enuStream > DMA_enuStream7){
        Loc_enuStatus = DMA_enu_INVALID_STREAM;
    }
    else{
        DMA->stream[Copy_enuStream].CR &= DMA_CR_DIS_MASK;
    }

    return Loc_enuStatus;
}


DMA_enuErrorStatus_t DMA_enuRegisterCb(DMA_enuDMA_t copy_enuDMA,
DMA_enuStream_t Copy_enuStream,DMA_CB Add_cb,DMA_enuEvents_t Copy_enuEvent){
    
    DMA_enuErrorStatus_t Loc_enuStatus = DMA_enu_OK;
    
    if(Add_cb == NULL){
        Loc_enuStatus = DMA_enu_NULL_POINTER;
    }   
    else if(Copy_enuEvent > DMA_enuOnHalfAndFullTransfer){
        Loc_enuStatus = DMA_enu_INVALID_EVENT;
    }
    else if(copy_enuDMA > DMA_enuDMA2){
        Loc_enuStatus = DMA_enu_INVALID_DMA;
    }
    else if(Copy_enuStream > DMA_enuStream7){
        Loc_enuStatus = DMA_enu_INVALID_STREAM;
    }
    else{
        DMA_stRegs_t* DMA = (DMA_stRegs_t*)DMA_arrBases[copy_enuDMA];
        DMA_arrCb[copy_enuDMA][Copy_enuStream] = Add_cb;    
        DMA->stream[Copy_enuStream].CR &= DMA_CR_INT_MASK;
        switch(Copy_enuEvent){
            case DMA_enuOnFullTransfer:
            DMA->stream[Copy_enuStream].CR |= DMA_CR_FT_INT_MASK;
            break;

            case DMA_enuOnHalfTransfer:
            DMA->stream[Copy_enuStream].CR |= DMA_CR_HT_INT_MASK;
            break;

            case DMA_enuOnHalfAndFullTransfer:
            DMA->stream[Copy_enuStream].CR |= DMA_CR_HT_INT_MASK;
            DMA->stream[Copy_enuStream].CR |= DMA_CR_HT_INT_MASK;
            break;
            
            default:
            break;
        }
    }
    return Loc_enuStatus;
}

DMA_enuErrorStatus_t DMA_enuResizeBuffer(DMA_enuDMA_t Copy_enuDMA,
DMA_enuStream_t Copy_enuStream,uint16_t Copy_u16Size){
    
    DMA_enuErrorStatus_t Loc_enuStatus = DMA_enu_OK;
    DMA_stRegs_t* DMA = (DMA_stRegs_t*)DMA_arrBases[Copy_enuDMA];

    if(Copy_enuDMA > DMA_enuDMA2){
        Loc_enuStatus = DMA_enu_INVALID_DMA;
    }
    else if(Copy_enuStream > DMA_enuStream7){
        Loc_enuStatus = DMA_enu_INVALID_STREAM;
    }
    else{
        DMA->stream[Copy_enuStream].CR &= DMA_CR_DIS_MASK;
        DMA->stream[Copy_enuStream].NDTR = Copy_u16Size;
    }
    return Loc_enuStatus;
}

DMA_enuErrorStatus_t DMA_enuChangePerAddr(DMA_enuDMA_t Copy_enuDMA,
DMA_enuStream_t Copy_enuStream,void* add_Per){

    DMA_enuErrorStatus_t Loc_enuStatus = DMA_enu_OK;
    DMA_stRegs_t* DMA = (DMA_stRegs_t*)DMA_arrBases[Copy_enuDMA];

    if(add_Per == NULL){
        Loc_enuStatus = DMA_enu_NULL_POINTER;
    }
    else if(Copy_enuDMA > DMA_enuDMA2){
        Loc_enuStatus = DMA_enu_INVALID_DMA;
    }
    else if(Copy_enuStream > DMA_enuStream7){
        Loc_enuStatus = DMA_enu_INVALID_STREAM;
    }
    else{
        DMA->stream[Copy_enuStream].CR &= DMA_CR_DIS_MASK;
        DMA->stream[Copy_enuStream].PAR = (uint32_t)add_Per;
    }
    return Loc_enuStatus;
}

DMA_enuErrorStatus_t DMA_enuChangeM0Addr(DMA_enuDMA_t Copy_enuDMA,
    DMA_enuStream_t Copy_enuStream,void* add_M0){
    
        DMA_enuErrorStatus_t Loc_enuStatus = DMA_enu_OK;
        DMA_stRegs_t* DMA = (DMA_stRegs_t*)DMA_arrBases[Copy_enuDMA];
    
        if(add_M0 == NULL){
            Loc_enuStatus = DMA_enu_NULL_POINTER;
        }
        else if(Copy_enuDMA > DMA_enuDMA2){
            Loc_enuStatus = DMA_enu_INVALID_DMA;
        }
        else if(Copy_enuStream > DMA_enuStream7){
            Loc_enuStatus = DMA_enu_INVALID_STREAM;
        }
        else{
            DMA->stream[Copy_enuStream].CR &= DMA_CR_DIS_MASK;
            DMA->stream[Copy_enuStream].M0AR = (uint32_t)add_M0;
        }
        return Loc_enuStatus;
}

DMA_enuErrorStatus_t DMA_enuChangeM1Addr(DMA_enuDMA_t Copy_enuDMA,
    DMA_enuStream_t Copy_enuStream,void* add_M1){
    
        DMA_enuErrorStatus_t Loc_enuStatus = DMA_enu_OK;
        DMA_stRegs_t* DMA = (DMA_stRegs_t*)DMA_arrBases[Copy_enuDMA];
    
        if(add_M1 == NULL){
            Loc_enuStatus = DMA_enu_NULL_POINTER;
        }
        else if(Copy_enuDMA > DMA_enuDMA2){
            Loc_enuStatus = DMA_enu_INVALID_DMA;
        }
        else if(Copy_enuStream > DMA_enuStream7){
            Loc_enuStatus = DMA_enu_INVALID_STREAM;
        }
        else{
            DMA->stream[Copy_enuStream].CR &= DMA_CR_DIS_MASK;
            DMA->stream[Copy_enuStream].M1AR = (uint32_t)add_M1;
        }
        return Loc_enuStatus;
}

inline static void DMA_vGenericCb(DMA_enuDMA_t Copy_enuDMA,
DMA_enuStream_t Copy_enuStream){
    
    DMA_stRegs_t* DMA = (DMA_stRegs_t*)DMA_arrBases[Copy_enuDMA];
    uint8_t Loc_u8StreamPos = (uint8_t)(Copy_enuStream%4);
    uint32_t Loc_u32streamHTMask = DMA_arrHTmasks[Loc_u8StreamPos];
    uint32_t Loc_u32streamFTMask = DMA_arrFTmasks[Loc_u8StreamPos];
    
    uint32_t* Loc_u32StatusReg =NULL;
    uint32_t* Loc_u32ClearReg=NULL;

    if(Copy_enuStream > 3){
        Loc_u32StatusReg = (uint32_t*)&DMA->HISR;
        Loc_u32ClearReg = (uint32_t*)&DMA->HIFCR;   
    }
    else{
        Loc_u32StatusReg = (uint32_t*)&DMA->LISR;
        Loc_u32ClearReg = (uint32_t*)&DMA->LIFCR; 
    }

    if(DMA_arrCb[Copy_enuDMA][Copy_enuStream]!=NULL){
        if((*Loc_u32StatusReg) & Loc_u32streamFTMask){
            *Loc_u32ClearReg |= Loc_u32streamFTMask;
            DMA_arrCb[Copy_enuDMA][Copy_enuStream](DMA_enuOnFullTransfer);
        }

        if((*Loc_u32StatusReg) & Loc_u32streamHTMask){
            *Loc_u32ClearReg |= Loc_u32streamHTMask;
            DMA_arrCb[Copy_enuDMA][Copy_enuStream](DMA_enuOnHalfTransfer);
        }
    }
}

DMA_enuErrorStatus_t DMA_enuClearALlFlags(DMA_enuDMA_t Copy_enuDMA){
    
    DMA_enuErrorStatus_t Loc_enuStatus = DMA_enu_OK;
    DMA_stRegs_t* DMA = (DMA_stRegs_t*)DMA_arrBases[Copy_enuDMA];
    if(Copy_enuDMA > DMA_enuDMA2){
        Loc_enuStatus = DMA_enu_INVALID_DMA;
    }
    else{
        DMA->HIFCR = 0xF7D0F7D;
        DMA->LIFCR = 0xF7D0F7D;
    }
    return Loc_enuStatus;
}

DMA_enuErrorStatus_t Dma_enuClearFlags(DMA_enuDMA_t Copy_enuDMA,
DMA_enuStream_t Copy_enuStream){

    DMA_enuErrorStatus_t Loc_enuStatus = DMA_enu_OK;
    DMA_stRegs_t* DMA = (DMA_stRegs_t*)DMA_arrBases[Copy_enuDMA];

    if(Copy_enuDMA > DMA_enuDMA2){
        Loc_enuStatus = DMA_enu_INVALID_DMA;
    }
    else if(Copy_enuStream > DMA_enuStream7){
        Loc_enuStatus = DMA_enu_INVALID_STREAM;
    }
    else{
        if(Copy_enuStream > DMA_enuStream3){
            DMA->HIFCR = DMA_arrClrFlags[Copy_enuStream-4];
        }
        else{
            DMA->LIFCR = DMA_arrClrFlags[Copy_enuStream];
        }
    } 
    return Loc_enuStatus;
}

void DMA1_Stream0_IRQHandler(void){
    DMA_vGenericCb(DMA_enuDMA1,DMA_enuStream0);
}

void DMA1_Stream1_IRQHandler(void){
    DMA_vGenericCb(DMA_enuDMA1,DMA_enuStream1);
}

void DMA1_Stream2_IRQHandler(void){
    DMA_vGenericCb(DMA_enuDMA1,DMA_enuStream2);
}

void DMA1_Stream3_IRQHandler(void){
    DMA_vGenericCb(DMA_enuDMA1,DMA_enuStream3);
}

void DMA1_Stream4_IRQHandler(void){
    DMA_vGenericCb(DMA_enuDMA1,DMA_enuStream4);
}

void DMA1_Stream5_IRQHandler(void){
    DMA_vGenericCb(DMA_enuDMA1,DMA_enuStream5);
}

void DMA1_Stream6_IRQHandler(void){
    DMA_vGenericCb(DMA_enuDMA1,DMA_enuStream6);
}

void DMA1_Stream7_IRQHandler(void){
    DMA_vGenericCb(DMA_enuDMA1,DMA_enuStream7);
}

void DMA2_Stream0_IRQHandler(void){
    DMA_vGenericCb(DMA_enuDMA2,DMA_enuStream0);
}

void DMA2_Stream1_IRQHandler(void){
    DMA_vGenericCb(DMA_enuDMA2,DMA_enuStream1);
}

void DMA2_Stream2_IRQHandler(void){
    DMA_vGenericCb(DMA_enuDMA2,DMA_enuStream2);
}

void DMA2_Stream3_IRQHandler(void){
    DMA_vGenericCb(DMA_enuDMA2,DMA_enuStream3);
}

void DMA2_Stream4_IRQHandler(void){
    DMA_vGenericCb(DMA_enuDMA2,DMA_enuStream4);
}

void DMA2_Stream5_IRQHandler(void){
    DMA_vGenericCb(DMA_enuDMA2,DMA_enuStream5);
}

void DMA2_Stream6_IRQHandler(void){
    DMA_vGenericCb(DMA_enuDMA2,DMA_enuStream6);
}

void DMA2_Stream7_IRQHandler(void){
    DMA_vGenericCb(DMA_enuDMA2,DMA_enuStream7);
}
