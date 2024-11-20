#include "usart_api.h"
#include "fifo.h"

extern UART_HandleTypeDef huart1;

__IO bool rxFrameFlag = false;
__IO uint8_t rxCmd[FIFO_SIZE] = {0};
__IO uint8_t rxCount = 0;

/**
    * @brief   USART3??????
    * @param   ?
    * @retval  ?
    */
void USART3_IRQHandler(void)
{
    __IO uint16_t i = 0;

    HAL_UART_IRQHandler(&huart1);

/**********************************************************
***	??????
**********************************************************/
    if(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE))
    {
        // ?????????,????????
        fifo_enQueue((uint8_t)(huart1.Instance->DR & 0xFF));

        // ????????
        __HAL_UART_CLEAR_FLAG(&huart1, UART_FLAG_RXNE);
    }

/**********************************************************
***	??????
**********************************************************/
    else if(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE))
    {
        // ??SR??DR,??IDLE??
        __HAL_UART_CLEAR_IDLEFLAG(&huart1);

        // ????????
        rxCount = fifo_queueLength(); 
        for(i=0; i < rxCount; i++) 
        { 
            rxCmd[i] = fifo_deQueue(); 
        }

        // ????????,??????
        rxFrameFlag = true;
    }
}

/**
    * @brief   USART??????
    * @param   data ??????
    * @retval  ?
    */
void usart_SendByte(uint16_t data)
{
    HAL_UART_Transmit(&huart1, (uint8_t*)&data, 1, HAL_MAX_DELAY);
}

/**
    * @brief   USART??????
    * @param   cmd ????????
    * @param   len ????
    * @retval  ?
    */
void usart_SendCmd(__IO uint8_t *cmd, uint8_t len)
{
    HAL_UART_Transmit(&huart1, (const uint8_t*)cmd, len, HAL_MAX_DELAY);
}
