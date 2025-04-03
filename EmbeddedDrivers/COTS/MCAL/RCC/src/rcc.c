#include "rcc.h"

#define RCC_BASE            0x40023800

#define RCC_CLK_TIMEOUT     5000
#define RCC_CLK_VRF_MASK    0xFEFEFFFEUL
#define RCC_CFGR_SW_MASK    0xFFFFFFFCUL

#define RCC_SEL_HSI_MASK    0x00000000UL
#define RCC_SEL_HSE_MASK    0x00000001UL
#define RCC_SEL_PLL_MASK    0x00000002UL

#define RCC_PLL_P_MASK      0xFFFCFFFFUL
#define RCC_PLL_M_MASK      0xFFFFFFE0UL
#define RCC_PLL_N_MASK      0xFFFF803FUL
#define RCC_PLL_Q_MASK      0xF0FFFFFFUL

#define HSE_BYP_MASK        0x0004000UL

#define RCC_HSI_FREQ        16000000UL
#define RCC_HSE_FREQ        8000000UL
#define RCC_HSE_BYP_FREQ    25000000UL 

#define RCC_VCO_MIN         1000000UL
#define RCC_VCO_MAX         2000000UL

#define RCC_AHB1_MASK       0x000000000ULL   
#define RCC_AHB2_MASK       0x100000000ULL   
#define RCC_APB1_MASK       0x200000000ULL   
#define RCC_APB2_MASK       0x400000000ULL   

#define RCC_AHB1_PER_MASK   0xFF9FEF60UL
#define RCC_AHB2_PER_MASK   0xFFFFFF7FUL
#define RCC_APB1_PER_MASK   0xEF1D37F0UL
#define RCC_APB2_PER_MASK   0xFFF886CEUL

#define RCC_CFGR_HPRE_MASK  0xFFFFFF0FUL
#define RCC_CFGR_PPR1_MASK  0xFFFFE3FFUL
#define RCC_CFGR_PPR2_MASK  0xFFFF1FFFUL


typedef struct {
    volatile uint32_t CR;
    volatile uint32_t PLLCFGR;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;
    volatile uint32_t AHB1RSTR;
    volatile uint32_t AHB2RSTR;
    volatile const uint32_t Reserved1[2];  
    volatile uint32_t APB1RSTR;
    volatile uint32_t APB2RSTR;
    volatile const uint32_t Reserved2[2];  
    volatile uint32_t AHB1ENR;
    volatile uint32_t AHB2ENR;
    volatile const uint32_t Reserved3[2];  
    volatile uint32_t APB1ENR;
    volatile uint32_t APB2ENR;
    volatile const uint32_t Reserved4[2];  
    volatile uint32_t AHB1LPENR;
    volatile uint32_t AHB2LPENR;
    volatile const uint32_t Reserved5[2];  
    volatile uint32_t APB1LPENR;
    volatile uint32_t APB2LPENR;
    volatile uint32_t BDCR;
    volatile uint32_t CSR;
    volatile uint32_t SSCGR;
    volatile uint32_t PLLI2SCFGR;
    volatile const uint32_t Reserved6;  
    volatile uint32_t DCKCFGR;
} RCC_stRegs_t;

typedef struct{
    uint8_t AHB; 
    uint8_t APB1;
    uint8_t APB2;
}RCC_stBusPrescalers_t;


RCC_stPllParams_t RCC_stPLLCurrParams = {0};
RCC_stBusPrescalers_t RCC_stCurrBusPres = {0,0,0};

static const uint8_t RCC_arrHPREmap[9] = {
    0b00000000,
    0b00001000,
    0b00001001,
    0b00001010,
    0b00001011,
    0b00001100,
    0b00001101,
    0b00001110,
    0b00001111
};

static const uint8_t RCC_arrPPRmap[5]={
    0b00000000,
    0b00000100,
    0b00000101,
    0b00000110,
    0b00000111
};


RCC_enuErrorStatus_t RCC_enuCtlClk(uint32_t Copy_u32Clk,uint32_t Copy_u32State){
    
    RCC_enuErrorStatus_t Loc_enuStatus = RCC_enu_OK;
    RCC_stRegs_t* RCC = (RCC_stRegs_t*)RCC_BASE;
    uint16_t Loc_16TimeOut = RCC_CLK_TIMEOUT;
    uint8_t Loc_u8ClkState = 0;
    if(Copy_u32Clk&RCC_CLK_VRF_MASK){
        Loc_enuStatus = RCC_enu_INVALID_CLK_TYPE;
    }
    else if(Copy_u32State&&(~Copy_u32State)){
        Loc_enuStatus = RCC_enu_INVALID_CLK_STATE;
    }
    else{
        RCC->CR = (RCC->CR & (~Copy_u32Clk)) | (Copy_u32Clk & Copy_u32State);
        if(Copy_u32State){
            while((!Loc_u8ClkState)&&Loc_16TimeOut--){
                Loc_u8ClkState = ((RCC->CR&(Copy_u32Clk<<1))!=0);
            }
            if(!Loc_u8ClkState){
                Loc_enuStatus = RCC_enu_CLK_TIMEOUT;
            }
            else{}
        }
        else{}
    }
    return Loc_enuStatus;
}

RCC_enuErrorStatus_t RCC_enuClkIsRdy(uint32_t Copy_u32Clk,uint8_t* Add_u8Ret){
    
    RCC_enuErrorStatus_t Loc_enuStatus = RCC_enu_OK;
    RCC_stRegs_t* RCC = (RCC_stRegs_t*)RCC_BASE;
    if(Add_u8Ret==NULL){
        Loc_enuStatus = RCC_enu_NULL_POINTER;
    }
    else if(Copy_u32Clk&RCC_CLK_VRF_MASK){
        Loc_enuStatus = RCC_enu_INVALID_CLK_TYPE;
    }
    else{
        *Add_u8Ret = ((RCC->CR&(Copy_u32Clk<<1))!=0);
    }
    return Loc_enuStatus;
}


RCC_enuErrorStatus_t RCC_selCLK(uint32_t Copy_u32Clk){
    
    uint8_t Loc_u8IsReady = 0;
    RCC_enuErrorStatus_t Loc_enuStatus = RCC_enuClkIsRdy(Copy_u32Clk,&Loc_u8IsReady);
    RCC_stRegs_t* RCC = (RCC_stRegs_t*)RCC_BASE;
    if(!Loc_u8IsReady){
        Loc_enuStatus = RCC_CLK_NOT_RDY;
    }
    else if(Loc_enuStatus==RCC_enu_OK){
        switch (Copy_u32Clk){
            case RCC_CLK_HSI:
                RCC->CFGR &= RCC_CFGR_SW_MASK;
                break;
            case RCC_CLK_HSE:
                RCC->CFGR = (RCC->CFGR & RCC_CFGR_SW_MASK) | RCC_SEL_HSE_MASK;
                break;
            case RCC_CLK_PLL:
                RCC->CFGR = (RCC->CFGR & RCC_CFGR_SW_MASK) | RCC_SEL_PLL_MASK;
                break;
            default:
                break;
        }
    }
    else{}
    return Loc_enuStatus;
}

uint32_t RCC_enuGetSysClk(void){
 
   RCC_stRegs_t* RCC = (RCC_stRegs_t*)RCC_BASE;
   uint32_t Loc_u32SysClk = 0;
   uint8_t Loc_u8SW = ((uint8_t)RCC->CR & 0xC) >> 2 ;
   switch (Loc_u8SW){
        case 0:
            Loc_u32SysClk = RCC_CLK_HSI;
            break;
        case 1:
            Loc_u32SysClk = RCC_CLK_HSE;
            break;
        case 2:
            Loc_u32SysClk = RCC_CLK_PLL;
            break;
        default:
            break;
   }
   return Loc_u32SysClk;
}

uint32_t RCC_enuGetClkFreq(uint32_t Copy_u32Clk,uint32_t* Add_u32ClkFreq){

    RCC_enuErrorStatus_t Loc_enuStatus = RCC_enu_OK;
    uint32_t Loc_u32PLLfactor = RCC_stPLLCurrParams.N/(RCC_stPLLCurrParams.M*
    RCC_stPLLCurrParams.P);
    if(Add_u32ClkFreq==NULL){
        Loc_enuStatus = RCC_enu_NULL_POINTER;
    }
    else{
        switch(Copy_u32Clk){
            case RCC_CLK_HSI:
                *Add_u32ClkFreq = RCC_HSI_FREQ;
                break;
            
            case RCC_CLK_HSE:
                if(RCC_enuIsHSEBYP()){
                    *Add_u32ClkFreq = RCC_HSE_FREQ;
                }
                else{
                    *Add_u32ClkFreq = RCC_HSE_FREQ;
                }
                break;

            case RCC_CLK_PLL:
                if(RCC_stPLLCurrParams.clk==RCC_CLK_HSE){
                    if(RCC_enuIsHSEBYP()){
                        *Add_u32ClkFreq = RCC_HSE_BYP_FREQ *Loc_u32PLLfactor;
                    } 
                    else{
                        *Add_u32ClkFreq = RCC_HSE_FREQ * Loc_u32PLLfactor;
                    }
                }
                else{
                    *Add_u32ClkFreq = RCC_HSI_FREQ * Loc_u32PLLfactor;
                }
                break;
                
            default:
                Loc_enuStatus = RCC_enu_INVALID_CLK_TYPE;
                break;
        }
    }
    return Loc_enuStatus;
}

RCC_enuErrorStatus_t RCC_enuCFGPLL(RCC_stPllParams_t* Add_stParams){
    
    RCC_enuErrorStatus_t Loc_enuStatus = RCC_enu_OK;
    uint8_t Loc_u8IsSelRdy = 0;
    uint8_t Loc_u8IsPLLRdy = 0;
    RCC_stRegs_t* RCC = (RCC_stRegs_t*)RCC_BASE;

    RCC_enuClkIsRdy(RCC_CLK_PLL,&Loc_u8IsPLLRdy);
    RCC_enuClkIsRdy(Add_stParams->clk,&Loc_u8IsSelRdy);
    uint32_t Loc_u32inFreq = 0;
    RCC_enuGetClkFreq(Add_stParams->clk,&Loc_u32inFreq);
    uint32_t loc_u32VCO = Loc_u32inFreq / Add_stParams->M;
    if(Loc_u8IsPLLRdy){
        Loc_enuStatus = RCC_enu_PLL_ON;
    }
    else if(!Loc_u8IsSelRdy){
        Loc_enuStatus = RCC_CLK_NOT_RDY;
    }
    else if(Add_stParams->M>63 || Add_stParams->M<2){
        Loc_enuStatus = RCC_enu_INVALID_PLL_CFG;
    }
    else if(Add_stParams->N>432 || Add_stParams->N<192){
        Loc_enuStatus = RCC_enu_INVALID_PLL_CFG;
    }
    else if(Add_stParams->Q>15||Add_stParams->Q<2){
        Loc_enuStatus = RCC_enu_INVALID_PLL_CFG;
    }
    else if(loc_u32VCO>RCC_VCO_MAX || loc_u32VCO<RCC_VCO_MIN){
        Loc_enuStatus = RCC_enu_INVALID_PLL_CFG;
    }
    else{
        switch (Add_stParams->P){
            case 2:
                RCC->PLLCFGR &= RCC_PLL_P_MASK;
                break;
            case 4:
                RCC->PLLCFGR &= RCC_PLL_P_MASK;
                RCC->PLLCFGR |=0x00010000;
                break;
            
            case 6:
                RCC->PLLCFGR &= RCC_PLL_P_MASK;
                RCC->PLLCFGR |=0x00020000;                
                break;
            
            case 8:
                RCC->PLLCFGR &= RCC_PLL_P_MASK;
                RCC->PLLCFGR |=0x00030000; 
                break;
            
            default:
                Loc_enuStatus = RCC_enu_INVALID_PLL_CFG;
                break;
        }

        if(Loc_enuStatus==RCC_enu_OK){            
            RCC->PLLCFGR &= RCC_PLL_M_MASK;
            RCC->PLLCFGR |= Add_stParams->M;

            RCC->PLLCFGR &=RCC_PLL_N_MASK;
            RCC->PLLCFGR |=((Add_stParams->N)<<6);

            RCC->PLLCFGR &= RCC_PLL_Q_MASK;
            RCC->PLLCFGR |=((Add_stParams->Q)<<24);

            RCC_stPLLCurrParams.M = Add_stParams->M;
            RCC_stPLLCurrParams.N = Add_stParams->N;
            RCC_stPLLCurrParams.P = Add_stParams->P;
            RCC_stPLLCurrParams.Q = Add_stParams->Q;
            RCC_stPLLCurrParams.clk = Add_stParams->clk;
        }   
        else{}    
    }
    return Loc_enuStatus;
}

RCC_enuErrorStatus_t RCC_enuBypassHSE(uint8_t Copy_u8State){

    RCC_enuErrorStatus_t Loc_enuStatus = RCC_enu_OK;
    RCC_stRegs_t* RCC = (RCC_stRegs_t*)RCC_BASE;

    uint8_t Loc_u8IsRdy = 0;
    RCC_enuClkIsRdy(RCC_CLK_HSE,&Loc_u8IsRdy);
    if(Loc_u8IsRdy){
        Loc_enuStatus = RCC_enu_HSE_ON;
    }
    else if(Copy_u8State>RCC_HSE_BY_ON){
        Loc_enuStatus = RCC_enu_INVALID_CLK_STATE;
    }
    else{
        RCC->CR = HSE_BYP_MASK;
        RCC->CR |= Copy_u8State;
    }
    return Loc_enuStatus;
}

uint8_t RCC_enuIsHSEBYP(void){

    RCC_stRegs_t* RCC = (RCC_stRegs_t*)RCC_BASE;
    return ((RCC->CR&HSE_BYP_MASK)!=0);
}

RCC_enuErrorStatus_t RCC_enuCtlPeripheral(uint64_t Copy_u64Per,uint32_t Copy_u32State){
    
    RCC_enuErrorStatus_t Loc_enuStatus = RCC_enu_OK;
    RCC_stRegs_t* RCC = (RCC_stRegs_t*)RCC_BASE;
    uint32_t Loc_u32Per = (uint32_t)Copy_u64Per;
    uint64_t Loc_u64Bus = Copy_u64Per & 0xFFFFFFFF00000000ULL; 
    if(Copy_u32State&&(~Copy_u32State)){
        Loc_enuStatus = RCC_enu_INVALID_CLK_STATE;
    }
    else{
        switch (Loc_u64Bus){
            case RCC_AHB1_MASK:
                if(Loc_u32Per&RCC_AHB1_PER_MASK){
                    Loc_enuStatus = RCC_enu_INVALID_PERIPHERAL;
                }   
                else{
                    RCC->AHB1ENR = (RCC->AHB1ENR & (~Loc_u32Per)) | 
                    (Loc_u32Per & Copy_u32State);
                }             
                break;

            case RCC_AHB2_MASK:
                if(Loc_u32Per&RCC_AHB2_PER_MASK){
                    Loc_enuStatus = RCC_enu_INVALID_PERIPHERAL;
                }   
                else{
                    RCC->AHB2ENR = (RCC->AHB2ENR & (~Loc_u32Per)) | 
                    (Loc_u32Per & Copy_u32State);                }             
                break;
            
            case RCC_APB1_MASK:
                if(Loc_u32Per&RCC_APB1_PER_MASK){
                    Loc_enuStatus = RCC_enu_INVALID_PERIPHERAL;
                }   
                else{
                    RCC->APB1ENR = (RCC->APB1ENR & (~Loc_u32Per)) | 
                    (Loc_u32Per & Copy_u32State);                }             
                break;

            case RCC_APB2_MASK:
                if(Loc_u32Per&RCC_APB2_PER_MASK){
                    Loc_enuStatus = RCC_enu_INVALID_PERIPHERAL;
                }   
                else{
                    RCC->APB2ENR = (RCC->APB2ENR & (~Loc_u32Per)) | 
                    (Loc_u32Per & Copy_u32State);  
                    }             
                break;

            default:
                Loc_enuStatus = RCC_enu_INVALID_PERIPHERAL;
                break;
        }
    }
    return Loc_enuStatus;
}

RCC_enuErrorStatus_t RCC_enuRstPeripheral(uint64_t Copy_u64Per){

    RCC_enuErrorStatus_t Loc_enuStatus = RCC_enu_OK;
    RCC_stRegs_t* RCC = (RCC_stRegs_t*)RCC_BASE;
    uint32_t Loc_u32Per = (uint32_t)Copy_u64Per;
    uint64_t Loc_u64Bus = Copy_u64Per & 0xFFFFFFFF00000000ULL; 
    switch(Loc_u64Bus){
        case RCC_AHB1_MASK:
            if(Loc_u32Per&RCC_AHB1_MASK){
                Loc_enuStatus = RCC_enu_INVALID_PERIPHERAL;
            }
            else{
                RCC->AHB1RSTR |= Loc_u32Per;
                RCC->AHB1RSTR &= ~Loc_u32Per;
            }
            break;

        case RCC_AHB2_MASK:
            if(Loc_u32Per&RCC_AHB2_MASK){
                Loc_enuStatus = RCC_enu_INVALID_PERIPHERAL;
            }
            else{
                RCC->AHB2RSTR |= Loc_u32Per;
                RCC->AHB2RSTR &= ~Loc_u32Per;
            }
            break;

        case RCC_APB1_MASK:
            if(Loc_u32Per&RCC_APB1_MASK){
                Loc_enuStatus = RCC_enu_INVALID_PERIPHERAL;
            }
            else{
                RCC->APB1RSTR |= Loc_u32Per;
                RCC->APB1RSTR &= ~Loc_u32Per;
            }
            break;

        case RCC_APB2_MASK:
            if(Loc_u32Per&RCC_APB2_MASK){
                Loc_enuStatus = RCC_enu_INVALID_PERIPHERAL;
            }
            else{
                RCC->APB2RSTR |= Loc_u32Per;
                RCC->APB2RSTR &= ~Loc_u32Per;
            }
            break;
        default:
            Loc_enuStatus = RCC_enu_INVALID_PERIPHERAL;
            break;
    }
    return Loc_enuStatus;
}

RCC_enuErrorStatus_t RCC_enuSetBusPrescaler(RCC_enuBuses_t Copy_enuBus,
uint32_t Copy_u32Prescaler){
    
    RCC_enuErrorStatus_t Loc_enuStatus = RCC_enu_OK;
    RCC_stRegs_t* RCC = (RCC_stRegs_t*)RCC_BASE;
    uint32_t Loc_u32Prescaler = 0;
    switch (Copy_enuBus){
        case RCC_enu_AHB:
            if(Copy_u32Prescaler>RCC_AHB_PRE_512){
                Loc_enuStatus = RCC_enu_INVALID_PRESCALER;
            }
            else{
                Loc_u32Prescaler = (uint32_t)RCC_arrHPREmap[Copy_u32Prescaler];
                RCC->CFGR = (RCC->CFGR & RCC_CFGR_HPRE_MASK) | (Loc_u32Prescaler << 4);
                RCC_stCurrBusPres.AHB = Copy_u32Prescaler;
            }
            break;
        case RCC_enu_APB1:
            if(Loc_u32Prescaler>RCC_APB1_PRE_16){
                Loc_enuStatus = RCC_enu_INVALID_PRESCALER;
            }
            else{
                Loc_u32Prescaler = (uint32_t)RCC_arrPPRmap[Copy_u32Prescaler];
                RCC->CFGR = (RCC->CFGR&RCC_CFGR_PPR1_MASK) | (Loc_u32Prescaler << 10);
                RCC_stCurrBusPres.APB1 = Copy_u32Prescaler;
            }

            break;
        
        case RCC_enu_APB2:
            if(Loc_u32Prescaler>RCC_APB2_PRE_16){
                Loc_enuStatus = RCC_enu_INVALID_PRESCALER;
            }
            else{
                Loc_u32Prescaler = (uint32_t)RCC_arrPPRmap[Copy_u32Prescaler];
                RCC->CFGR = (RCC->CFGR&RCC_CFGR_PPR2_MASK) | (Loc_u32Prescaler << 13);
                RCC_stCurrBusPres.APB2 = Copy_u32Prescaler;
            }
            break;
        default:
            Loc_enuStatus = RCC_enu_INVALID_BUS;
            break;
        }
    return Loc_enuStatus;
}

RCC_enuErrorStatus_t RCC_getBusFreq(RCC_enuBuses_t Copy_enuBus,
uint32_t* Add_u64Freq){

    RCC_enuErrorStatus_t Loc_enuStatus = RCC_enu_OK;
    uint8_t Loc_u8APB1Pre = 1<<RCC_stCurrBusPres.APB1;
    uint8_t Loc_u8APB2Pre = 1<<RCC_stCurrBusPres.APB2;
    uint8_t Loc_u16AHBpre = 0;

    uint32_t Loc_enuSysClk = RCC_enuGetSysClk();
    uint32_t Loc_enuSysfreq = 0;
    RCC_enuGetClkFreq(Loc_enuSysClk,&Loc_enuSysfreq);
    
    if(Loc_u8APB1Pre>4){
        Loc_u16AHBpre = (1<<(RCC_stCurrBusPres.AHB+1));
    }
    else{
        Loc_u16AHBpre = (1<<RCC_stCurrBusPres.AHB);
    }
    
    if(Add_u64Freq == NULL){
        Loc_enuStatus = RCC_enu_NULL_POINTER;
    }
    else{
        switch(Copy_enuBus){
            case RCC_enu_AHB:
                *Add_u64Freq = Loc_enuSysfreq / Loc_u16AHBpre;
                break;
            case RCC_enu_APB1:
                *Add_u64Freq = Loc_enuSysfreq /(Loc_u16AHBpre*Loc_u8APB1Pre);
                break;
            case RCC_enu_APB2:
                *Add_u64Freq = Loc_enuSysfreq /(Loc_u16AHBpre*Loc_u8APB2Pre);
                break;                
            default:
                Loc_enuStatus = RCC_enu_INVALID_BUS;           
        }
    }
    return Loc_enuStatus; 
}