#ifndef SERVO_H_
#define SERVO_H_
#include "servo_cfg.h"

typedef enum{
    SERVO_enu_OK,
    SERVO_enu_INVALID_SERVO,
    SERVO_enu_INVALID_ANGLE,
    SERVO_enu_NOK
}SERVO_enuErrorStatus_t;


void SERVO_vInit(void);

SERVO_enuErrorStatus_t SERVO_enuMoveToAngle(uint8_t Copy_u8Servo,
uint8_t Copy_u8Angle);

#endif