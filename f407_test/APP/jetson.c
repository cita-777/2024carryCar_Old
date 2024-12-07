#include "jetson.h"
#include "usart.h"

uint8_t  value; // main.cҪ����AngelaDecode.h��value�Ķ��������.h���涨����extern
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
    memset(Send_Data_Jetson, 0, sizeof(Send_Data_Jetson)); // �������
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
    HAL_UART_Receive_IT(&huart5, &value, 1); // ȷ���ڽ��յ����ݺ���������

    if (JetsonRxCNT == 9)
    {
        if (JetsonRx[0] == 0x00 && JetsonRx[8] == 0xEF)
        {
            switch (JetsonRx[1])
            {
                case 0x0A: // ����A
                    // �����ά����Ϣ
                    // ʾ������ά����Ϣ123+321
                    // JetsonRx[2] �� JetsonRx[7] ������ά����Ϣ
                    Jetson_Send();
                    break;

                case 0x0C: // ����C
                    // ����΢������
                    // ʾ����С��ģʽ������x:+21mm y:-C3 mm
                    // JetsonRx[2] �� JetsonRx[7] ����΢������
                    break;

                case 0x0D: // ����D
                    // ����ץȡ����
                    // ʾ����0x02�����ץȡ
                    // JetsonRx[2] �� JetsonRx[7] ����ץȡ����
                    break;

                default:
                    // ��Ч����
                    break;
            }
        }

        // �������
        memset(JetsonRx, 0, sizeof(JetsonRx));
        //HAL_UART_Transmit(&huart5, (uint8_t *)"_RECEIVE_COMPLETE!\r\n", 20, 0xffff);
        JetsonRxCNT = 0;
    }
}

