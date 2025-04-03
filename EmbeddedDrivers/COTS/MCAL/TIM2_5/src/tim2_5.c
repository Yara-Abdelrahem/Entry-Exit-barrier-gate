#include "time2_5.h"

#define TIM2_5_BASE                     0x40000000UL
#define TIM2_5_OFFSET                   0x00000400UL

#define TIM2_5_GET_BASE(TIM)            (TIM2_5_BASE + (TIM2_5_OFFSET * TIM))

#define TIM2_5_MAX_COUNTER_VAL          ((uint16_t)0xFFFF)

#define TIM2_5_CR1_CE_MASK              0x00000001UL
#define TIM2_5_CR1_CD_MASK              0xFFFFFFFEUL
#define TIM2_5_EGR_UG_MASK              0x00000001UL
#define TIM2_5_CCMR1_CH1_CLR_MASK       0xFFFFFF00UL
#define TIM2_5_CCMR1_CH2_CLR_MASK       0xFFFF00FFUL
#define TIM2_5_CCMR2_CH3_CLR_MASK       0xFFFFFF00UL
#define TIM2_5_CCMR2_CH4_CLR_MASK       0xFFFF00FFUL
#define TIM2_5_CCMR_CH1_3_PWM1_MODE     0x00000060UL
#define TIM2_5_CCMR_CH2_4_PWM1_MODE     0x00006000UL
#define TIM2_5_CCER_CCE1_EN_MASK        0x00000001UL
#define TIM2_5_CCER_CCE2_EN_MASK        0x00000010UL
#define TIM2_5_CCER_CCE3_EN_MASK        0x00000100UL
#define TIM2_5_CCER_CCE4_EN_MASK        0x00001000UL
#define TIM2_5_CCER_CCP1_EN_MASK        0x00000002UL
#define TIM2_5_CCER_CCP2_EN_MASK        0x00000020UL
#define TIM2_5_CCER_CCP3_EN_MASK        0x00000200UL
#define TIM2_5_CCER_CCP4_EN_MASK        0x00002000UL

#define TIM2_5_ICU_FILTER_VRF_MASK      ((uint8_t)0xF0)

#define TIM2_5_CR1_OPM_MASK             0x00000008UL

#define TIM2_5_NO_OF_TIMERS             4
#define TIM2_5_NO_OF_CHANNELS           4

typedef struct{
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SMCR;
    volatile uint32_t DIER;
    volatile uint32_t SR;
    volatile uint32_t EGR;
    volatile uint32_t CCMR1;
    volatile uint32_t CCMR2;
    volatile uint32_t CCER;
    volatile uint32_t CNT;
    volatile uint32_t PSC;
    volatile uint32_t ARR;
    volatile uint32_t Reserved1;
    volatile uint32_t CCR1;
    volatile uint32_t CCR2;
    volatile uint32_t CCR3;
    volatile uint32_t CCR4;
    volatile uint32_t Reserved2;
    volatile uint32_t DCR;
    volatile uint32_t DMAR;
    volatile uint32_t OR;
}TIM2_5_stRegs_t;

typedef struct{
    TIM2_5_ICU_CB CallBack;
    TIM2_5_enuICUModes_t mode;
    uint8_t isConfigured;
}TIM2_5_stICUdata_t;

TIM2_5_stICUdata_t TIM2_5_arrICUdata[TIM2_5_NO_OF_TIMERS][TIM2_5_NO_OF_CHANNELS] = {{0}};

TIM2_5_enuErrorStatus_t TIM2_5_enuCfgPWM(TIM2_5_stPWMCfg_t* Add_stCfg){

    TIM2_5_enuErrorStatus_t Loc_enuStatus = TIM2_5_enu_OK;
    TIM2_5_stRegs_t* TIM = (TIM2_5_stRegs_t*)TIM2_5_GET_BASE(Add_stCfg->Timer);
    uint32_t Loc_u32Prescaler = 0;
    uint32_t Loc_u32PeriodTicks = 0;
    uint32_t Loc_u32PulseTicks = 0;
    uint32_t Loc_u32TargetTicks = Add_stCfg->SystemClkFreq/Add_stCfg->Frequency;

    if(Add_stCfg == NULL){
        Loc_enuStatus = TIM2_5_enu_NULL_POINTER;
    }
    else if(Add_stCfg->Timer > TIM2_5_enuTIM5){
        Loc_enuStatus = TIM2_5_enu_INVALID_TIMER;
    }
    else if(Add_stCfg->Channel > TIM2_5_enuCH4){
        Loc_enuStatus = TIM2_5_enu_INVALID_CHANNEL;
    }
    else {
        if(Loc_u32TargetTicks > TIM2_5_MAX_COUNTER_VAL){
            Loc_u32Prescaler = (Loc_u32TargetTicks/TIM2_5_MAX_COUNTER_VAL) + 1;
            Loc_u32PeriodTicks = (Loc_u32TargetTicks/Loc_u32Prescaler) - 1;
        }
        else{
            Loc_u32Prescaler = 0;
            Loc_u32PeriodTicks = Loc_u32TargetTicks - 1;
        }

        TIM->CR1 = 0;
        TIM->PSC = Loc_u32Prescaler;
        TIM->ARR = Loc_u32PeriodTicks;
        TIM->EGR = TIM2_5_EGR_UG_MASK; 
        Loc_u32PulseTicks = (TIM->ARR * Add_stCfg->DutyCycle)/100.0;

        switch(Add_stCfg->Channel){
            case TIM2_5_enuCH1:
            TIM->CCMR1 &= TIM2_5_CCMR1_CH1_CLR_MASK;
            TIM->CCMR1 |= TIM2_5_CCMR_CH1_3_PWM1_MODE;
            TIM->CCER |= TIM2_5_CCER_CCE1_EN_MASK;
            TIM->CCR1 = Loc_u32PulseTicks;
            break;

            case TIM2_5_enuCH2:
            TIM->CCMR1 &= TIM2_5_CCMR1_CH2_CLR_MASK;
            TIM->CCMR1 |= TIM2_5_CCMR_CH2_4_PWM1_MODE;
            TIM->CCER |= TIM2_5_CCER_CCE2_EN_MASK;
            TIM->CCR2 = Loc_u32PulseTicks;
            break;

            case TIM2_5_enuCH3:
            TIM->CCMR2 &= TIM2_5_CCMR2_CH3_CLR_MASK;
            TIM->CCMR2 |= TIM2_5_CCMR_CH1_3_PWM1_MODE;
            TIM->CCER |= TIM2_5_CCER_CCE3_EN_MASK;
            TIM->CCR3 = Loc_u32PulseTicks;
            break;

            case TIM2_5_enuCH4:
            TIM->CCMR2 &= TIM2_5_CCMR2_CH4_CLR_MASK;
            TIM->CCMR2 |= TIM2_5_CCMR_CH2_4_PWM1_MODE;
            TIM->CCER |= TIM2_5_CCER_CCE4_EN_MASK;
            TIM->CCR4 = Loc_u32PulseTicks;
            break;

            default:
            break;
        }
        TIM->CR1 |= TIM2_5_CR1_CE_MASK;
    }
    return Loc_enuStatus;
}

TIM2_5_enuErrorStatus_t TIM2_5_enuSetDutyCycle(TIM2_5_enuTimers_t Timer, 
TIM2_5_enuChannels_t Copy_enuChannel, float32_t Copy_f32DutyCycle) {

    TIM2_5_enuErrorStatus_t Loc_enuStatus = TIM2_5_enu_OK;
    TIM2_5_stRegs_t* TIM = (TIM2_5_stRegs_t*)TIM2_5_GET_BASE(Timer);
    uint32_t pulse = (uint32_t)((Copy_f32DutyCycle * TIM->ARR) / 100.0);
    
    if(Timer > TIM2_5_enuTIM5){
        Loc_enuStatus = TIM2_5_enu_INVALID_TIMER;
    }
    else if(Copy_f32DutyCycle > 100){
        Loc_enuStatus = TIM2_5_enu_INVALID_DUTY_CYLCE;
    }
    else{
        switch(Copy_enuChannel){
            case TIM2_5_enuCH1: 
            TIM->CCR1 = pulse; 
            break;

            case TIM2_5_enuCH2: 
            TIM->CCR2 = pulse; 
            break;

            case TIM2_5_enuCH3: 
            TIM->CCR3 = pulse; 
            break;
        
            case TIM2_5_enuCH4: 
            TIM->CCR4 = pulse; 
            break;

            default:
            Loc_enuStatus = TIM2_5_enu_INVALID_CHANNEL;
            break;
        }
    }
    return Loc_enuStatus;
}

TIM2_5_enuErrorStatus_t TIM2_5_enuICUInit(TIM2_5_stICUCfg_t* Add_stCfg){
   
    TIM2_5_enuErrorStatus_t Loc_enuStatus = TIM2_5_enu_OK;
    TIM2_5_stRegs_t* TIM = (TIM2_5_stRegs_t*)TIM2_5_GET_BASE(Add_stCfg->Timer);
    uint8_t Loc_u8ChannelOffset = 0;
    if(Add_stCfg == NULL){
        Loc_enuStatus = TIM2_5_enu_NULL_POINTER;
    } 
    else if(Add_stCfg->Timer > TIM2_5_enuTIM5){
        Loc_enuStatus = TIM2_5_enu_INVALID_TIMER;
    }
    else if(Add_stCfg->Channel > TIM2_5_enuCH4){
        Loc_enuStatus = TIM2_5_enu_INVALID_CHANNEL;
    }
    else if(Add_stCfg->CallBack == NULL){
        Loc_enuStatus = TIM2_5_enu_NULL_POINTER;
    }
    else if(Add_stCfg->Mode > TIM2_5_enuICUBothEdges){
        Loc_enuStatus = TIM2_5_enu_INVALID_ICU_MODE;
    }
    else if(Add_stCfg->Filter & TIM2_5_ICU_FILTER_VRF_MASK){
        Loc_enuStatus = TIM2_5_enu_INVALID_ICU_FILTER;
    }
    else if(Add_stCfg->Prescaler > TIM2_5_enuICUPre8){
        Loc_enuStatus = TIM2_5_enu_INVALID_ICU_PRE;
    }
    else{
        if(Add_stCfg->Channel < TIM2_5_enuCH3){
            Loc_u8ChannelOffset = 8*Add_stCfg->Channel;
            TIM->CCMR1 &= (~(0xF << (Loc_u8ChannelOffset)));
            TIM->CCMR1 |= ((0x1) << (Loc_u8ChannelOffset));
            TIM->CCMR1 |= ((Add_stCfg->Prescaler) << 
            (Loc_u8ChannelOffset + 2));
            TIM->CCMR1 |= ((Add_stCfg->Filter) << 
            (Loc_u8ChannelOffset + 4));
        }
        else{
            Loc_u8ChannelOffset = 8*(Add_stCfg->Timer - 2);
            TIM->CCMR2 &= (~(0xF << (Loc_u8ChannelOffset)));
            TIM->CCMR2 |= ((0x1) << (Loc_u8ChannelOffset));
            TIM->CCMR1 |= ((Add_stCfg->Prescaler) << 
            (Loc_u8ChannelOffset + 2));
            TIM->CCMR1 |= ((Add_stCfg->Filter) << 
            (Loc_u8ChannelOffset + 4));
        }

        Loc_u8ChannelOffset = 4*Add_stCfg->Channel;
        TIM->CCER |= (1 << Loc_u8ChannelOffset);
        TIM->CCER &= (~(0xA << Loc_u8ChannelOffset));
        switch(Add_stCfg->Mode){

            case TIM2_5_enuICURisingEdge:
            break;

            case TIM2_5_enuICUFallingEdge:
            TIM->CCER |= (0x2 << (Loc_u8ChannelOffset));            
            break;

            case TIM2_5_enuICUBothEdges:
            TIM->CCER |= (0xA << (Loc_u8ChannelOffset));            
            break;

            default:
            break;
        }

        TIM->CCER |= (0x1 << Loc_u8ChannelOffset);

        TIM2_5_arrICUdata[Add_stCfg->Timer][Add_stCfg->Channel].CallBack = Add_stCfg->CallBack;
        TIM2_5_arrICUdata[Add_stCfg->Timer][Add_stCfg->Channel].mode = Add_stCfg->Mode;
        TIM2_5_arrICUdata[Add_stCfg->Timer][Add_stCfg->Channel].isConfigured = TRUE;
    }
    return Loc_enuStatus;    
}

TIM2_5_enuErrorStatus_t TIM2_5_enuICUenableRisingEdge(TIM2_5_enuTimers_t Copy_enuTimer,
TIM2_5_enuChannels_t Copy_enuChannel){
    
    TIM2_5_enuErrorStatus_t Loc_enuStatus = TIM2_5_enu_OK;
    TIM2_5_stRegs_t* TIM = (TIM2_5_stRegs_t*)TIM2_5_GET_BASE(Copy_enuTimer);
    uint8_t Loc_u8ChannelOffset = 0;

    if(Copy_enuTimer > TIM2_5_enuTIM5){
        Loc_enuStatus = TIM2_5_enu_INVALID_TIMER;
    }
    else if(Copy_enuChannel > TIM2_5_enuCH4){
        Loc_enuStatus = TIM2_5_enu_INVALID_CHANNEL;
    }
    else{
        Loc_u8ChannelOffset = 4*Copy_enuChannel;
        TIM->CCER |= (1 << Loc_u8ChannelOffset);
        TIM->CCER &= (~(0xA << Loc_u8ChannelOffset));
    }
    return Loc_enuStatus; 
}

TIM2_5_enuErrorStatus_t TIM2_5_enuICUenableFallingEdge(TIM2_5_enuTimers_t Copy_enuTimer,
TIM2_5_enuChannels_t Copy_enuChannel){
    
    TIM2_5_enuErrorStatus_t Loc_enuStatus = TIM2_5_enu_OK;
    TIM2_5_stRegs_t* TIM = (TIM2_5_stRegs_t*)TIM2_5_GET_BASE(Copy_enuTimer);
    uint8_t Loc_u8ChannelOffset = 0;

    if(Copy_enuTimer > TIM2_5_enuTIM5){
        Loc_enuStatus = TIM2_5_enu_INVALID_TIMER;
    }
    else if(Copy_enuChannel > TIM2_5_enuCH4){
        Loc_enuStatus = TIM2_5_enu_INVALID_CHANNEL;
    }
    else{
        Loc_u8ChannelOffset = 4*Copy_enuChannel;
        TIM->CCER |= (1 << Loc_u8ChannelOffset);
        TIM->CCER &= (~(0xA << Loc_u8ChannelOffset));
        TIM->CCER |= (0x2 << (Loc_u8ChannelOffset));            
    }
    return Loc_enuStatus;
}

TIM2_5_enuErrorStatus_t TIM2_5_enuStartICU(TIM2_5_enuTimers_t Copy_enuTimer,
TIM2_5_enuChannels_t Copy_enuChannel){

    TIM2_5_enuErrorStatus_t Loc_enuStatus = TIM2_5_enu_OK;
    TIM2_5_stRegs_t* TIM = (TIM2_5_stRegs_t*)TIM2_5_GET_BASE(Copy_enuTimer);

    if(Copy_enuTimer > TIM2_5_enuTIM5){
        Loc_enuStatus = TIM2_5_enu_INVALID_TIMER;
    }
    else if(Copy_enuChannel > TIM2_5_enuCH4){
        Loc_enuStatus = TIM2_5_enu_INVALID_CHANNEL;
    }
    else if(!TIM2_5_arrICUdata[Copy_enuTimer][Copy_enuChannel].isConfigured){
        Loc_enuStatus = TIM2_5_enu_ICU_NOT_CONFIGURED;
    }
    else{
        TIM->DIER |= (0x1 << (Copy_enuChannel + 1));
        TIM->CR1 |= TIM2_5_CR1_CE_MASK;
    }

    return Loc_enuStatus;
}

TIM2_5_enuErrorStatus_t TIM2_5_enuStopICU(TIM2_5_enuTimers_t Copy_enuTimer,
TIM2_5_enuChannels_t Copy_enuChannel){
    
    TIM2_5_enuErrorStatus_t Loc_enuStatus = TIM2_5_enu_OK;
    TIM2_5_stRegs_t* TIM = (TIM2_5_stRegs_t*)TIM2_5_GET_BASE(Copy_enuTimer);

    if(Copy_enuTimer > TIM2_5_enuTIM5){
        Loc_enuStatus = TIM2_5_enu_INVALID_TIMER;
    }
    else if(Copy_enuChannel > TIM2_5_enuCH4){
        Loc_enuStatus = TIM2_5_enu_INVALID_CHANNEL;
    }
    else if(!TIM2_5_arrICUdata[Copy_enuTimer][Copy_enuChannel].isConfigured){
        Loc_enuStatus = TIM2_5_enu_ICU_NOT_CONFIGURED;
    }
    else{
        TIM->DIER &= (~(0x1 << (Copy_enuChannel + 1)));
    }

    return Loc_enuStatus;
}

TIM2_5_enuErrorStatus_t TIM2_5_enuGetICUVal(TIM2_5_enuTimers_t Copy_enuTimer,
TIM2_5_enuChannels_t Copy_enuChannel,uint32_t* Add_u32Val){
    
    TIM2_5_enuErrorStatus_t Loc_enuStatus = TIM2_5_enu_OK;
    TIM2_5_stRegs_t* TIM = (TIM2_5_stRegs_t*)TIM2_5_GET_BASE(Copy_enuTimer);
    
    if(Add_u32Val == NULL){ 
        Loc_enuStatus = TIM2_5_enu_NULL_POINTER;
    }
    else if(Copy_enuTimer > TIM2_5_enuTIM5){
        Loc_enuStatus = TIM2_5_enu_INVALID_TIMER;
    }
    else if(!TIM2_5_arrICUdata[Copy_enuTimer][Copy_enuChannel].isConfigured){
        Loc_enuStatus = TIM2_5_enu_ICU_NOT_CONFIGURED;
    }
    else{
        switch(Copy_enuChannel){
            case TIM2_5_enuCH1:
            *Add_u32Val = TIM->CCR1;
            break;

            case TIM2_5_enuCH2:
            *Add_u32Val = TIM->CCR2;
            break;

            case TIM2_5_enuCH3:
            *Add_u32Val = TIM->CCR3;
            break;

            case TIM2_5_enuCH4:
            *Add_u32Val = TIM->CCR4;
            break;
            
            default:
            Loc_enuStatus = TIM2_5_enu_INVALID_CHANNEL;
            break;
        }
    }
    return Loc_enuStatus;
}

TIM2_5_enuErrorStatus_t TIM_enuInitPulse(TIM2_5_stPulseCfg_t* Add_stCfg){
    
    TIM2_5_enuErrorStatus_t Loc_enuStatus = TIM2_5_enu_OK;
    TIM2_5_stRegs_t* TIM = NULL;
    uint32_t Loc_u32TimFreq = 1000000;
    uint32_t Loc_u32prescaler = 0;
    uint32_t Loc_u32pulseTicks = 0;
    if(Add_stCfg == NULL){ 
        Loc_enuStatus = TIM2_5_enu_NULL_POINTER;
    }
    else if(Add_stCfg->Timer > TIM2_5_enuTIM5){
        Loc_enuStatus = TIM2_5_enu_INVALID_TIMER;
    }
    else if(Add_stCfg->Channel > TIM2_5_enuCH4){
        Loc_enuStatus = TIM2_5_enu_INVALID_CHANNEL;
    }
    else{
        TIM = (TIM2_5_stRegs_t*)TIM2_5_GET_BASE(Add_stCfg->Timer);
        TIM->CR1 &= TIM2_5_CR1_CD_MASK;
        Loc_u32prescaler = (Add_stCfg->SystemClockFreq / Loc_u32TimFreq) - 1;
        Loc_u32pulseTicks = Add_stCfg->PulseWidthUs * (Loc_u32TimFreq / 1000000); 

        TIM->PSC = Loc_u32prescaler;
        TIM->ARR = Loc_u32pulseTicks;
        TIM->EGR = TIM2_5_EGR_UG_MASK;

        switch(Add_stCfg->Channel){
            case TIM2_5_enuCH1:
            TIM->CCMR1 &= TIM2_5_CCMR1_CH1_CLR_MASK;
            TIM->CCMR1 |= TIM2_5_CCMR_CH1_3_PWM1_MODE;
            TIM->CCER |= (TIM2_5_CCER_CCE1_EN_MASK );
            TIM->CCER &= (~TIM2_5_CCER_CCP1_EN_MASK);
            TIM->CCR1 = Loc_u32pulseTicks;
            break;

            case TIM2_5_enuCH2:
            TIM->CCMR1 &= TIM2_5_CCMR1_CH2_CLR_MASK;
            TIM->CCMR1 |= TIM2_5_CCMR_CH2_4_PWM1_MODE;
            TIM->CCER |= (TIM2_5_CCER_CCE2_EN_MASK);
            TIM->CCER &= (~TIM2_5_CCER_CCP2_EN_MASK);
            TIM->CCR2 = Loc_u32pulseTicks;
            break;

            case TIM2_5_enuCH3:
            TIM->CCMR2 &= TIM2_5_CCMR2_CH3_CLR_MASK;
            TIM->CCMR2 |= TIM2_5_CCMR_CH1_3_PWM1_MODE;
            TIM->CCER |= (TIM2_5_CCER_CCE3_EN_MASK);
            TIM->CCER &= (~TIM2_5_CCER_CCP1_EN_MASK);
            TIM->CCER &= (~TIM2_5_CCER_CCP3_EN_MASK);
            TIM->CCR3 = Loc_u32pulseTicks;
            break;

            case TIM2_5_enuCH4:
            TIM->CCMR2 &= TIM2_5_CCMR2_CH4_CLR_MASK;
            TIM->CCMR2 |= TIM2_5_CCMR_CH2_4_PWM1_MODE;
            TIM->CCER |= (TIM2_5_CCER_CCE4_EN_MASK);
            TIM->CCER &= (~TIM2_5_CCER_CCP4_EN_MASK);
            TIM->CCR4 = Loc_u32pulseTicks;
            break;

            default:
            break;
        }

        TIM->CR1 |= TIM2_5_CR1_OPM_MASK;  
        TIM->CR1 &= TIM2_5_CR1_CD_MASK;
    }   
    return Loc_enuStatus;
}

TIM2_5_enuErrorStatus_t TIM_enuGeneratePulse(TIM2_5_enuTimers_t Copy_enuTimer){
    
    TIM2_5_enuErrorStatus_t Loc_enuStatus = TIM2_5_enu_OK;
    TIM2_5_stRegs_t* TIM = (TIM2_5_stRegs_t*)TIM2_5_GET_BASE(Copy_enuTimer);

    if(Copy_enuTimer > TIM2_5_enuTIM5){
        Loc_enuStatus = TIM2_5_enu_INVALID_TIMER;
    }
    else{
        TIM->CNT = 0;
        TIM->CR1 |= TIM2_5_CR1_CE_MASK;
    }
    return Loc_enuStatus;
}

static void TIM2_5_vGenericICUCallBack(TIM2_5_enuTimers_t Copy_enuTimer,
TIM2_5_enuChannels_t Copy_enuChannel){

    uint8_t Loc_u8SRoffset = (Copy_enuChannel +1);
    uint8_t Loc_u8CCerOffset = (4*Copy_enuChannel + 1);
    TIM2_5_stRegs_t* TIM = (TIM2_5_stRegs_t*)TIM2_5_GET_BASE(Copy_enuTimer);
    uint32_t Loc_u32CaptureVal = 0; 

    if(TIM->SR & (1 << Loc_u8SRoffset)){
        TIM->SR &= (~(1<<Loc_u8SRoffset));
        TIM2_5_enuGetICUVal(Copy_enuTimer,Copy_enuChannel,&Loc_u32CaptureVal);

        if(TIM2_5_arrICUdata[Copy_enuTimer][Copy_enuChannel].CallBack != NULL){
            TIM2_5_arrICUdata[Copy_enuTimer][Copy_enuChannel].CallBack(Loc_u32CaptureVal);
        }

        if(TIM2_5_arrICUdata[Copy_enuTimer][Copy_enuChannel].mode == TIM2_5_enuICUBothEdges){
            TIM->CCER ^= (1 << Loc_u8CCerOffset);
        }
    }
}

TIM2_5_enuErrorStatus_t TIM2_5_enuSetPrescalerVal(TIM2_5_enuTimers_t Copy_enuTimer,
uint32_t Copy_u32Prescaler){
    
    TIM2_5_enuErrorStatus_t Loc_enuStatus = TIM2_5_enu_OK;
    TIM2_5_stRegs_t* TIM = (TIM2_5_stRegs_t*)TIM2_5_GET_BASE(Copy_enuTimer);

    if(Copy_enuTimer > TIM2_5_enuTIM5){
        Loc_enuStatus = TIM2_5_enu_INVALID_TIMER;
    }
    else{
        TIM->PSC = Copy_u32Prescaler;
    }
    return Loc_enuStatus;
}

TIM2_5_enuErrorStatus_t TIM2_5_enuSetARRVal(TIM2_5_enuTimers_t Copy_enuTimer,
uint32_t Copy_u32ARR){

    TIM2_5_enuErrorStatus_t Loc_enuStatus = TIM2_5_enu_OK;
    TIM2_5_stRegs_t* TIM = (TIM2_5_stRegs_t*)TIM2_5_GET_BASE(Copy_enuTimer);

    if(Copy_enuTimer > TIM2_5_enuTIM5){
        Loc_enuStatus = TIM2_5_enu_INVALID_TIMER;
    }
    else{
        TIM->ARR = Copy_u32ARR;
    }
    return Loc_enuStatus;
}

TIM2_5_enuErrorStatus_t TIM2_5_enuUpdateRegs(TIM2_5_enuTimers_t Copy_enuTimer){
    
    TIM2_5_enuErrorStatus_t Loc_enuStatus = TIM2_5_enu_OK;
    TIM2_5_stRegs_t* TIM = (TIM2_5_stRegs_t*)TIM2_5_GET_BASE(Copy_enuTimer);

    if(Copy_enuTimer > TIM2_5_enuTIM5){
        Loc_enuStatus = TIM2_5_enu_INVALID_TIMER;
    }
    else{
        TIM->EGR = TIM2_5_EGR_UG_MASK;
    }
    return Loc_enuStatus;
}

TIM2_5_enuErrorStatus_t TIM2_5_enuSetCountVal(TIM2_5_enuTimers_t Copy_enuTimer,
uint16_t Copy_u16Val){

    TIM2_5_enuErrorStatus_t Loc_enuStatus = TIM2_5_enu_OK;
    TIM2_5_stRegs_t* TIM = (TIM2_5_stRegs_t*)TIM2_5_GET_BASE(Copy_enuTimer);

    if(Copy_enuTimer > TIM2_5_enuTIM5){
        Loc_enuStatus = TIM2_5_enu_INVALID_TIMER;
    }
    else{
        TIM->CNT = Copy_u16Val;
    }
    return Loc_enuStatus;
}


TIM2_5_enuErrorStatus_t TIM2_5_enuEnableTimer(TIM2_5_enuTimers_t Copy_enuTimer){
    
    TIM2_5_enuErrorStatus_t Loc_enuStatus = TIM2_5_enu_OK;
    TIM2_5_stRegs_t* TIM = (TIM2_5_stRegs_t*)TIM2_5_GET_BASE(Copy_enuTimer);

    if(Copy_enuTimer > TIM2_5_enuTIM5){
        Loc_enuStatus = TIM2_5_enu_INVALID_TIMER;
    }
    else{
        TIM->CR1 |= TIM2_5_CR1_CE_MASK;
    }
    return Loc_enuStatus;
}

TIM2_5_enuErrorStatus_t TIM2_5_enuDisableTimer(TIM2_5_enuTimers_t Copy_enuTimer){
    
    TIM2_5_enuErrorStatus_t Loc_enuStatus = TIM2_5_enu_OK;
    TIM2_5_stRegs_t* TIM = (TIM2_5_stRegs_t*)TIM2_5_GET_BASE(Copy_enuTimer);

    if(Copy_enuTimer > TIM2_5_enuTIM5){
        Loc_enuStatus = TIM2_5_enu_INVALID_TIMER;
    }
    else{
        TIM->CR1 &= TIM2_5_CR1_CD_MASK;
    }
    return Loc_enuStatus;
}

void TIM2_IRQHandler(void){
    if(TIM2_5_arrICUdata[TIM2_5_enuTIM2][TIM2_5_enuCH1].isConfigured){
        TIM2_5_vGenericICUCallBack(TIM2_5_enuTIM2,TIM2_5_enuCH1);
    }
    if(TIM2_5_arrICUdata[TIM2_5_enuTIM2][TIM2_5_enuCH2].isConfigured){
        TIM2_5_vGenericICUCallBack(TIM2_5_enuTIM2,TIM2_5_enuCH2);
    }
    if(TIM2_5_arrICUdata[TIM2_5_enuTIM2][TIM2_5_enuCH3].isConfigured){
        TIM2_5_vGenericICUCallBack(TIM2_5_enuTIM2,TIM2_5_enuCH2);
    }
    if(TIM2_5_arrICUdata[TIM2_5_enuTIM2][TIM2_5_enuCH4].isConfigured){
        TIM2_5_vGenericICUCallBack(TIM2_5_enuTIM2,TIM2_5_enuCH2);
    }
}

void TIM3_IRQHandler(void){
    
    if(TIM2_5_arrICUdata[TIM2_5_enuTIM3][TIM2_5_enuCH1].isConfigured){
        TIM2_5_vGenericICUCallBack(TIM2_5_enuTIM3,TIM2_5_enuCH1);
    }
    if(TIM2_5_arrICUdata[TIM2_5_enuTIM3][TIM2_5_enuCH2].isConfigured){
        TIM2_5_vGenericICUCallBack(TIM2_5_enuTIM3,TIM2_5_enuCH2);
    }
    if(TIM2_5_arrICUdata[TIM2_5_enuTIM3][TIM2_5_enuCH3].isConfigured){
        TIM2_5_vGenericICUCallBack(TIM2_5_enuTIM3,TIM2_5_enuCH2);
    }
    if(TIM2_5_arrICUdata[TIM2_5_enuTIM3][TIM2_5_enuCH4].isConfigured){
        TIM2_5_vGenericICUCallBack(TIM2_5_enuTIM3,TIM2_5_enuCH2);
    } 
}

void TIM4_IRQHandler(void){
    if(TIM2_5_arrICUdata[TIM2_5_enuTIM4][TIM2_5_enuCH1].isConfigured){
        TIM2_5_vGenericICUCallBack(TIM2_5_enuTIM4,TIM2_5_enuCH1);
    }
    if(TIM2_5_arrICUdata[TIM2_5_enuTIM4][TIM2_5_enuCH2].isConfigured){
        TIM2_5_vGenericICUCallBack(TIM2_5_enuTIM4,TIM2_5_enuCH2);
    }
    if(TIM2_5_arrICUdata[TIM2_5_enuTIM4][TIM2_5_enuCH3].isConfigured){
        TIM2_5_vGenericICUCallBack(TIM2_5_enuTIM4,TIM2_5_enuCH2);
    }
    if(TIM2_5_arrICUdata[TIM2_5_enuTIM4][TIM2_5_enuCH4].isConfigured){
        TIM2_5_vGenericICUCallBack(TIM2_5_enuTIM4,TIM2_5_enuCH2);
    } 
}

void TIM5_IRQHandler(void){
    if(TIM2_5_arrICUdata[TIM2_5_enuTIM5][TIM2_5_enuCH1].isConfigured){
        TIM2_5_vGenericICUCallBack(TIM2_5_enuTIM5,TIM2_5_enuCH1);
    }
    if(TIM2_5_arrICUdata[TIM2_5_enuTIM4][TIM2_5_enuCH2].isConfigured){
        TIM2_5_vGenericICUCallBack(TIM2_5_enuTIM5,TIM2_5_enuCH2);
    }
    if(TIM2_5_arrICUdata[TIM2_5_enuTIM4][TIM2_5_enuCH3].isConfigured){
        TIM2_5_vGenericICUCallBack(TIM2_5_enuTIM5,TIM2_5_enuCH2);
    }
    if(TIM2_5_arrICUdata[TIM2_5_enuTIM4][TIM2_5_enuCH4].isConfigured){
        TIM2_5_vGenericICUCallBack(TIM2_5_enuTIM5,TIM2_5_enuCH2);
    } 
}