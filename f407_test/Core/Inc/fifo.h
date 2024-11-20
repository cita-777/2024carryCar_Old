#ifndef __FIFO_H
#define __FIFO_H

#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"
/**********************************************************
***	Emm_V5.0????????
***	????:ZHANGDATOU
***	????:???????
***	????:https://zhangdatou.taobao.com
***	CSDN??:http s://blog.csdn.net/zhangdatou666
***	qq???:262438510
**********************************************************/

#define 	FIFO_SIZE   128
typedef struct {
	uint16_t buffer[FIFO_SIZE];
	__IO uint8_t ptrWrite;
	__IO uint8_t ptrRead;
}FIFO_t;

extern __IO FIFO_t rxFIFO;

void fifo_initQueue(void);
void fifo_enQueue(uint16_t data);
uint16_t fifo_deQueue(void);
bool fifo_isEmpty(void);
uint16_t fifo_queueLength(void);

#endif
