#ifndef __IMU_H
#define __IMU_H

#include "stm32f4xx_hal.h"

void IMU_ProcessData(void);
float IMU_GetYaw(void);
float DATA_Trans(uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4);
void IMU_UART_RxCpltCallback(UART_HandleTypeDef *huart);

extern volatile uint8_t imu_data_ready;

#endif // __IMU_H

