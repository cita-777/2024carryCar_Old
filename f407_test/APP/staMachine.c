#include "staMachine.h"
#include "motor_ctrl.h"
#include "imu.h"
#include "usart.h"
#include "Delay.h"
#include "usr_tim.h"

extern UART_HandleTypeDef huart1;
extern MSG_EULER_ORIEN MEO_Struct;

uint8_t ready_flag = 0;
static State currentState = STATE_IDLE;
//static PI_Controller piController;


void StateMachine_SetState(State newState)
{
    currentState = newState;
}

void StateMachine_Init(void)
{
    StateMachine_SetState(STATE_IDLE);
}

void StateMachine_Update(void)
{

    switch (currentState)
    {
        case STATE_IDLE:

            // 空闲状态处理
            if (ready_flag == 1)
            {
                StateMachine_SetState(STATE_MOVE_LINEAR);
                ready_flag = 0;
            }

            printf("case IDEL \r\n");
            break;

        case STATE_MOVE_LINEAR:
            // 直线运动状态处理
            //y轴为前进方向，x轴为右手（滑轨）方向
            // 3200脉冲为1周，理论为408.407044966673121mm
            // 1mm = 7.835320275293308837脉冲
            //先都走75mm出启停区，
            Counter_Times=500;
            Counter_Enable=1;
            //0.01s触发一次中断,Counter++
            Car_Go_Target(-75, 75, 200, 150);

            if (Counter_Enable==0)
            {
                Counter_Times=1450;
                Counter_Enable=1;
                Car_Go_Target(0, 1450, 200, 150);
            }


            break;

        //        case STATE_ROTATE:
        //            // 旋转状态处理
        //            Motor_Rotate();
        //            break;

        //        case STATE_STOP:
        //            // 停止状态处理
        //            Motor_Stop();
        //            break;

        case STATE_CUSTOM:
            // 自定义状态处理
            // 可以根据需要添加自定义状态处理逻辑
            break;

        default:
            break;
    }
}
