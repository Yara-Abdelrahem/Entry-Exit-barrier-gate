#ifndef ULTRASONIC_CFG_H_
#define ULTRASONIC_CFG_H_
#include "rcc.h"
#include "time2_5.h"
#include "GPIO.h"
#include "nvic.h"
#include "scheduler.h"

#define NO_OF_ULTRASONICS       1

#define ULTRASONIC_1            0

typedef struct{
    GPIO_enuPorts_t Port;
    GPIO_enuPins_t Pin;
    GPIO_enuAltFn_t altFun;
    TIM2_5_enuTimers_t Timer;
    TIM2_5_enuChannels_t Channel;
}Ultrasonic_stPins_t;

typedef struct{
    Ultrasonic_stPins_t TriggerPin;
    Ultrasonic_stPins_t EchoPin;
    uint32_t pulseWidth;
}Ultrasonic_stCfg_t;

extern Ultrasonic_stCfg_t Ultrasonic_arrCfgs[NO_OF_ULTRASONICS];

#endif