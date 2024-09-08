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
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "cmsis_os.h"

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
#define UART_Handle huart2
#define SPI_Handle hspi2
#define STEP_TIM_Handle htim3
#define RTC_Handle hrtc
#define BUILTIN_BTN_Pin GPIO_PIN_13
#define BUILTIN_BTN_GPIO_Port GPIOC
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define Y2_DIR_Pin GPIO_PIN_5
#define Y2_DIR_GPIO_Port GPIOA
#define Y2_STEP_Pin GPIO_PIN_6
#define Y2_STEP_GPIO_Port GPIOA
#define TFT_DC_Pin GPIO_PIN_1
#define TFT_DC_GPIO_Port GPIOB
#define Y_DIR_Pin GPIO_PIN_10
#define Y_DIR_GPIO_Port GPIOB
#define SPI2_SCK_Pin GPIO_PIN_13
#define SPI2_SCK_GPIO_Port GPIOB
#define TFT_CS_Pin GPIO_PIN_14
#define TFT_CS_GPIO_Port GPIOB
#define SPI2_MOSI_Pin GPIO_PIN_15
#define SPI2_MOSI_GPIO_Port GPIOB
#define Z_DIR_Pin GPIO_PIN_8
#define Z_DIR_GPIO_Port GPIOA
#define ENABLE_Pin GPIO_PIN_9
#define ENABLE_GPIO_Port GPIOA
#define X_STEP_Pin GPIO_PIN_10
#define X_STEP_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define Y_STEP_Pin GPIO_PIN_3
#define Y_STEP_GPIO_Port GPIOB
#define X_DIR_Pin GPIO_PIN_4
#define X_DIR_GPIO_Port GPIOB
#define Y_STEPB5_Pin GPIO_PIN_5
#define Y_STEPB5_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

#define BUILTIN_LED_Pin Y2_DIR_Pin
#define BUILTIN_LED_GPIO_Port Y2_DIR_GPIO_Port

extern osThreadId_t MainTask_Handle;
extern osMutexId_t RXBuffLockHandle;
extern osTimerId_t RXTimerHandle;

extern RTC_HandleTypeDef RTC_Handle;
extern SPI_HandleTypeDef SPI_Handle;
extern UART_HandleTypeDef UART_Handle;

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
