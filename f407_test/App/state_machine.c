#include "state_machine.h"
#include "motor_ctrl.h"
#include "imu.h"
#include "pi_controller.h"
#include "usart.h"


extern UART_HandleTypeDef huart1;
extern MSG_EULER_ORIEN MEO_Struct;

static State currentState = STATE_IDLE;
static PI_Controller piController;


void StateMachine_SetState(State newState)
{
    currentState = newState;
}

void StateMachine_Init(void)
{
    // ��ʼ��PI������
    //PI_Controller_Init(&piController, 1.0, 0.1); // Kp=1.0, Ki=0.1
	    // ���ó�ʼ״̬
    StateMachine_SetState(STATE_MOVE_LINEAR);
}

void StateMachine_Update(void)
{

    switch (currentState)
    {
        case STATE_IDLE:
            // ����״̬����
				    printf("case IDEL \r\n");
            break;

        case STATE_MOVE_LINEAR:
            // ��ȡYaw������
						printf("Yaw Angle: %f\n", MEO_Struct.Heading);
            // ����״̬����
				    printf("case LINEAR \r\n");
            // ʹ��PI����������ֱ���˶�
            //correction = PI_Controller_Update(&piController, yaw);
						//Motor_Move_Linear();
            // ��������ٶ�����������
            // Motor_Custom_Move(1, 0, 1000 + correction, 100, 20000, 0, 1);
            // Motor_Custom_Move(2, 1, 1000 - correction, 100, 20000, 0, 1);
            // Motor_Custom_Move(3, 0, 1000 + correction, 100, 20000, 0, 1);
            // Motor_Custom_Move(4, 1, 1000 - correction, 100, 20000, 0, 1);

            break;

//        case STATE_ROTATE:
//            // ��ת״̬����
//            Motor_Rotate();
//            break;

        case STATE_STOP:
            // ֹͣ״̬����
            Motor_Stop();
            break;

        case STATE_CUSTOM:
            // �Զ���״̬����
            // ���Ը�����Ҫ����Զ���״̬�����߼�
            break;

        default:
            break;
    }
}
