#include "imu.h"
#include "usart.h"
#include <string.h>

#define FRAME_HEAD 0xAA
#define FRAME_END 0x55
#define TYPE_IMU 0x01
#define TYPE_AHRS 0x02
#define IMU_LEN 64
#define AHRS_LEN 56

uint8_t Fd_data[80];
uint8_t Fd_rsahrs[80];
uint8_t Usart_Receive[80]; // 将Usart_Receive声明为非易失性（non-volatile）
uint8_t Count = 0;
uint8_t rs_count = 0;
uint8_t rsacc_flag = 0;
uint8_t rs_ahrstype = 0;

float yaw_angle = 0.0f;
volatile uint8_t imu_data_ready = 0;

void IMU_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    for (int i = 0; i < 80; i++)
    {
        Fd_data[i] = Usart_Receive[i];
    }

    for (int i = 0; i < 80; i++)
    {
        if (((rs_count == 0) && (Fd_data[i] == FRAME_HEAD)) || (Count > 0))
        {
            rs_count = 1;
            Count++;
            if ((Fd_data[1] == TYPE_AHRS) && (Fd_data[2] == AHRS_LEN))
                rsacc_flag = 1;
        }

        if (rsacc_flag == 1 && Count == AHRS_LEN)
        {
            Count = 0;
            rsacc_flag = 0;
            rs_ahrstype = 1;
            if (Fd_data[AHRS_LEN - 1] == FRAME_END)
                memcpy(Fd_rsahrs, Fd_data, AHRS_LEN);
        }
    }

    // 设置标志位，表示IMU数据已准备好
    imu_data_ready = 1;

    // 重新启动DMA接收
    HAL_UART_Receive_DMA(&huart2, Usart_Receive, 80);
}

void IMU_ProcessData(void)
{
    if (rs_ahrstype == 1)
    {
        if (Fd_rsahrs[1] == TYPE_AHRS && Fd_rsahrs[2] == AHRS_LEN)
        {
            yaw_angle = DATA_Trans(Fd_rsahrs[27], Fd_rsahrs[28], Fd_rsahrs[29], Fd_rsahrs[30]);
            rs_ahrstype = 0;
        }
    }
}

float IMU_GetYaw(void)
{
    return yaw_angle;
}

float DATA_Trans(uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4)
{
    uint32_t data = ((uint32_t)byte1 << 24) | ((uint32_t)byte2 << 16) | ((uint32_t)byte3 << 8) | byte4;
    float result;
    memcpy(&result, &data, sizeof(result));
    return result;
}
