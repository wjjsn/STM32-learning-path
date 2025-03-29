/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
void HIDSend(int command);
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
#define BT1_Pin GPIO_PIN_12
#define BT1_GPIO_Port GPIOB
#define BT2_Pin GPIO_PIN_13
#define BT2_GPIO_Port GPIOB
#define BT3_Pin GPIO_PIN_14
#define BT3_GPIO_Port GPIOB
#define BT4_Pin GPIO_PIN_15
#define BT4_GPIO_Port GPIOB
#define BT5_Pin GPIO_PIN_8
#define BT5_GPIO_Port GPIOA
#define BT6_Pin GPIO_PIN_9
#define BT6_GPIO_Port GPIOA
#define BT7_Pin GPIO_PIN_10
#define BT7_GPIO_Port GPIOA
#define COIN_Pin GPIO_PIN_15
#define COIN_GPIO_Port GPIOA
#define SERVER_Pin GPIO_PIN_3
#define SERVER_GPIO_Port GPIOB
#define TEST_Pin GPIO_PIN_4
#define TEST_GPIO_Port GPIOB
#define HOME_Pin GPIO_PIN_5
#define HOME_GPIO_Port GPIOB
#define BT8_Pin GPIO_PIN_6
#define BT8_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define BT_TRIG_STATE 1
#define OUTHER_KET_TRIG_STATE 0
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
