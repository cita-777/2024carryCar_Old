#ifndef __MOTOR_CTRL_H
#define __MOTOR_CTRL_H

#include "stm32f4xx_hal.h"

void Motor_Init(void);
void Motor_Home(void);
void Motor_Move_Linear(void);
void Motor_turnLeft_rotate(void);
void Motor_Stop(void);
void Motor_Custom_Move(uint8_t motor_id, uint8_t direction, uint16_t speed, uint16_t acceleration, uint32_t distance, uint8_t mode, uint8_t sync);

#endif // __MOTOR_CTRL_H

