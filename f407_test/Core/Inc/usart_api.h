#ifndef __USART_API_H
#define __USART_API_H

#include "stm32f4xx_hal.h"
#include "fifo.h"
#include <stdbool.h>

extern __IO bool rxFrameFlag;
extern __IO uint8_t rxCmd[FIFO_SIZE];
extern __IO uint8_t rxCount;

void usart_SendByte(uint16_t data);
void usart_SendCmd(__IO uint8_t *cmd, uint8_t len);

#endif // __USART_API_H
