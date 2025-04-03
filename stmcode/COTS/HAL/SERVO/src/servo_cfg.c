#include "servo_cfg.h"

SERVO_stCfg_t SERVO_arrCfgs[SERVO_NO_OF_SERVOS] = {
    [SERVO_1] = {
        .port = GPIO_enu_GPIOA,
        .pin = GPIO_enu_PIN0,
        .AltFnNo = GPIO_enu_AltFn1,
        .Timer = TIM2_5_enuTIM2,
        .TimerChannel = TIM2_5_enuCH1,
        .operatingFreq = 60,
        .minPWM = 5,
        .maxPWM = 12.5,
        .maxAngle = 135
    }
};