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
#include "cmsis_os.h"
#include "can.h"
#include "crc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ecat_def.h"
#include "applInterface.h"
#include "SSC-Device.h"
#include "oled.h"
#include "stdio.h"
#include "cpp_app.h"


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* 告知连接器不从C库链接使用半主机的函数 */

//#pragma import(__use_no_semihosting)
 
/* 定义 _sys_exit() 以避免使用半主机模式 */
 
void _sys_exit(int x)
 
{
 
    x = x;
 
}

 
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

int8_t flag;

extern float hz_detect ;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void OLED_ShowStatus()
{
    // 显示标题
    OLED_ShowString(0, 0, "SONY-EtherCAT", 16);  // 第一行显示标题

    // 显示设备状态
    for (int i = 0; i < ERROR_LIST_LENGHT; i++) 
    {
        uint8_t status_char = '1';  // 默认正常状态，显示'1'（正常）

        if (error_list[i].error_exist)
        {
            if (error_list[i].is_lost)
            {
                status_char = '0';  // 错误且丢失，显示'0'（丢失）
            }
            else
            {
                status_char = '0';  // 仅错误，显示'2'（错误）
            }
        }
        else if (error_list[i].is_lost)
        {
            status_char = '0';  // 仅丢失，显示'0'（丢失）
        }

        // 计算显示位置
        int row = 16 + (i / 3) * 16;  // 每行显示三个设备
        int col = (i % 3) * 36;  // 每个设备占24个像素位置

        // 显示 M1, M2, M3, ... 和状态
        OLED_ShowChar(col, row, 'M', 12, 1);  // 显示'M'
        OLED_ShowChar(col + 8, row, '1' + i, 12, 1);  // 显示M1, M2, M3, ...
        OLED_ShowChar(col + 16, row, ':', 12, 1);  // 显示冒号
        OLED_ShowChar(col + 24, row, status_char, 12, 1);  // 显示状态 '0', '1' 或 '2'
    }
		
		// 显示控制频率
    char freq_str[16];
    snprintf(freq_str, sizeof(freq_str), "Freq: %.2f Hz", hz_detect);  // 格式化频率字符串
    OLED_ShowString(0, 45, freq_str, 12);  // 在最后一行显示控制频率

    // 刷新OLED显示
    OLED_Refresh_Gram();
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
  MX_DMA_Init();
  MX_FSMC_Init();
  MX_TIM3_Init();
  MX_UART4_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_USART6_UART_Init();
  MX_CAN1_Init();
  MX_CRC_Init();
  /* USER CODE BEGIN 2 */
	HW_Init();
	MainInit();
	cpp_main_init();
	
	//HAL_TIM_Base_Start_IT(&htim2);
	//HAL_TIM_Base_Start_IT(&htim4);
	//HAL_TIM_Base_Start_IT(&htim5);
	
	OLED_Init();		
	//OLED_ShowString(0,0,"SONY-EtherCAT ",16);  
	//OLED_ShowString(0,16, "Servo Drive",12); 
	//OLED_ShowString(0,28,"AX58100+F407",12); 	
	//OLED_ShowString(0,45,"TIME 2025/2/17",12); 
	//OLED_Refresh_Gram();//������ʾ��OLED
		
  /* USER CODE END 2 */

  /* Call init function for freertos objects (in cmsis_os2.c) */
  MX_FREERTOS_Init();

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */


  }
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
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/************* EXTI TEST ************/
//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
//{
//	if(GPIO_Pin==PDI_INT_Pin)
//	{
//		SEGGER_RTT_printf(0, "PDI_INT_Pin\n");
//	}
//	else if(GPIO_Pin==SYNC0_INT_Pin)
//	{
//		SEGGER_RTT_printf(0, "SYNC0_INT_Pin\n");
//	}
//	else if(GPIO_Pin==SYNC1_INT_Pin)
//	{
//		SEGGER_RTT_printf(0, "SYNC1_INT_Pin\n");
//	}
//}

/************* TIMER TEST ************/
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//	if(htim == &htim3)
//	{
//		ECAT_CheckTimer();
//		static uint16_t i = 0;
//		i++;
//		if(i == 1000)
//		{
//			SEGGER_RTT_printf(0, "HAL_TIM_PeriodElapsedCallback\n");
//			i = 0;
//		}
//	}
//}

/************* FSMC test ************/
//		if(HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin) == GPIO_PIN_RESET)
//		{
//				for(uint8_t i = 0; i < 128; i++)
//				{
//						for(uint8_t j = 0; j < 64; j++)
//						{
//							 SEGGER_RTT_printf(0, "%02x ", *((uint8_t *)(0x6C000000) + j + i*128));
//						}
//						SEGGER_RTT_printf(0, "\n");
//						HAL_Delay(2);
//				}
//		}
//		HAL_Delay(2000);





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
