#include "state_machine.h"
#include "motor_ctrl.h"
#include "imu.h"
#include "pi_controller.h"
#include "usart.h"
#include <stdio.h>

extern UART_HandleTypeDef huart1;

static State currentState = STATE_IDLE;
static PI_Controller piController;


void StateMachine_SetState(State newState)
{
    currentState = newState;
}

void StateMachine_Init(void)
{
    // 初始化PI控制器
    PI_Controller_Init(&piController, 1.0, 0.1); // Kp=1.0, Ki=0.1
	    // 设置初始状态
    StateMachine_SetState(STATE_MOVE_LINEAR);
}

void StateMachine_Update(void)
{
    float yaw;
    float correction;

    switch (currentState)
    {
        case STATE_IDLE:
            // 空闲状态处理
            break;

        case STATE_MOVE_LINEAR:
            // 获取Yaw轴数据
            yaw = IMU_GetYaw();

            // 使用PI控制器修正直线运动
            correction = PI_Controller_Update(&piController, yaw);

            // 调整电机速度以修正方向
            // Motor_Custom_Move(1, 0, 1000 + correction, 100, 20000, 0, 1);
            // Motor_Custom_Move(2, 1, 1000 - correction, 100, 20000, 0, 1);
            // Motor_Custom_Move(3, 0, 1000 + correction, 100, 20000, 0, 1);
            // Motor_Custom_Move(4, 1, 1000 - correction, 100, 20000, 0, 1);

            // 通过printf格式化输出Yaw轴数据

            printf("Yaw: %.2f\r\n", yaw);
            break;

//        case STATE_ROTATE:
//            // 旋转状态处理
//            Motor_Rotate();
//            break;

        case STATE_STOP:
            // 停止状态处理
            Motor_Stop();
            break;

        case STATE_CUSTOM:
            // 自定义状态处理
            // 可以根据需要添加自定义状态处理逻辑
            break;

        default:
            break;
    }
}
