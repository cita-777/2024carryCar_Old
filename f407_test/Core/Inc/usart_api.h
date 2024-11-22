#ifndef __USART_API_H
#define __USART_API_H

#include "stm32f4xx_hal.h"
#include "fifo.h"
#include <stdbool.h>

#define FRAME_HEAD 0xAA
#define FRAME_END 0x55
#define TYPE_IMU 0x01
#define TYPE_AHRS 0x02
#define IMU_LEN 64
#define AHRS_LEN 56

extern __IO bool rxFrameFlag;
extern __IO uint8_t rxCmd[FIFO_SIZE];
extern __IO uint8_t rxCount;

extern __IO uint8_t Fd_data[80];
extern __IO uint8_t Fd_rsahrs[80];
extern __IO uint8_t Usart_Receive[80];
extern __IO uint8_t Count ;
extern __IO uint8_t rs_count;
extern __IO uint8_t rsacc_flag;
extern __IO uint8_t rs_ahrstype;

extern __IO float yaw_angle;
 
void usart_SendByte(uint16_t data);
void usart_SendCmd(__IO uint8_t *cmd, uint8_t len);

#endif // __USART_API_H
