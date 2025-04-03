#include "GPIO.h"

#define GPIO_NO_OF_PORTS        6 

#define GPIOA_BASE              0x40020000UL    
#define GPIOB_BASE              0x40020400UL
#define GPIOC_BASE              0x40020800UL
#define GPIOD_BASE              0x40020C00UL
#define GPIOE_BASE              0x40021000UL
#define GPIOH_BASE              0x40021C00UL


typedef struct{
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFLR;
    volatile uint32_t AFHR;

}GPIO_stRegs_t;

GPIO_stRegs_t* GPIO_arrPortBases[GPIO_NO_OF_PORTS] = {
    (GPIO_stRegs_t*)GPIOA_BASE,
    (GPIO_stRegs_t*)GPIOB_BASE,
    (GPIO_stRegs_t*)GPIOC_BASE,
    (GPIO_stRegs_t*)GPIOD_BASE,
    (GPIO_stRegs_t*)GPIOE_BASE,
    (GPIO_stRegs_t*)GPIOH_BASE
};

GPIO_enuErrorStatus_t GPIO_enuCfgPin(GPIO_stPinCFG_t* Add_stPinCFG){
    
    GPIO_enuErrorStatus_t Loc_enuStatus = GPIO_enu_OK;
    if(Add_stPinCFG==NULL){
        Loc_enuStatus = GPIO_enu_NULL_POINTER;
    }
    else if(Add_stPinCFG->port>GPIO_enu_GPIOH){
        Loc_enuStatus = GPIO_enu_INVALID_PORT;
    }
    else if(Add_stPinCFG->pin>GPIO_enu_PIN15){
        Loc_enuStatus = GPIO_enu_INVALID_PIN;
    }
    else if(Add_stPinCFG->AltFn>GPIO_enu_NoAF){
        Loc_enuStatus = GPIO_enu_INVALID_AF;
    }
    else if(Add_stPinCFG->mode>GPIO_enu_ANALOG){
        Loc_enuStatus = GPIO_enu_INVALID_MODE;
    }
    else if(Add_stPinCFG->OutputType>GPIO_enu_OPEN_DRAIN){
        Loc_enuStatus = GPIO_enu_INVALID_OUTPUT_TYPE;
    }
    else if(Add_stPinCFG->InputType>GPIO_enu_PD){
        Loc_enuStatus = GPIO_enu_INVALID_INPUT_TYPE;
    }
    else{
        GPIO_stRegs_t* GPIO = (GPIO_stRegs_t*)GPIO_arrPortBases[Add_stPinCFG->port];
        uint32_t Loc_u32AltFnPin = 0;
        GPIO->MODER = (GPIO->MODER& ~(0x3<<(Add_stPinCFG->pin*2))) | 
        (Add_stPinCFG->mode << (Add_stPinCFG->pin*2));
        
        GPIO->OTYPER = (GPIO->OTYPER & ~(0x1<<Add_stPinCFG->pin)) | 
        (Add_stPinCFG->OutputType << Add_stPinCFG->pin);        
        
        GPIO->OSPEEDR = (GPIO->OSPEEDR& ~(0x3<<(Add_stPinCFG->pin*2))) | 
        (Add_stPinCFG->speed << (Add_stPinCFG->pin*2));
        
        GPIO->PUPDR = (GPIO->PUPDR& ~(0x3<<(Add_stPinCFG->pin*2))) | 
        (Add_stPinCFG->InputType << (Add_stPinCFG->pin*2));

        if(Add_stPinCFG->mode==GPIO_enu_ALT_FN){
            if(Add_stPinCFG->AltFn>GPIO_enu_AltFn15){
                Loc_enuStatus = GPIO_enu_INVALID_AF;                
            }
            else if(Add_stPinCFG->pin>GPIO_enu_PIN7){
                Loc_u32AltFnPin = Add_stPinCFG->pin - 8;
                Loc_u32AltFnPin *= 4;
                GPIO->AFHR &= (~(0xF << Loc_u32AltFnPin));
                GPIO->AFHR |= Add_stPinCFG->AltFn << Loc_u32AltFnPin; 
            }
            else{
                Loc_u32AltFnPin = Add_stPinCFG->pin;
                Loc_u32AltFnPin *= 4; 
                GPIO->AFLR &= (~(0xF << Loc_u32AltFnPin));
                GPIO->AFLR |= Add_stPinCFG->AltFn << Loc_u32AltFnPin; 
            }
        }
    }
    return Loc_enuStatus;
}

GPIO_enuErrorStatus_t GPIO_enuSetPinState(GPIO_enuPorts_t Copy_enuPort,
GPIO_enuPins_t Copy_enuPin,uint8_t Copy_u8State){

    GPIO_enuErrorStatus_t Loc_enuStatus = GPIO_enu_OK;
    if(Copy_enuPort>GPIO_enu_GPIOH){
        Loc_enuStatus = GPIO_enu_INVALID_PORT;
    }   
    else if(Copy_enuPin>GPIO_enu_PIN15){
        Loc_enuStatus = GPIO_enu_INVALID_PIN;
    }
    else if(Copy_u8State>GPIO_HIGH_STATE){
        Loc_enuStatus = GPIO_enu_INVALID_STATE;
    } 
    else{
        GPIO_stRegs_t* GPIO = (GPIO_stRegs_t*)GPIO_arrPortBases[Copy_enuPort];
        uint16_t* Loc_p16Reg = (uint16_t*)(&GPIO->BSRR);
        if(Copy_u8State){
            *Loc_p16Reg = (1<<Copy_enuPin);
        }
        else{
            ++Loc_p16Reg;
            *Loc_p16Reg = (1<<Copy_enuPin);
        }
    }
    return Loc_enuStatus;
}

GPIO_enuErrorStatus_t GPIO_enuGetPinState(GPIO_enuPorts_t Copy_enuPort,
GPIO_enuPins_t Copy_enuPin,uint8_t* Add_u8State){

    GPIO_enuErrorStatus_t Loc_enuStatus = GPIO_enu_OK;
    if(Add_u8State==NULL){
        Loc_enuStatus = GPIO_enu_NULL_POINTER;
    }
    else if(Copy_enuPort>GPIO_enu_GPIOH){
        Loc_enuStatus = GPIO_enu_INVALID_PORT;
    }   
    else if(Copy_enuPin>GPIO_enu_PIN15){
        Loc_enuStatus = GPIO_enu_INVALID_PIN;
    }
    else{
        GPIO_stRegs_t* GPIO = (GPIO_stRegs_t*)GPIO_arrPortBases[Copy_enuPort];
        *Add_u8State = (GPIO->IDR & (0x1<<Copy_enuPin)) !=0;         
    }
    return Loc_enuStatus;

}

GPIO_enuErrorStatus_t GPIO_enuSetPortVal(GPIO_enuPorts_t Copy_enuPort,
uint16_t Copy_u16val){

    GPIO_enuErrorStatus_t Loc_enuStatus = GPIO_enu_OK;
    if(Copy_enuPort>GPIO_enu_GPIOH){
        Loc_enuStatus = GPIO_enu_INVALID_PORT;
    }   
    else{
        GPIO_stRegs_t* GPIO = (GPIO_stRegs_t*)GPIO_arrPortBases[Copy_enuPort];
        GPIO->ODR = Copy_u16val;
    }
    return Loc_enuStatus;
}

GPIO_enuErrorStatus_t GPIO_enuGetPortVal(GPIO_enuPorts_t Copy_enuPort,
uint16_t* Add_u16val){

    GPIO_enuErrorStatus_t Loc_enuStatus = GPIO_enu_OK;
    if(Add_u16val==NULL){
        Loc_enuStatus = GPIO_enu_NULL_POINTER;
    }
    else if(Copy_enuPort>GPIO_enu_GPIOH){
        Loc_enuStatus = GPIO_enu_INVALID_PORT;
    }   
    else{
        GPIO_stRegs_t* GPIO = (GPIO_stRegs_t*)GPIO_arrPortBases[Copy_enuPort];
        *Add_u16val = GPIO->IDR;
    }
    return Loc_enuStatus;
}