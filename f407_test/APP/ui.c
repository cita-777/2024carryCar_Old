#include "ui.h"


uint8_t  uiValue;
uint8_t  uiRx[7]= {0x00};
uint8_t uiRxCNT = 0;

void UI_Init(void)
{

    HAL_UART_Receive_IT(&huart1, &uiValue, 1); // 打开串口接收中断
}

uint8_t UI_Receive_Data(UART_HandleTypeDef *huart)
{
    uiRx[uiRxCNT] = uiValue;
    uiRxCNT++;
    HAL_UART_Receive_IT(&huart5, &uiValue, 1); // 确保在接收到数据后立即调用

    if (uiRxCNT == 7)
    {
        if (uiRx[0] == 0x55 && uiRx[4] == 0xFF&& uiRx[5] == 0xFF&& uiRx[6] == 0xFF)
        {
            //printh 55 00 00 00 ff ff ff
            return 1;

        }

        // 清空数组
        memset(uiRx, 0, sizeof(uiRx));
        uiRxCNT = 0;
    }

    return 0;
}
