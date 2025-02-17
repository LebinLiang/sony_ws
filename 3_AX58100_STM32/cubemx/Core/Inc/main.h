/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "tim.h"						/* timer */
#include "SEGGER_RTT.h"			/* print */
#include "string.h"					/* memcpy */
#include "stdlib.h"					/* malloc */
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define KEY_2_Pin GPIO_PIN_2
#define KEY_2_GPIO_Port GPIOE
#define KEY_1_Pin GPIO_PIN_3
#define KEY_1_GPIO_Port GPIOE
#define KEY_0_Pin GPIO_PIN_4
#define KEY_0_GPIO_Port GPIOE
#define WK_UP_Pin GPIO_PIN_5
#define WK_UP_GPIO_Port GPIOE
#define PDI_INT_Pin GPIO_PIN_0
#define PDI_INT_GPIO_Port GPIOC
#define PDI_INT_EXTI_IRQn EXTI0_IRQn
#define SYNC1_INT_Pin GPIO_PIN_1
#define SYNC1_INT_GPIO_Port GPIOC
#define SYNC1_INT_EXTI_IRQn EXTI1_IRQn
#define SYNC0_INT_Pin GPIO_PIN_3
#define SYNC0_INT_GPIO_Port GPIOC
#define SYNC0_INT_EXTI_IRQn EXTI3_IRQn
#define LED_Pin GPIO_PIN_1
#define LED_GPIO_Port GPIOB
#define OLED_CS_Pin GPIO_PIN_3
#define OLED_CS_GPIO_Port GPIOG
#define OLED_DC_Pin GPIO_PIN_4
#define OLED_DC_GPIO_Port GPIOG
#define OLED_RES_Pin GPIO_PIN_5
#define OLED_RES_GPIO_Port GPIOG
#define OLED_MOSI_Pin GPIO_PIN_6
#define OLED_MOSI_GPIO_Port GPIOG
#define OLED_CLK_Pin GPIO_PIN_7
#define OLED_CLK_GPIO_Port GPIOG
#define BEEP_Pin GPIO_PIN_12
#define BEEP_GPIO_Port GPIOC
#define LED_1_Pin GPIO_PIN_2
#define LED_1_GPIO_Port GPIOD
#define LED_2_Pin GPIO_PIN_9
#define LED_2_GPIO_Port GPIOG
#define LED_3_Pin GPIO_PIN_10
#define LED_3_GPIO_Port GPIOG
#define LED_4_Pin GPIO_PIN_11
#define LED_4_GPIO_Port GPIOG
#define LED_5_Pin GPIO_PIN_15
#define LED_5_GPIO_Port GPIOG

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
