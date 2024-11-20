/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usart_api.h"
#include "Emm_V5.h"
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
// 定义实时位置全局变量
float pos = 0.0f, Motor_Cur_Pos = 0.0f;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
#ifdef __GNUC__

#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)

#else

#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)

#endif /* __GNUC__ */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int fputc(int ch, FILE *f)
{
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
  return ch;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  /**********************************************************
   ***	上电延时2秒等待Emm_V5.0闭环初始化完毕
   **********************************************************/
  HAL_Delay(2000);

  /**********************************************************
  ***	将当前四个电机位置清零
  **********************************************************/
  Emm_V5_Reset_CurPos_To_Zero(1);
  HAL_Delay(20);
  Emm_V5_Reset_CurPos_To_Zero(2);
  HAL_Delay(20);
  Emm_V5_Reset_CurPos_To_Zero(3);
  HAL_Delay(20);
  Emm_V5_Reset_CurPos_To_Zero(4);
  HAL_Delay(20);

  /**********************************************************
  ***	触发回零
  **********************************************************/
  Emm_V5_Synchronous_motion(0);
  HAL_Delay(3000);

  /**********************************************************
  ***	使用位置模式平缓运行一段时间
  **********************************************************/
  Emm_V5_Pos_Control(1, 1, 700, 50, 20000, 0, 1);
  HAL_Delay(20); // 延时1s
  Emm_V5_Pos_Control(2, 1, 700, 50, 20000, 0, 1);
  HAL_Delay(20); // 延时1s
  Emm_V5_Pos_Control(3, 1, 1000, 100, 20000, 0, 1);
  HAL_Delay(20); // 延时1s
  Emm_V5_Pos_Control(4, 1, 700, 50, 20000, 0, 1);
  HAL_Delay(20); // 延时1s
  Emm_V5_Synchronous_motion(0);

  /**********************************************************
  ***	到位后自动停止
  **********************************************************/
  //    Emm_V5_Stop_Now(0, 1);
  //    HAL_Delay(20);
  //    Emm_V5_Synchronous_motion(0);

  /**********************************************************
  ***	延时2秒，等待运动完成
  **********************************************************/
  HAL_Delay(8000);

  /**********************************************************
  ***	读取电机实时位置
  **********************************************************/
  Emm_V5_Read_Sys_Params(1, S_CPOS);

  /**********************************************************
  ***	等待返回命令，命令数据缓存在数组rxCmd上，长度为rxCount
  **********************************************************/
  while (rxFrameFlag == false){
    HAL_UART_Transmit(&huart1, (uint8_t *)"hello 1!\r\n", 16 , 0xffff);}
    ;
  rxFrameFlag = false;

  /**********************************************************
  ***	校验地址、功能码、返回数据长度，校验成功则计算当前位置角度
  **********************************************************/
  if (rxCmd[0] == 1 && rxCmd[1] == 0x36 && rxCount == 8)
  {
    // 拼接成uint32_t类型
    pos = (uint32_t)(((uint32_t)rxCmd[3] << 24) |
                     ((uint32_t)rxCmd[4] << 16) |
                     ((uint32_t)rxCmd[5] << 8) |
                     ((uint32_t)rxCmd[6] << 0));

    // 转换成角度
    Motor_Cur_Pos = (float)pos * 360.0f / 65536.0f;

    // 符号
    if (rxCmd[2])
    {
      Motor_Cur_Pos = -Motor_Cur_Pos;
    }
  }

  /* USER CODE END 2 */
while(1){
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    HAL_Delay(500);
    printf("Float Data: %f \r\n", pos);
    HAL_Delay(500);
    printf("Float Data: %f \r\n", Motor_Cur_Pos);
    HAL_UART_Transmit(&huart1, (uint8_t *)"hello 1!\r\n", 16 , 0xffff);
		 HAL_Delay(500);  //延时1s
    /* USER CODE END WHILE */
};
    /* USER CODE BEGIN 3 */
  /* USER CODE END 3 */

}
/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
