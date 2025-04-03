#ifndef SERVO_CFG_H_
#define SERVO_CFG_H_
#include "std_types.h"
#include "rcc.h"
#include "GPIO.h"
#include "time2_5.h"

#define SERVO_1                     0
#define SERVO_2                     1

#define SERVO_NO_OF_SERVOS          2

typedef struct{
    GPIO_enuPorts_t port;
    GPIO_enuPins_t pin; 
    GPIO_enuAltFn_t AltFnNo;
    TIM2_5_enuTimers_t Timer;
    TIM2_5_enuChannels_t TimerChannel;  
    uint32_t operatingFreq;
    float32_t minPWM;
    float32_t maxPWM;
    uint8_t maxAngle;
}SERVO_stCfg_t;

extern SERVO_stCfg_t SERVO_arrCfgs[SERVO_NO_OF_SERVOS];

#endif