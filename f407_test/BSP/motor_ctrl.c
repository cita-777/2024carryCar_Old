#include "motor_ctrl.h"
#include "usart_api.h"
#include "Emm_V5.h"
#include "fifo.h"
#include <stdio.h>

/**
 * @brief  初始化电机控制
 * @param  无
 * @retval 无
 */
void Motor_Init(void)
{
    // 上电延时2秒等待Emm_V5.0闭环初始化完毕
    HAL_Delay(1000);

    // 将当前四个电机位置清零
    Emm_V5_Reset_CurPos_To_Zero(1);
    HAL_Delay(20);
    Emm_V5_Reset_CurPos_To_Zero(2);
    HAL_Delay(20);
    Emm_V5_Reset_CurPos_To_Zero(3);
    HAL_Delay(20);
    Emm_V5_Reset_CurPos_To_Zero(4);
    HAL_Delay(20);
}

/**
 * @brief  触发回零
 * @param  无
 * @retval 无
 */
void Motor_Home(void)
{
    Emm_V5_Synchronous_motion(0);
    HAL_Delay(100);
}

/**
 * @brief  直线运动
 * @param  无
 * @retval 无
 */
void Motor_Move_Linear(void)
{
    HAL_Delay(20);
    Emm_V5_Pos_Control(1, 1, 230, 165, 30000, 0, 1);
    HAL_Delay(20);
    Emm_V5_Pos_Control(2, 1, 201, 150, 30000, 0, 1);
    HAL_Delay(20);
    Emm_V5_Pos_Control(3, 0, 201, 150, 30000, 0, 1);
    HAL_Delay(20);
    Emm_V5_Pos_Control(4, 0, 230, 165, 30000, 0, 1);
    HAL_Delay(20);
    Emm_V5_Synchronous_motion(0);
}

/**
 * @brief  向左旋转运动
 * @param  无
 * @retval 无
 */
void Motor_turnLeft_rotate(void)
{
    Emm_V5_Pos_Control(1, 1, 999, 150, 100000, 0, 1);
    HAL_Delay(20);
    Emm_V5_Pos_Control(2, 1, 999, 150, 100000, 0, 1);
    HAL_Delay(20);
    Emm_V5_Pos_Control(3, 1, 999, 150, 100000, 0, 1);
    HAL_Delay(20);
    Emm_V5_Pos_Control(4, 1, 999, 150, 100000, 0, 1);
    HAL_Delay(20);
    Emm_V5_Synchronous_motion(0);
}

/**
 * @brief  停止运动
 * @param  无
 * @retval 无
 */
void Motor_Stop(void)
{
    Emm_V5_Stop_Now(1, 1);
    HAL_Delay(5);
    Emm_V5_Stop_Now(2, 1);
    HAL_Delay(5);
    Emm_V5_Stop_Now(3, 1);
    HAL_Delay(5);
    Emm_V5_Stop_Now(4, 1);
    HAL_Delay(5);
}

/**
 * @brief  自定义运动
 * @param  motor_id: 电机ID
 * @param  direction: 方向
 * @param  speed: 速度
 * @param  acceleration: 加速度
 * @param  distance: 距离
 * @param  mode: 模式
 * @param  sync: 同步标志
 * @retval 无
 */
void Motor_Custom_Move(uint8_t motor_id, uint8_t direction, uint16_t speed, uint16_t acceleration, uint32_t distance, uint8_t mode, uint8_t sync)
{
    Emm_V5_Pos_Control(motor_id, direction, speed, acceleration, distance, mode, sync);
    HAL_Delay(20);
    if (sync)
    {
        Emm_V5_Synchronous_motion(0);
    }
}
