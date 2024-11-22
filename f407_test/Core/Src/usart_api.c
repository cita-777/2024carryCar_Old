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
    * @brief   UART������ɻص�����
    * @param   huart: UART���
    * @retval  ��
    */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        // �����յ������ݴ��뻺����
        fifo_enQueue((uint8_t)(huart->Instance->DR & 0xFF));

        // ����Ƿ���յ�����������֡
        rxCount = fifo_queueLength();
        if (rxCount >= 8) // ����һ֡���ݳ���Ϊ8
        {
            for (uint8_t i = 0; i < rxCount; i++)
            {
                rxCmd[i] = fifo_deQueue();
            }
            rxFrameFlag = true;
            rxCount = 0; // ���ü�����
        }

        // �������������ж�
        HAL_UART_Receive_IT(&huart1, (uint8_t *)&rxCmd[rxCount], 1);
    }
    else if (huart->Instance == USART2)
    {
        IMU_UART_RxCpltCallback(huart);
    }
}

/**
    * @brief   UART�����жϻص�����
    * @param   huart: UART���
    * @retval  ��
    */
void HAL_UART_IdleCallback(UART_HandleTypeDef *huart)
{

    if (huart->Instance == USART1)
    {
        // �ȶ�SR�ٶ�DR�����IDLE�ж�
        __HAL_UART_CLEAR_IDLEFLAG(huart);

        // ��ȡһ֡��������
        rxCount = fifo_queueLength();
        for (uint8_t i = 0; i < rxCount; i++)
        {
            rxCmd[i] = fifo_deQueue();
        }

        // һ֡���ݽ�����ɣ���λ֡��־λ
        rxFrameFlag = true;
    }
}

/**
    * @brief   USART����һ���ֽ�
    * @param   data Ҫ���͵�����
    * @retval  ��
    */
void usart_SendByte(uint16_t data)
{
    HAL_UART_Transmit(&huart1, (uint8_t*)&data, 1, HAL_MAX_DELAY);
}

/**
    * @brief   USART���Ͷ���ֽ�
    * @param   cmd Ҫ���͵�����ָ��
    * @param   len ���ݳ���
    * @retval  ��
    */
void usart_SendCmd(__IO uint8_t *cmd, uint8_t len)
{
    HAL_UART_Transmit(&huart1, (const uint8_t*)cmd, len, HAL_MAX_DELAY);
}

