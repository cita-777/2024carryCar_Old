#ifndef __UI_H__
#define __UI_H__

#include "main.h"
#include "usart.h"
#include <string.h>

void UI_Init(void);
uint8_t UI_Receive_Data(UART_HandleTypeDef *huart);

#endif /* __UI_H */
