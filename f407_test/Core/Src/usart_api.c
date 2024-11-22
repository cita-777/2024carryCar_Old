#include "usart_api.h"
#include "fifo.h"
#include <stdio.h>
#include "imu.h"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

__IO bool rxFrameFlag = false;
__IO uint8_t rxCmd[FIFO_SIZE] = {0};
__IO uint8_t rxCount = 0;

/**
    * @brief   UART接收完成回调函数
    * @param   huart: UART句柄
    * @retval  无
    */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        // 将接收到的数据存入缓冲区
        fifo_enQueue((uint8_t)(huart->Instance->DR & 0xFF));

        // 检查是否接收到完整的数据帧
        rxCount = fifo_queueLength();
        if (rxCount >= 8) // 假设一帧数据长度为8
        {
            for (uint8_t i = 0; i < rxCount; i++)
            {
                rxCmd[i] = fifo_deQueue();
            }
            rxFrameFlag = true;
            rxCount = 0; // 重置计数器
        }

        // 重新启动接收中断
        HAL_UART_Receive_IT(&huart1, (uint8_t *)&rxCmd[rxCount], 1);
    }
    else if (huart->Instance == USART2)
    {
        IMU_UART_RxCpltCallback(huart);
    }
}

/**
    * @brief   UART空闲中断回调函数
    * @param   huart: UART句柄
    * @retval  无
    */
void HAL_UART_IdleCallback(UART_HandleTypeDef *huart)
{

    if (huart->Instance == USART1)
    {
        // 先读SR再读DR，清除IDLE中断
        __HAL_UART_CLEAR_IDLEFLAG(huart);

        // 提取一帧数据命令
        rxCount = fifo_queueLength();
        for (uint8_t i = 0; i < rxCount; i++)
        {
            rxCmd[i] = fifo_deQueue();
        }

        // 一帧数据接收完成，置位帧标志位
        rxFrameFlag = true;
    }
}

/**
    * @brief   USART发送一个字节
    * @param   data 要发送的数据
    * @retval  无
    */
void usart_SendByte(uint16_t data)
{
    HAL_UART_Transmit(&huart1, (uint8_t*)&data, 1, HAL_MAX_DELAY);
}

/**
    * @brief   USART发送多个字节
    * @param   cmd 要发送的数据指针
    * @param   len 数据长度
    * @retval  无
    */
void usart_SendCmd(__IO uint8_t *cmd, uint8_t len)
{
    HAL_UART_Transmit(&huart1, (const uint8_t*)cmd, len, HAL_MAX_DELAY);
}

