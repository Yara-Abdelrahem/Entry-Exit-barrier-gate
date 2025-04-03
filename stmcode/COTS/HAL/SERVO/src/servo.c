#include "servo.h"

#define SERVO_NO_OF_TIMERS          4

uint64_t SERVO_arrTIMClkMasks[SERVO_NO_OF_TIMERS] = {
    RCC_TIM2,
    RCC_TIM3,
    RCC_TIM4,
    RCC_TIM5
};

void SERVO_vInit(void){
    uint8_t i = 0;
    uint32_t Loc_u32ClkFreq = 0;
    uint64_t Loc_u64GPIOClkMask = 0;
    GPIO_stPinCFG_t Loc_stGpioCfg = {0};
    TIM2_5_stPWMCfg_t Loc_stPwmCfg = {0};

    RCC_getBusFreq(RCC_enu_AHB,&Loc_u32ClkFreq);

    for(;i<SERVO_NO_OF_SERVOS;i++){
        
        if(SERVO_arrCfgs[i].port < GPIO_enu_GPIOH){
            Loc_u64GPIOClkMask |= (1 << SERVO_arrCfgs[i].port);
        }
        else{
            Loc_u64GPIOClkMask |= RCC_GPIOH;
        }

        
        RCC_enuCtlPeripheral(
            SERVO_arrTIMClkMasks[SERVO_arrCfgs[i].Timer],
            RCC_PER_ON
        );
    }

    RCC_enuCtlPeripheral(Loc_u64GPIOClkMask,RCC_PER_ON);

    for(i=0;i<SERVO_NO_OF_SERVOS;i++){
        Loc_stGpioCfg.port = SERVO_arrCfgs[i].port;
        Loc_stGpioCfg.pin = SERVO_arrCfgs[i].pin;
        Loc_stGpioCfg.mode = GPIO_enu_ALT_FN;
        Loc_stGpioCfg.AltFn = SERVO_arrCfgs[i].AltFnNo;
        Loc_stGpioCfg.speed = GPIO_enu_high_speed;
        Loc_stGpioCfg.OutputType = GPIO_enu_PUSH_PULL;
        Loc_stGpioCfg.InputType = GPIO_enu_NO_PU_PD;
        
        GPIO_enuCfgPin(&Loc_stGpioCfg);

        Loc_stPwmCfg.Timer = SERVO_arrCfgs[i].Timer;
        Loc_stPwmCfg.Channel = SERVO_arrCfgs[i].TimerChannel;
        Loc_stPwmCfg.Frequency = SERVO_arrCfgs[i].operatingFreq;
        Loc_stPwmCfg.DutyCycle = SERVO_arrCfgs[i].minPWM;
        Loc_stPwmCfg.SystemClkFreq = Loc_u32ClkFreq;

        TIM2_5_enuCfgPWM(&Loc_stPwmCfg);
       }
}

SERVO_enuErrorStatus_t SERVO_enuMoveToAngle(uint8_t Copy_u8Servo,
uint8_t Copy_u8Angle){
    
    SERVO_enuErrorStatus_t Loc_enuStatus = SERVO_enu_OK;

    float32_t Loc_f32PWM = 0.0;
    float32_t Loc_f32Slope = 0.0;
   
    if(Copy_u8Servo >= SERVO_NO_OF_SERVOS){
        Loc_enuStatus = SERVO_enu_INVALID_SERVO;
    }
    else if(Copy_u8Angle > SERVO_arrCfgs[Copy_u8Servo].maxAngle){
        Loc_enuStatus = SERVO_enu_INVALID_ANGLE;
    }
    else{
        Loc_f32Slope = (SERVO_arrCfgs[Copy_u8Servo].maxPWM - 
        SERVO_arrCfgs[Copy_u8Servo].minPWM)/
        (SERVO_arrCfgs[Copy_u8Servo].maxAngle);

        Loc_f32PWM = Copy_u8Angle*Loc_f32Slope + 
        SERVO_arrCfgs[Copy_u8Servo].minPWM;
        TIM2_5_enuSetDutyCycle(
            SERVO_arrCfgs[Copy_u8Servo].Timer,
            SERVO_arrCfgs[Copy_u8Servo].TimerChannel,
            Loc_f32PWM
        );
    }

    return Loc_enuStatus;
}