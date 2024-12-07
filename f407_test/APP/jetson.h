#ifndef __IMU_H__
#define __IMU_H__

#include "main.h"
#include "Delay.h"
#include <string.h>

extern uint8_t value;   // main.c要包含AngelaDecode.h，value的定义在这里，.h里面定义了extern
extern uint8_t JetsonRx[9];
extern uint8_t JetsonRxCNT;
void Jetson_Init(void);
void Jetson_Send(void);
void Jetson_Receive_Data(UART_HandleTypeDef *huart);

#endif // __IMU_H
