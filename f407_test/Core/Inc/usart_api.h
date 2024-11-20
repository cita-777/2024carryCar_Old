#ifndef __USART_API_H
#define __USART_API_H

#include "stm32f4xx_hal.h"
#include <stdbool.h>

void usart_SendByte(uint16_t data);
void usart_SendCmd(__IO uint8_t *cmd, uint8_t len);

#endif // __USART_API_H
