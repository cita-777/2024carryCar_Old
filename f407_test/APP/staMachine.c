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

            // ����״̬����
            if (ready_flag == 1)
            {
                StateMachine_SetState(STATE_MOVE_LINEAR);
                ready_flag = 0;
            }

            printf("case IDEL \r\n");
            break;

        case STATE_MOVE_LINEAR:
            // ֱ���˶�״̬����
            //y��Ϊǰ������x��Ϊ���֣����죩����
            // 3200����Ϊ1�ܣ�����Ϊ408.407044966673121mm
            // 1mm = 7.835320275293308837����
            //�ȶ���75mm����ͣ����
            Counter_Times=500;
            Counter_Enable=1;
            //0.01s����һ���ж�,Counter++
            Car_Go_Target(-75, 75, 200, 150);

            if (Counter_Enable==0)
            {
                Counter_Times=1450;
                Counter_Enable=1;
                Car_Go_Target(0, 1450, 200, 150);
            }


            break;

        //        case STATE_ROTATE:
        //            // ��ת״̬����
        //            Motor_Rotate();
        //            break;

        //        case STATE_STOP:
        //            // ֹͣ״̬����
        //            Motor_Stop();
        //            break;

        case STATE_CUSTOM:
            // �Զ���״̬����
            // ���Ը�����Ҫ����Զ���״̬�����߼�
            break;

        default:
            break;
    }
}
