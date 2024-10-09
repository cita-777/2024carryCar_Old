#ifndef _SOFT_USER_H_
#define _SOFT_USER_H_

#include "main.h"
#include <stdio.h>
#include "ring_buffer.h"

#define USART_RECV_BUF_SIZE 2048
#define USART_SEND_BUF_SIZE 2048

typedef struct
{  
    UART_HandleTypeDef *pUSARTx;
    // ���Ͷ˻�����
    RingBufferTypeDef *sendBuf;
		// ���ն˻�����
    RingBufferTypeDef *recvBuf;
} Usart_DataTypeDef;



void delay(uint32_t time);
void delay_us(uint32_t time);
void Usart_SendString(uint8_t *str);
void clearArray(uint8_t * array,uint16_t length);




// ��ʱ
// ��ʱ us
void SysTick_DelayUs(__IO uint32_t nTime);
// ��ʱ ms
void SysTick_DelayMs(__IO uint32_t nTime);
// ��ʱ s
void SysTick_DelayS(__IO uint32_t nTime);

// ����ʱ
// ���õ���ʱ(������ʽ)
void SysTick_CountdownBegin(__IO uint32_t nTime);
// ��������ʱ
void SysTick_CountdownCancel(void);
// �жϵ���ʱ�Ƿ�ʱ
uint8_t SysTick_CountdownIsTimeout(void);
void Usart_SendAll(Usart_DataTypeDef *usart);
void usart_SendCmd(__IO uint8_t *cmd, uint8_t len);
#endif // !_SOFT_USER_H_
