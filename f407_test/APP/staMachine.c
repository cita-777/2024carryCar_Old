#include "staMachine.h"
#include "motor_ctrl.h"
#include "imu.h"
#include "usart.h"
#include "Delay.h"
#include "usr_tim.h"
#include "servo.h"

extern MSG_EULER_ORIEN MEO_Struct;

uint8_t ready_flag = 0;
uint8_t running_flag = 0;
uint8_t is_turning = 0;
static State currentState = STATE_IDLE;

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
        if (ready_flag == 1)
        {
            Delay_ms(2000);
            StateMachine_SetState(MOVE_TO_RAW);
            ready_flag = 0;
        }

        // printf("t5.txt=\"%d\"\xff\xff\xff", currentState); // ����״̬��Ϣ
        break;

    case MOVE_TO_RAW:
        if (Counter_Enable == 0 || is_turning == 1)
        {
            switch (running_flag)
            {
            case 0: // ����
                Counter_Times = 200;
                Counter_Enable = 1;
                printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                Car_Go_Target(-115, 0, 200, 200);
                printf("t5.txt=\"%d\"\xff\xff\xff", currentState);
                running_flag++;
                break;

            case 1: // y����ֱ��1450mm
                Counter_Times = 500;
                Counter_Enable = 1;
                printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                Car_Go_Target(0, 1450, 300, 150);
                printf("t5.txt=\"%d\"\xff\xff\xff", currentState);
                running_flag++;
                break;

            case 2: // x�᷽����50mm��������
                Counter_Times = 200;
                Counter_Enable = 1;
                printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                Car_Go_Target(50, 0, 200, 150);
                printf("t5.txt=\"%d\"\xff\xff\xff", currentState);
                // �������ж���ɣ�������һ��״̬
                running_flag = 0;
                StateMachine_SetState(MOVE_TO_ROUGH);
                printf("t5.txt=\"%d\"\xff\xff\xff", currentState);
                break;
            }
        }

        break;

    case MOVE_TO_ROUGH:
        if (Counter_Enable == 0 || is_turning == 1)
        {
            switch (running_flag)
            {
            case 0: // x�����50mm
                Counter_Times = 200;
                Counter_Enable = 1;
                printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                Car_Go_Target(-50, 0, 200, 150);
                printf("t5.txt=\"%d\"\xff\xff\xff", currentState);
                running_flag++;
                break;

            case 1: // y�������400mm
                Counter_Times = 300;
                Counter_Enable = 1;
                printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                Car_Go_Target(0, -400, 200, 150);
                printf("t5.txt=\"%d\"\xff\xff\xff", currentState);
                running_flag++;
                break;

            case 2: // ԭ����ת����90�ȵ������췽��x�᷽��
                Counter_Times = 1200;
                Counter_Enable = 1;
                is_turning = 1;
                printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                Car_Turn(90, 200, 250);
                printf("t5.txt=\"%d\"\xff\xff\xff", currentState);
                if (Counter_Enable == 0)
                {
                    is_turning = 0;
                    running_flag++;
                }
                break;

            case 3: // y�᷽����-1720mm
                Counter_Times = 860;
                Counter_Enable = 1;
                printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                Car_Go_Target(0, -1720, 250, 150);
                printf("t5.txt=\"%d\"\xff\xff\xff", currentState);
                running_flag++;
                break;

            case 4: // ԭ����ת����90�ȵ������췽��x�᷽��
                Counter_Times = 1200;
                Counter_Enable = 1;
                is_turning = 1;
                printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                Car_Turn(90, 200, 250);
                printf("t5.txt=\"%d\"\xff\xff\xff", currentState);
                if (Counter_Enable == 0)
                {
                    is_turning = 0;
                    running_flag = 0;
                    StateMachine_SetState(MOVE_TO_TEMPSTORE);
                    printf("t5.txt=\"%d\"\xff\xff\xff", currentState);
                }
                break;
            }
        }
        break;
    case MOVE_TO_TEMPSTORE:
        if (Counter_Enable == 0 || is_turning == 1)
        {
            switch (running_flag)
            {
            case 0: // y�������825mm
                Counter_Times = 500;
                Counter_Enable = 1;
                printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                Car_Go_Target(0, -825, 250, 150);
                printf("t5.txt=\"%d\"\xff\xff\xff", currentState);

                running_flag++;
                break;

            case 1: // ԭ��������ת90�ȵ������췽��x�᷽��
                Counter_Times = 700;
                Counter_Enable = 1;
                is_turning = 1;
                printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                Car_Turn(90, 70, 250);
                printf("t5.txt=\"%d\"\xff\xff\xff", currentState);
                if (Counter_Enable == 0)
                {
                    is_turning = 0;
                    running_flag++;
                }
                break;

            case 2: // y�᷽����-860mm
                Counter_Times = 500;
                Counter_Enable = 1;
                printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                Car_Go_Target(0, -860, 250, 150);
                printf("t5.txt=\"%d\"\xff\xff\xff", currentState);
                // �������ж���ɣ�������һ��״̬
                running_flag = 0;
                StateMachine_SetState(MOVE_TO_RAW_AGAIN);
                printf("t5.txt=\"%d\"\xff\xff\xff", currentState);
                break;
            }
        }

        break;

    case MOVE_TO_RAW_AGAIN:
        if (Counter_Enable == 0 || is_turning == 1)
        {
            switch (running_flag)
            {
            case 0: // y�������860mm
                Counter_Times = 500;
                Counter_Enable = 1;
                printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                Car_Go_Target(0, -860, 250, 150);
                printf("t5.txt=\"%d\"\xff\xff\xff", currentState);

                running_flag++;
                break;

            case 1: // ԭ��������ת90�ȵ������췽��x�᷽��
                Counter_Times = 700;
                Counter_Enable = 1;
                is_turning = 1;
                printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                Car_Turn(90, 100, 250);
                printf("t5.txt=\"%d\"\xff\xff\xff", currentState);
                if (Counter_Enable == 0)
                {
                    is_turning = 0;
                    running_flag++;
                }
                break;

            case 2: // y�᷽����-425mm
                Counter_Times = 400;
                Counter_Enable = 1;
                printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                Car_Go_Target(0, -425, 250, 150);
                printf("t5.txt=\"%d\"\xff\xff\xff", currentState);
                // �������ж���ɣ�������һ��״̬
                running_flag = 0;
                StateMachine_SetState(STATE_IDLE);
                printf("t5.txt=\"%d\"\xff\xff\xff", currentState);
                break;
            }
        }

        break;

    case STATE_CUSTOM:
        // �Զ���״̬�߼�
        break;

    default:
        break;
    }
}
