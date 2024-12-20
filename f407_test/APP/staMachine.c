#include "staMachine.h"
#include "motor_ctrl.h"
#include "imu.h"
#include "usart.h"
#include "Delay.h"
#include "usr_tim.h"
#include "servo.h"

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
        Car_Go_Target(0, 0, 0, 0);
        printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
        printf("t5.txt=\"%d\"\xff\xff\xff", currentState);
        if (ready_flag == 1)
        {
            Delay_ms(2000);
            StateMachine_SetState(MOVE_TO_RAW);
            ready_flag = 0;
        }
        // printf("%f\r\n",YawAngle);
        // printf("t5.txt=\"%d\"\xff\xff\xff", currentState); // ����״̬��Ϣ
        break;

    case MOVE_TO_RAW:
        if (Counter_Enable == 0 || is_turning == 1)
        {
            switch (running_flag)
            {
            case 0: // ����
                printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                printf("t5.txt=\"%d\"\xff\xff\xff", currentState);
                Counter_Times = 200;
                Counter_Enable = 1;
                Car_Go_Target(-150, 0, 200, 150);
                running_flag++;
                break;

            case 1: // y����ֱ��1450mm
                printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                Counter_Times = 400;
                Counter_Enable = 1;
                Car_Go_Target(0, 1450, 300, 150);
                running_flag++;
                break;

            case 2: // x�᷽����50mm��������
                printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                Counter_Times = 80;
                Counter_Enable = 1;
                Car_Go_Target(50, 0, 200, 150);
                running_flag++;
                break;
            case 3: // �������ж���ɣ�������һ��״̬
                running_flag = 0;
                StateMachine_SetState(MOVE_TO_ROUGH);
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
                printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                Counter_Times = 80;
                Counter_Enable = 1;
                Car_Go_Target(-50, 0, 200, 150);

                running_flag++;
                break;

            case 1: // y�������400mm
                printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                Counter_Times = 200;
                Counter_Enable = 1;
                Car_Go_Target(0, -400, 300, 150);
                running_flag++;
                break;

            case 2: // ԭ����ת����90�ȵ������췽��x�᷽��
                printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                is_turning = 1;
                if (is_turning == 1 && Car_Turn_Use_IMU(90, 200, 150))
                {
                    is_turning = 0;
                }
                if (!is_turning)
                    running_flag++;
                break;

            case 3: // y�᷽����-1720mm
                printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                Counter_Times = 900;
                Counter_Enable = 1;
                Car_Go_Target(0, -1720, 400, 130);
                running_flag++;
                break;

             case 4: // ԭ����ת����90�ȵ������췽��x�᷽��
                 printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                 is_turning = 1;
                 if (is_turning == 1 && Car_Turn_Use_IMU(180, 200, 200))
                 {
                    is_turning = 0;
                 }
                if (!is_turning)
                     running_flag++;
                 break;
            case 5: // �������ж���ɣ�������һ��״̬
                printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                is_turning = 1;
                if (is_turning == 1 && Car_Calibration(200, 200))
                {
                    is_turning = 0;
                }
                if (!is_turning)
                {
                    running_flag = 0;
                    StateMachine_SetState(MOVE_TO_TEMPSTORE);
                }
								
//						 running_flag = 0;
//						StateMachine_SetState(MOVE_TO_TEMPSTORE);
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
                printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                printf("t5.txt=\"%d\"\xff\xff\xff", currentState);
                Counter_Times = 350;
                Counter_Enable = 1;
                Car_Go_Target(0, -875, 300, 150);
                running_flag++;
                break;

            case 1: // ԭ��������ת90�ȵ������췽��x�᷽��
                printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                is_turning = 1;
                if (is_turning == 1 && Car_Turn_Use_IMU(270, 200, 200))
                {
                    is_turning = 0;
                }
                if (!is_turning)
                    running_flag++;
                break;

            case 2: // y�᷽����-860mm
                printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                Counter_Times = 350;
                Counter_Enable = 1;
                Car_Go_Target(0, -860, 300, 150);
                running_flag++;
                break;
            case 3: // �������ж���ɣ�������һ��״̬
                printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                is_turning = 1;
                if (is_turning == 1 && Car_Calibration(200, 200))
                {
                    is_turning = 0;
                }
                if (!is_turning)
                {
                    running_flag = 0;
                    StateMachine_SetState(MOVE_TO_RAW_AGAIN);
                }
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
                printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                printf("t5.txt=\"%d\"\xff\xff\xff", currentState);
                Counter_Times = 350;
                Counter_Enable = 1;
                Car_Go_Target(0, -860, 300, 150);
                running_flag++;
                break;

            case 1: // ԭ��������ת90�ȵ������췽��x�᷽��
                printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                is_turning = 1;
                if (is_turning == 1 && Car_Turn_Use_IMU(0, 200, 200))
                {
                    is_turning = 0;
                }
                if (!is_turning)
                    running_flag++;
                break;

            case 2: // y�᷽����-425mm
                printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                Counter_Times = 280;
                Counter_Enable = 1;
                Car_Go_Target(0, -425, 250, 150);
                running_flag++;
                break;
            case 3: // �������ж���ɣ�������һ��״̬
                printf("t9.txt=\"%d\"\xff\xff\xff", running_flag);
                is_turning = 1;
                if (is_turning == 1 && Car_Calibration(200, 200))
                {
                    is_turning = 0;
                }
                if (!is_turning)
                {
                    running_flag = 0;
                    StateMachine_SetState(STATE_IDLE);
                }
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
