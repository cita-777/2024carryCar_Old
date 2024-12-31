#include "motor_ctrl.h"
#include "stdio.h"
#include "usr_tim.h"
uint8_t uart3_rxbuff;
uint8_t Send_Data[20];
uint8_t Stop_Flag_Car = 1; // С��ֹͣ��־λ
uint16_t RxBuffer3[10] = {0};
uint16_t Motor_HuaGui_Current = 0;
/*
---------------------------------�������ָ���---------------------------------
*/
// ����ֵ����
uint32_t My_ABS(int32_t temp)
{
    if (temp >= 0)
        return temp;
    else
        return -temp;
}

void Motor_Receive_Data(uint8_t com_data)
{
    uint8_t i;
    static uint8_t RxCounter1 = 0; // ����������¼���յ��������ֽ���
    static uint8_t RxState = 0;    // ����״̬������ǽ��յĵ�ǰ״̬

    // ���״̬����0����ʼ״̬�������ҽ��յ����ݰ��Ŀ�ʼ��־
    if (RxState == 0 && (com_data == 0x01 || com_data == 0x05))
    {
        RxState = 1;                        // �������״̬1
        RxBuffer3[RxCounter1++] = com_data; // �����յ������ݴ洢�ڽ��ջ�����
    }
    // ���״̬����1����������
    else if (RxState == 1)
    {
        RxBuffer3[RxCounter1++] = com_data; // �洢���յ�������

        // �жϽ��յ��ֽ����Ƿ�ﵽ10�������߽��յ�������־��0x6B��
        if (RxCounter1 >= 10 || com_data == 0x6B)
        {
            RxState = 2; // ���ݽ�����ɣ�����״̬2
        }
    }

    // ��״̬������״̬2����ʾ���ݽ��������
    if (RxState == 2)
    {
        // ������յ������һ���ֽ��ǽ�����־0x6B
        if (RxBuffer3[RxCounter1 - 1] == 0x6B)
        {
            RxCounter1 = 0; // ���ý��ռ�����
            RxState = 0;    // ����״̬��

            // �����յ������ݰ�ͷ�Ƿ����ض�����
            if (RxBuffer3[0] == 0x01 && RxBuffer3[1] == 0xFD && RxBuffer3[2] == 0x9F)
            {
                Stop_Flag_Car = 1; // ����ͣ����־
#if Serial_Debug == 1
                printf("Car_Stop\r\n"); // ��ӡ������Ϣ
#endif

                // ��ս��ջ�����
                for (i = 0; i < 4; i++)
                {
                    RxBuffer3[i] = 0x00;
                }
            }
            // �������һ���ض��������ͣ����־
            else if (RxBuffer3[0] == 0x05 && RxBuffer3[1] == 0xFD && RxBuffer3[2] == 0x9F && Stop_Flag_HuaGui == 0)
            {
                Stop_Flag_HuaGui = 1; // ����ͣ����־
#if Serial_Debug == 1
                printf("HuaGui_Stop\r\n"); // ��ӡ������Ϣ
#endif

                for (i = 0; i < 4; i++)
                {
                    RxBuffer3[i] = 0x00; // ��ս��ջ�����
                }
            }
            // ����ǿ��Ƶ����������ȡ��������
            else if (RxBuffer3[0] == 0x05 && RxBuffer3[1] == 0x27)
            {
                Motor_HuaGui_Current = RxBuffer3[2] << 8 | RxBuffer3[3]; // ��ϵ�������
            }
            else
            {
                // ������ݰ��������κ�Ԥ�ڸ�ʽ����ս��ջ�����
                for (i = 0; i < 10; i++)
                {
                    RxBuffer3[i] = 0x00;
                }
            }
        }
        else // ������յ������һ���ֽڲ��ǽ�����־0x6B������ʧ��
        {
            RxState = 0;       // ����״̬��
            RxCounter1 = 0;    // ���ý��ռ�����
            Stop_Flag_Car = 2; // ���ý��մ����־

            for (i = 0; i < 10; i++)
            {
                RxBuffer3[i] = 0x00; // ��ս��ջ�����
            }
        }
    }
}

void Motor_Enable(uint8_t Motor_Num)
{
    Send_Data[0] = Motor_Num;
    Send_Data[1] = 0xF3;
    Send_Data[2] = 0xAB;
    Send_Data[3] = 0x01;
    Send_Data[4] = 0x00;
    Send_Data[5] = 0x6B;
    HAL_UART_Transmit(&huart3, Send_Data, 6, 1000);
    Delay_ms(10);
}

void Motor_Disable(uint8_t Motor_Num)
{
    Send_Data[0] = Motor_Num;
    Send_Data[1] = 0xF3;
    Send_Data[2] = 0xAB;
    Send_Data[3] = 0x00;
    Send_Data[4] = 0x00;
    Send_Data[5] = 0x6B;
    HAL_UART_Transmit(&huart3, Send_Data, 6, 1000);
    Delay_ms(10);
}

void Motor_Reset(uint8_t Motor_Num)
{
    Send_Data[0] = Motor_Num;
    Send_Data[1] = 0xFE;
    Send_Data[2] = 0x98;
    Send_Data[3] = 0x00;
    Send_Data[4] = 0x6B;
    HAL_UART_Transmit(&huart3, Send_Data, 5, 1000);
    Delay_ms(10);
    Send_Data[0] = Motor_Num;
    Send_Data[1] = 0x0A;
    Send_Data[2] = 0x6D;
    Send_Data[3] = 0x6B;
    HAL_UART_Transmit(&huart3, Send_Data, 4, 1000);
    Delay_ms(10);
}

void Motor_Clear(uint8_t Motor_Num)
{
    Send_Data[0] = Motor_Num;
    Send_Data[1] = 0x0A;
    Send_Data[2] = 0x6D;
    Send_Data[3] = 0x6B;
    HAL_UART_Transmit(&huart3, Send_Data, 4, 1000);
    Delay_ms(10);
}

void Motor_Init(void)
{
    __HAL_UART_ENABLE_IT(&huart3, UART_IT_RXNE);
    HAL_UART_Receive_IT(&huart3, &uart3_rxbuff, 1);
    Motor_Reset(5);
    Motor_Reset(1);
    Motor_Reset(2);
    Motor_Reset(3);
    Motor_Reset(4);
    Delay_ms(100);
    // HuaGui_JiaoZhun();
    // Delay_ms(10);
    Motor_Enable_All();
}

void Motor_Enable_All(void)
{
    Motor_Enable(1);
    Motor_Enable(2);
    Motor_Enable(3);
    Motor_Enable(4);
    Motor_Enable(5);
}

void Motor_Disable_All(void)
{
    Motor_Disable(1);
    Motor_Disable(2);
    Motor_Disable(3);
    Motor_Disable(4);
    Motor_Disable(5);
}

void Motor_Set_PID(uint8_t Motor_Num, uint32_t KP, uint32_t KI, uint32_t KD)
{
    Send_Data[0] = Motor_Num;
    Send_Data[1] = 0x4A;
    Send_Data[2] = 0xC3;
    Send_Data[3] = 0x01;
    Send_Data[4] = KP >> 24;
    Send_Data[5] = (KP << 8) >> 24;
    Send_Data[6] = (KP << 16) >> 24;
    Send_Data[7] = (KP << 24) >> 24;
    Send_Data[8] = KI >> 24;
    Send_Data[9] = (KI << 8) >> 24;
    Send_Data[10] = (KI << 16) >> 24;
    Send_Data[11] = (KI << 24) >> 24;
    Send_Data[12] = KD >> 24;
    Send_Data[13] = (KD << 8) >> 24;
    Send_Data[14] = (KD << 16) >> 24;
    Send_Data[15] = (KD << 24) >> 24;
    Send_Data[16] = 0x6B;
    HAL_UART_Transmit(&huart3, Send_Data, 17, 1000);
    Delay_ms(10);
}

void Motor_Read_Current(uint8_t Motor_Num)
{
    Send_Data[0] = Motor_Num;
    Send_Data[1] = 0x27;
    Send_Data[2] = 0x6B;
    HAL_UART_Transmit(&huart3, Send_Data, 3, 1000);
    Delay_ms(20);
}

// ���ͬ���˶���־
void Motor_Run(void)
{
    Send_Data[0] = 0x00;
    Send_Data[1] = 0xFF;
    Send_Data[2] = 0x66;
    Send_Data[3] = 0x6B;
    HAL_UART_Transmit(&huart3, Send_Data, 4, 1000);
    // Delay_ms(10);
    // Delay_us(1);
}

// Speed ��λRPM
// Acc   0---255
void Motor_SetSpeed(uint8_t Motor_Num, int16_t Speed, uint8_t Acc)
{
    static uint32_t Last_Send_Time = 0; // ��̬�������ڼ�¼��һ�η���ʱ��
    uint8_t Direction;
    uint16_t Speed_Temp = My_ABS(Speed);

    // ��䷢������
    Send_Data[0] = Motor_Num;
    Send_Data[1] = 0xF6;

    if (Motor_Num == 1 || Motor_Num == 4)
    {
        Direction = (Speed >= 0) ? 0 : 1;
    }
    else
    {
        Direction = (Speed >= 0) ? 1 : 0;
    }

    Send_Data[2] = Direction;
    Send_Data[3] = Speed_Temp >> 8;
    Send_Data[4] = (Speed_Temp & 0xFF);
    Send_Data[5] = Acc;
    Send_Data[6] = 0x01;
    Send_Data[7] = 0x6B;

    //    // ��������ʱ�߼�
    //    if (Get_Time_Interval(HAL_GetTick(), Last_Send_Time, 1) == 1) // ������Ҫÿ 10ms ����һ��
    //    {
    //        HAL_UART_Transmit(&huart3, Send_Data, 8, 1000); // ��������
    //        Last_Send_Time = HAL_GetTick();                 // ���·���ʱ��
    //    }
    HAL_UART_Transmit(&huart3, Send_Data, 8, 1000); // ��������
    Delay_us(1);
}

// Speed ��λRPM
// Acc   0---255 (0����ر����μӼ���)
// Pulse 3200����һȦ
// �������
void Motor_SetPosition(uint8_t Motor_Num, uint32_t Pulse, int16_t Speed, uint8_t Acc)
{
    uint8_t Direction;
    uint16_t Speed_Temp = My_ABS(Speed);
    Send_Data[0] = Motor_Num;
    Send_Data[1] = 0xFD;

    if (Motor_Num == 1 || Motor_Num == 4)
    {
        if (Speed >= 0)
            Direction = 0;
        else
            Direction = 1;
    }
    else
    {
        if (Speed >= 0)
            Direction = 1;
        else
            Direction = 0;
    }

    Send_Data[2] = Direction;
    Send_Data[3] = Speed_Temp >> 8;
    Send_Data[4] = (Speed_Temp << 8) >> 8;
    Send_Data[5] = Acc;
    Send_Data[6] = Pulse >> 24;
    Send_Data[7] = (Pulse << 8) >> 24;
    Send_Data[8] = (Pulse << 16) >> 24;
    Send_Data[9] = (Pulse << 24) >> 24;
    Send_Data[10] = 0x00;
    Send_Data[11] = 0x01;
    Send_Data[12] = 0x6B;
    HAL_UART_Transmit(&huart3, Send_Data, 13, 1000);

    if (Motor_Num == 1)
    {
        Stop_Flag_Car = 0;
        Delay_ms(10);
        Stop_Flag_Car = 0;
    }

    Delay_us(1);
}

// Speed ��λRPM
// Acc   0---255 (0����ر����μӼ���)
// Pulse 3200����һȦ
// ��������
void Motor_SetPosition_A(uint8_t Motor_Num, uint32_t Pulse, int16_t Speed, uint8_t Acc)
{
    uint16_t Speed_Temp = My_ABS(Speed);
    uint8_t Direction;
    Send_Data[0] = Motor_Num;
    Send_Data[1] = 0xFD;

    if (Motor_Num == 1 || Motor_Num == 4)
    {
        if (Speed >= 0)
            Direction = 0;
        else
            Direction = 1;
    }
    else
    {
        if (Speed >= 0)
            Direction = 1;
        else
            Direction = 0;
    }

    Send_Data[2] = Direction;
    Send_Data[3] = Speed_Temp >> 8;
    Send_Data[4] = (Speed_Temp << 8) >> 8;
    Send_Data[5] = Acc;
    Send_Data[6] = Pulse >> 24;
    Send_Data[7] = (Pulse << 8) >> 24;
    Send_Data[8] = (Pulse << 16) >> 24;
    Send_Data[9] = (Pulse << 24) >> 24;
    Send_Data[10] = 0x01;
    Send_Data[11] = 0x01;
    Send_Data[12] = 0x6B;
    HAL_UART_Transmit(&huart3, Send_Data, 13, 1000);
    Delay_ms(10);

    if (Motor_Num == 1)
    {
        uint8_t i = 0;
        Stop_Flag_Car = 0;

        for (i = 0; i < 4; i++)
        {
            RxBuffer3[i] = 0x00; // �����������������
        }
    }

    if (Motor_Num == 5)
    {
        uint8_t i = 0;
        Stop_Flag_HuaGui = 0;

        for (i = 0; i < 4; i++)
        {
            RxBuffer3[i] = 0x00; // �����������������
        }

        Stop_Flag_HuaGui = 0;
    }
}

/*
---------------------------------С���˶�����---------------------------------
*/
// ���λ�ñջ� 12800����---4Ȧ---1��
void Car_Go(int16_t Angle, int16_t Speed, int32_t Distance, uint16_t Car_ACC)
{
    if (Angle >= 0 && Angle <= 90)
    {
        Motor_SetPosition(1, Distance, Speed, Car_ACC);
        Motor_SetPosition(2, My_ABS((45 * Distance / 45) - (Distance * Angle / 45)), (45 * Speed / 45) - (Speed * Angle / 45), Car_ACC);
        Motor_SetPosition(3, My_ABS((45 * Distance / 45) - (Distance * Angle / 45)), (45 * Speed / 45) - (Speed * Angle / 45), Car_ACC);
        Motor_SetPosition(4, Distance, Speed, Car_ACC);
    }
    else if (Angle > 90 && Angle <= 180)
    {
        Motor_SetPosition(1, My_ABS((135 * Distance / 45) - (Distance * Angle / 45)), (135 * Speed / 45) - (Speed * Angle / 45), Car_ACC);
        Motor_SetPosition(2, Distance, -Speed, Car_ACC);
        Motor_SetPosition(3, Distance, -Speed, Car_ACC);
        Motor_SetPosition(4, My_ABS((135 * Distance / 45) - (Distance * Angle / 45)), (135 * Speed / 45) - (Speed * Angle / 45), Car_ACC);
    }
    else if (Angle > -90 && Angle < 0)
    {
        Motor_SetPosition(1, My_ABS((45 * Distance + Distance * Angle) / 45), (45 * Speed / 45) + (Speed * Angle / 45), Car_ACC);
        Motor_SetPosition(2, Distance, Speed, Car_ACC);
        Motor_SetPosition(3, Distance, Speed, Car_ACC);
        Motor_SetPosition(4, My_ABS((45 * Distance + Distance * Angle) / 45), (45 * Speed / 45) + (Speed * Angle / 45), Car_ACC);
    }
    else if (Angle >= -180 && Angle <= -90)
    {
        Motor_SetPosition(1, Distance, -Speed, Car_ACC);
        Motor_SetPosition(2, My_ABS((135 * Distance + Distance * Angle) / 45), (135 * Speed + Speed * Angle) / 45, Car_ACC);
        Motor_SetPosition(3, My_ABS((135 * Distance + Distance * Angle) / 45), (135 * Speed + Speed * Angle) / 45, Car_ACC);
        Motor_SetPosition(4, Distance, -Speed, Car_ACC);
    }

    Motor_Run();
}

// ���λ�ñջ� 12800����---4Ȧ---1��
// YawΪ�Ƕ�,������λΪ�ף��ٶȺͼ��ٶȵ��Ĭ�ϵĵ�λrpm
// Wheel_axlespacing ΪС��(ǰ��)���Ķ���֮һ��Wheel_spacing ΪС��(����)�־�Ķ���֮һ��
// void Car_Go_Target(int32_t Tar_X, int32_t Tar_Y, int32_t Tar_Yaw, int16_t Speed, uint16_t Car_ACC)
//{
//    float Wheel_axlespacing=0.09;//ǰ��
//    float Wheel_spacing=0.12692;  //����
//    float rad = Tar_Yaw * 3.1415926 / 180;
//    float Motor_A_Pos = Tar_X + Tar_Y - rad * (Wheel_spacing + Wheel_axlespacing);
//    float Motor_B_Pos = Tar_X - Tar_Y - rad * (Wheel_spacing + Wheel_axlespacing);
//    float Motor_C_Pos = Tar_X + Tar_Y + rad * (Wheel_spacing + Wheel_axlespacing);
//    float Motor_D_Pos = Tar_X - Tar_Y + rad * (Wheel_spacing + Wheel_axlespacing);

//    Motor_SetPosition(1, Motor_A_Pos, Speed, Car_ACC);
//    Motor_SetPosition(2, Motor_B_Pos, Speed, Car_ACC);
//    Motor_SetPosition(3, Motor_C_Pos, Speed, Car_ACC);
//    Motor_SetPosition(4, Motor_D_Pos, Speed, Car_ACC);

//    Motor_Run();
//}3200����204.2035224833365605����
// ���λ�ñջ�  1mm = 7.835320275293308837����
void Car_Go_Target(int32_t Tar_X_mm, int32_t Tar_Y_mm, int16_t Speed, uint16_t Car_ACC)
{
    int32_t Tar_X = Tar_X_mm * 15.67064055058661768;
    int32_t Tar_Y = Tar_Y_mm * 15.67064055058661768;
    int32_t V1, V2;
    V2 = Speed * My_ABS(Tar_Y - Tar_X) / (1.4 * sqrt(Tar_X * Tar_X + Tar_Y * Tar_Y));
    V1 = Speed * My_ABS(Tar_Y + Tar_X) / (1.4 * sqrt(Tar_X * Tar_X + Tar_Y * Tar_Y));

    if (Tar_X + Tar_Y >= 0)
    {
        Motor_SetPosition(1, Tar_X + Tar_Y, V1, Car_ACC);
        Motor_SetPosition(3, Tar_X + Tar_Y, V1, Car_ACC);
    }
    else
    {
        Motor_SetPosition(1, -(Tar_X + Tar_Y), -V1, Car_ACC);
        Motor_SetPosition(3, -(Tar_X + Tar_Y), -V1, Car_ACC);
    }

    if (Tar_Y - Tar_X >= 0)
    {
        Motor_SetPosition(2, Tar_Y - Tar_X, V2, Car_ACC);
        Motor_SetPosition(4, Tar_Y - Tar_X, V2, Car_ACC);
    }
    else
    {
        Motor_SetPosition(2, Tar_X - Tar_Y, -V2, Car_ACC);
        Motor_SetPosition(4, Tar_X - Tar_Y, -V2, Car_ACC);
    }

    Motor_Run();
}

// ����λ�ñջ�
void Car_Go_Target_A(int32_t Tar_X, int32_t Tar_Y, int16_t Speed, uint16_t Car_ACC)
{
    static int32_t Car_Now_X = 0, Car_Now_Y = 0;
    int32_t Go_X, Go_Y;
    Go_X = Tar_X - Car_Now_X;
    Go_Y = Tar_Y - Car_Now_Y;
    Car_Now_X = Tar_X;
    Car_Now_Y = Tar_Y;
    int32_t V1, V2;
    V2 = Speed * My_ABS(Go_Y - Go_X) / (1.4 * sqrt(Go_X * Go_X + Go_Y * Go_Y));
    V1 = Speed * My_ABS(Go_Y + Go_X) / (1.4 * sqrt(Go_X * Go_X + Go_Y * Go_Y));

    if (Go_X + Go_Y >= 0)
    {
        Motor_SetPosition_A(1, Go_X + Go_Y, V1, Car_ACC);
        Motor_SetPosition_A(4, Go_X + Go_Y, V1, Car_ACC);
    }
    else
    {
        Motor_SetPosition_A(1, -(Go_X + Go_Y), -V1, Car_ACC);
        Motor_SetPosition_A(4, -(Go_X + Go_Y), -V1, Car_ACC);
    }

    if (Go_Y - Go_X >= 0)
    {
        Motor_SetPosition_A(2, Go_Y - Go_X, V2, Car_ACC);
        Motor_SetPosition_A(3, Go_Y - Go_X, V2, Car_ACC);
    }
    else
    {
        Motor_SetPosition_A(2, Go_X - Go_Y, -V2, Car_ACC);
        Motor_SetPosition_A(3, Go_X - Go_Y, -V2, Car_ACC);
    }

    Motor_Run();
}

// ������ֵ����ǰ�������ۼ���
void Car_Clear(void)
{
    Motor_Clear(1);
    Motor_Clear(2);
    Motor_Clear(3);
    Motor_Clear(4);
}
uint8_t Car_Turn_NoUse_IMU(int16_t Tar_Yaw, uint16_t Speed_Limit, uint16_t Car_ACC)
{

    static float Alpha = 56.4; // Alpha 46.8
    uint8_t ret = 0;
    static uint8_t Temp_State = 0;

    if (Temp_State == 0)
    {
        

        if (Tar_Yaw >= 0)
        {
            Motor_SetPosition(1, Tar_Yaw * Alpha, Speed_Limit, Car_ACC);
            Motor_SetPosition(2, Tar_Yaw * Alpha, -Speed_Limit, Car_ACC);
            Motor_SetPosition(3, Tar_Yaw * Alpha, -Speed_Limit, Car_ACC);
            Motor_SetPosition(4, Tar_Yaw * Alpha, Speed_Limit, Car_ACC);
            Motor_Run();
        }
        else
        {
            Motor_SetPosition(1, -Tar_Yaw * Alpha, Speed_Limit, Car_ACC);
            Motor_SetPosition(2, -Tar_Yaw * Alpha, -Speed_Limit, Car_ACC);
            Motor_SetPosition(3, -Tar_Yaw * Alpha, Speed_Limit, Car_ACC);
            Motor_SetPosition(4, -Tar_Yaw * Alpha, -Speed_Limit, Car_ACC);
            Motor_Run();
        }
				Temp_State = 1;
    }
    else if (Temp_State == 1)
    {	
				
        ret = 1;
        Temp_State = 0;
				
    }

    return ret;
}
// С��ת��
uint8_t Car_Turn_Use_IMU(int16_t Tar_Yaw, uint16_t Speed_Limit, uint16_t Car_ACC)
{
	IMU_Data_Proc();
    
    static uint8_t Temp_State = 0;
    static uint8_t Stop_Counter = 0;
    static float Temp_Target_Yaw = 0.0f;
    static float cal_YawAngle = 0.0f;
    static float Yaw_Error = 0.0f;
    static float Yaw_Integral = 0.0f;
    static float Yaw_Derivative = 0.0f;
    static float Previous_Yaw_Error = 0.0f;
    static float Motor_Kp = 0.6f;  // ת��KP
    static float Motor_Ki = 0.00f; // ת��KI
    static float Motor_Kd = 0.1f;  // ת��KD
	  static float Control_Output=0.0f;
    uint8_t ret = 0;

    if (YawAngle != 0)
    {
        cal_YawAngle = YawAngle;
    }

    if (Temp_State == 0) // ��ûת�䣬����׼����ת��
    {
        Temp_State = 1;
        Temp_Target_Yaw = Tar_Yaw;
        Yaw_Integral = 0.0f;
        Previous_Yaw_Error = 0.0f;
    }

    else if (Temp_State == 1) // �� Temp_State == 1 ʱ����ʾС�����ڽ���ת��
    {
        Yaw_Error = cal_YawAngle - Temp_Target_Yaw;
        if (Yaw_Error >= 180.0f)
            Yaw_Error -= 360.0f;
        else if (Yaw_Error <= -180.0f)
            Yaw_Error += 360.0f;

        Yaw_Integral += Yaw_Error;
        Yaw_Derivative = Yaw_Error - Previous_Yaw_Error;
        Previous_Yaw_Error = Yaw_Error;

         Control_Output = Motor_Kp * Yaw_Error + Motor_Ki * Yaw_Integral + Motor_Kd * Yaw_Derivative;
				     printf("t2.txt=\"%f\"\xff\xff\xff", cal_YawAngle);
        printf("t3.txt=\"%f\"\xff\xff\xff", Control_Output);
        if (Control_Output > Speed_Limit)
            Control_Output = Speed_Limit;
        else if (Control_Output < -Speed_Limit)
            Control_Output = -Speed_Limit;

        Motor_SetSpeed(1, -Control_Output, Car_ACC);
        Motor_SetSpeed(2, Control_Output, Car_ACC);
        Motor_SetSpeed(3, Control_Output, Car_ACC);
        Motor_SetSpeed(4, -Control_Output, Car_ACC);
        Motor_Run();

        if (cal_YawAngle >= Temp_Target_Yaw - 2 && cal_YawAngle <= Temp_Target_Yaw + 2)
            Stop_Counter++;
        else
            Stop_Counter = 0;

        if (Stop_Counter >= 3)
        {
            Stop_Counter = 0;
            ret = 1;
            Temp_State = 0;
        }
    }
    return ret;
}
uint8_t Car_Turn(int16_t Tar_Yaw, uint16_t Speed_Limit, uint16_t Car_ACC)
{
	//IMU_Data_Proc();
    uint8_t ret = 0;
    static uint8_t temp_state = 0;

    if (temp_state == 0)
    {
        if(Car_Turn_NoUse_IMU(Tar_Yaw, Speed_Limit, Car_ACC))
        {
            temp_state = 1;
        }
    }
    else
    {
        if(Car_Turn_Use_IMU(Tar_Yaw, Speed_Limit, Car_ACC))
        {
            ret = 1;
            temp_state = 0;
        }
    }

    return ret;
}
// �������
uint8_t Car_Calibration(uint16_t Speed_Limit, uint16_t Car_ACC)
{
    static uint8_t Temp_State = 0;
    static float Temp_Target_Cal_Angle = 0;
    static float cal_YawAngle = 0.0f;
    uint8_t ret = 0;
    if (YawAngle != 0)
    {
        cal_YawAngle = YawAngle;
    }
    if (Temp_State == 0) // ��û��ʼУ׼
    {
        Temp_State++;
        Temp_Target_Cal_Angle = cal_YawAngle;
    }
    else if (Temp_State == 1) // ����У׼��
    {
        uint8_t temp = 0;

        if (Temp_Target_Cal_Angle <= 10 || (Temp_Target_Cal_Angle >= 350 && Temp_Target_Cal_Angle <= 360))
        {
            temp = Car_Turn_Use_IMU(0, Speed_Limit, Car_ACC);
        }

        if (Temp_Target_Cal_Angle <= 95 && Temp_Target_Cal_Angle >= 85)
        {
            temp = Car_Turn_Use_IMU(90, Speed_Limit, Car_ACC);
        }
        if (Temp_Target_Cal_Angle <= 185 && Temp_Target_Cal_Angle >= 175)
        {
            temp = Car_Turn_Use_IMU(180, Speed_Limit, Car_ACC);
        }
        if (Temp_Target_Cal_Angle <= 275 && Temp_Target_Cal_Angle >= 265)
        {
            temp = Car_Turn_Use_IMU(270, Speed_Limit, Car_ACC);
        }

        if (temp == 1)
        {
            ret = 1;
            Temp_State = 0;
            Temp_Target_Cal_Angle = 0;
        }
    }

    return ret;
}

/*
---------------------------------�����˶�����---------------------------------
*/
#define Motor_HuaGui_Pulse_DEFAULT 0
#define Motor_HuaGui_Pulse_GROUND 7000
#define Motor_HuaGui_Pulse_ZhuanPan 2000
#define HuaGui_Motor_Pulse_Fang_To_Map 4500
#define HuaGui_Motor_Pulse_Get_From_Map 4500
#define Motor_HuaGui_Pulse_DOWN2 5500
uint8_t HuaGui_Motor_State = HuaGui_Motor_State_UP;
uint8_t Stop_Flag_HuaGui = 1; // ������ֹͣ��־λ
// ������λ��λ
void HuaGui_JiaoZhun(void)
{
    HuaGui_Motor_State = HuaGui_Motor_State_UP;
    Send_Data[0] = 0x05;
    Send_Data[1] = 0x9A;
    Send_Data[2] = 0x02;
    Send_Data[3] = 0x00;
    Send_Data[4] = 0x6B;
    HAL_UART_Transmit(&huart3, Send_Data, 5, 1000);
    Delay_ms(10);
}

// �����ƶ������϶���λ��
void HuaGui_UP(uint16_t Motor_HuaGui_Speed, uint16_t Motor_HuaGui_Acc)
{
    HuaGui_Motor_State = HuaGui_Motor_State_UP;
    Motor_SetPosition_A(5, Motor_HuaGui_Pulse_DEFAULT, Motor_HuaGui_Speed, Motor_HuaGui_Acc);
    Motor_Run();
}

// �����ƶ������¶�ȡ��λ��
void HuaGui_DOWN(uint16_t Motor_HuaGui_Speed, uint16_t Motor_HuaGui_Acc)
{
    HuaGui_Motor_State = HuaGui_Motor_State_DOWN;
    Motor_SetPosition_A(5, Motor_HuaGui_Pulse_GROUND, Motor_HuaGui_Speed, Motor_HuaGui_Acc);
    Motor_Run();
}

// �����ƶ����ڶ���ȡ��λ��
void HuaGui_DOWN2(uint16_t Motor_HuaGui_Speed, uint16_t Motor_HuaGui_Acc)
{
    HuaGui_Motor_State = HuaGui_Motor_State_DOWN2;
    Motor_SetPosition_A(5, Motor_HuaGui_Pulse_DOWN2, Motor_HuaGui_Speed, Motor_HuaGui_Acc);
    Motor_Run();
}

// �����ƶ�������̨����λ��
void HuaGui_Fang_To_Map(uint16_t Motor_HuaGui_Speed, uint16_t Motor_HuaGui_Acc)
{
    HuaGui_Motor_State = HuaGui_Motor_State_Fang_To_Map;
    Motor_SetPosition_A(5, HuaGui_Motor_Pulse_Fang_To_Map, Motor_HuaGui_Speed, Motor_HuaGui_Acc);
    Motor_Run();
}

// �����ƶ�������̨ȡ��λ��
void HuaGui_Get_From_Map(uint16_t Motor_HuaGui_Speed, uint16_t Motor_HuaGui_Acc)
{
    HuaGui_Motor_State = HuaGui_Motor_Pulse_Get_From_Map;
    Motor_SetPosition_A(5, HuaGui_Motor_Pulse_Get_From_Map, Motor_HuaGui_Speed, Motor_HuaGui_Acc);
    Motor_Run();
}

// �����ƶ���ת��ȡ��λ��
void HuaGui_ZhuanPan(uint16_t Motor_HuaGui_Speed, uint16_t Motor_HuaGui_Acc)
{
    HuaGui_Motor_State = HuaGui_Motor_State_ZhuanPan;
    Motor_SetPosition_A(5, Motor_HuaGui_Pulse_ZhuanPan, Motor_HuaGui_Speed, Motor_HuaGui_Acc);
    Motor_Run();
}

