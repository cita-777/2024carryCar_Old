#include "jetson.h"
#include "usart.h"

uint8_t  value; // main.c要包含AngelaDecode.h，value的定义在这里，.h里面定义了extern
uint8_t  JetsonRx[9]= {0x00};
uint8_t JetsonRxCNT = 0;

uint8_t Send_Data_Jetson[9];


void Jetson_Init(void)
{
    //__HAL_UART_ENABLE_IT(&huart5, UART_IT_RXNE);
    HAL_UART_Receive_IT(&huart5, &value, 1);
    Delay_ms(100);
    // Delay_ms(10);
}

void Jetson_Send(void)
{
    memset(Send_Data_Jetson, 0, sizeof(Send_Data_Jetson)); // 清空数组
    Send_Data_Jetson[0] = 0x00;
    Send_Data_Jetson[1] = 0x0B;

    for (int i = 2; i <= 7; i++)
    {
        Send_Data_Jetson[i] = 0x04;
    }

    Send_Data_Jetson[8] = 0xEF;
    HAL_UART_Transmit(&huart5, Send_Data_Jetson, 9, 1000);
    Delay_ms(20);
}

void Jetson_Receive_Data(UART_HandleTypeDef *huart)
{
    JetsonRx[JetsonRxCNT] = value;
    JetsonRxCNT++;
    HAL_UART_Receive_IT(&huart5, &value, 1); // 确保在接收到数据后立即调用

    if (JetsonRxCNT == 9)
    {
        if (JetsonRx[0] == 0x00 && JetsonRx[8] == 0xEF)
        {
            switch (JetsonRx[1])
            {
                case 0x0A: // 任务A
                    // 处理二维码信息
                    // 示例：二维码信息123+321
                    // JetsonRx[2] 到 JetsonRx[7] 包含二维码信息
                    Jetson_Send();
                    break;

                case 0x0C: // 任务C
                    // 处理微调命令
                    // 示例：小端模式解析得x:+21mm y:-C3 mm
                    // JetsonRx[2] 到 JetsonRx[7] 包含微调命令
                    break;

                case 0x0D: // 任务D
                    // 处理抓取命令
                    // 示例：0x02代表可抓取
                    // JetsonRx[2] 到 JetsonRx[7] 包含抓取命令
                    break;

                default:
                    // 无效命令
                    break;
            }
        }

        // 清空数组
        memset(JetsonRx, 0, sizeof(JetsonRx));
        //HAL_UART_Transmit(&huart5, (uint8_t *)"_RECEIVE_COMPLETE!\r\n", 20, 0xffff);
        JetsonRxCNT = 0;
    }
}

