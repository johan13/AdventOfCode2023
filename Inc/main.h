/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define PWM_PERIOD 8499
#define LCD_DISP_Pin GPIO_PIN_6
#define LCD_DISP_GPIO_Port GPIOF
#define RED_LED_Pin GPIO_PIN_8
#define RED_LED_GPIO_Port GPIOF
#define BACKLIGHT_EN_Pin GPIO_PIN_7
#define BACKLIGHT_EN_GPIO_Port GPIOA
#define LCD_DIM_PWM_Pin GPIO_PIN_4
#define LCD_DIM_PWM_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

#define DISPLAY_WIDTH (800)
#define DISPLAY_HEIGHT (480)
#define OVERLAY_WIDTH (400)
#define OVERLAY_HEIGHT (95 + 60 * 18) // 95px header + 60px per day
#define BACKGROUND_FRAME_BUF ((uint8_t *)0xc0000000) // Start of 2 MiB external SDRAM
#define BACKGROUND_FRAME_BUF_SIZE (DISPLAY_WIDTH * DISPLAY_HEIGHT)
#define OVERLAY_FRAME_BUF (BACKGROUND_FRAME_BUF + BACKGROUND_FRAME_BUF_SIZE)
#define OVERLAY_FRAME_BUF_SIZE (OVERLAY_WIDTH * OVERLAY_HEIGHT)
#define DECOMPRESS_BUF (OVERLAY_FRAME_BUF + OVERLAY_FRAME_BUF_SIZE)
#define DECOMPRESS_BUF_SIZE (32768)
#define FREE_SDRAM_START (DECOMPRESS_BUF + DECOMPRESS_BUF_SIZE)
#define FREE_SDRAM_SIZE ((uint8_t *)0xc0200000 - FREE_SDRAM_START)

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
