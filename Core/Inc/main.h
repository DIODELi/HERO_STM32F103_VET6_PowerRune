/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim6;
extern DMA_HandleTypeDef hdma_tim1_up;

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define AM_2_Pin GPIO_PIN_2
#define AM_2_GPIO_Port GPIOE
#define AM_3_Pin GPIO_PIN_3
#define AM_3_GPIO_Port GPIOE
#define AM_4_Pin GPIO_PIN_4
#define AM_4_GPIO_Port GPIOE
#define AM_5_Pin GPIO_PIN_5
#define AM_5_GPIO_Port GPIOE
#define Tune_Pin GPIO_PIN_5
#define Tune_GPIO_Port GPIOC
#define ARM1_Pin GPIO_PIN_0
#define ARM1_GPIO_Port GPIOD
#define ARM2_Pin GPIO_PIN_1
#define ARM2_GPIO_Port GPIOD
#define ARM3_Pin GPIO_PIN_3
#define ARM3_GPIO_Port GPIOD
#define ARM4_Pin GPIO_PIN_4
#define ARM4_GPIO_Port GPIOD
#define ARM5_Pin GPIO_PIN_5
#define ARM5_GPIO_Port GPIOD
#define AM_1_Pin GPIO_PIN_1
#define AM_1_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
