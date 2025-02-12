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
#include "stm32g4xx_hal.h"

#include "stm32g4xx_ll_ucpd.h"
#include "stm32g4xx_ll_bus.h"
#include "stm32g4xx_ll_cortex.h"
#include "stm32g4xx_ll_rcc.h"
#include "stm32g4xx_ll_system.h"
#include "stm32g4xx_ll_utils.h"
#include "stm32g4xx_ll_pwr.h"
#include "stm32g4xx_ll_gpio.h"
#include "stm32g4xx_ll_dma.h"

#include "stm32g4xx_ll_exti.h"

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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define output_ctrl_Pin GPIO_PIN_3
#define output_ctrl_GPIO_Port GPIOC
#define v_dc_sample_Pin GPIO_PIN_0
#define v_dc_sample_GPIO_Port GPIOA
#define i_in_sample_Pin GPIO_PIN_1
#define i_in_sample_GPIO_Port GPIOA
#define v_pd_sample_Pin GPIO_PIN_2
#define v_pd_sample_GPIO_Port GPIOA
#define v_out_sample_Pin GPIO_PIN_3
#define v_out_sample_GPIO_Port GPIOA
#define encoder_ch2_Pin GPIO_PIN_4
#define encoder_ch2_GPIO_Port GPIOA
#define v_in_sample_Pin GPIO_PIN_5
#define v_in_sample_GPIO_Port GPIOA
#define encoder_ch1_Pin GPIO_PIN_6
#define encoder_ch1_GPIO_Port GPIOA
#define buck_low_side_Pin GPIO_PIN_7
#define buck_low_side_GPIO_Port GPIOA
#define master_tx_Pin GPIO_PIN_4
#define master_tx_GPIO_Port GPIOC
#define master_rx_Pin GPIO_PIN_5
#define master_rx_GPIO_Port GPIOC
#define i_out_sample_Pin GPIO_PIN_0
#define i_out_sample_GPIO_Port GPIOB
#define serial_led_Pin GPIO_PIN_10
#define serial_led_GPIO_Port GPIOB
#define lcd_dc_Pin GPIO_PIN_11
#define lcd_dc_GPIO_Port GPIOB
#define lcd_cs_Pin GPIO_PIN_12
#define lcd_cs_GPIO_Port GPIOB
#define lcd_scl_Pin GPIO_PIN_13
#define lcd_scl_GPIO_Port GPIOB
#define lcd_bg_light_ctrl_Pin GPIO_PIN_14
#define lcd_bg_light_ctrl_GPIO_Port GPIOB
#define lcd_sda_Pin GPIO_PIN_15
#define lcd_sda_GPIO_Port GPIOB
#define boost_high_side_Pin GPIO_PIN_6
#define boost_high_side_GPIO_Port GPIOC
#define temp_sensor_scl_Pin GPIO_PIN_8
#define temp_sensor_scl_GPIO_Port GPIOC
#define temp_sensor_sda_Pin GPIO_PIN_9
#define temp_sensor_sda_GPIO_Port GPIOC
#define buck_high_side_Pin GPIO_PIN_8
#define buck_high_side_GPIO_Port GPIOA
#define debug_tx_Pin GPIO_PIN_10
#define debug_tx_GPIO_Port GPIOC
#define debug_rx_Pin GPIO_PIN_11
#define debug_rx_GPIO_Port GPIOC
#define boost_low_side_Pin GPIO_PIN_3
#define boost_low_side_GPIO_Port GPIOB
#define beep_Pin GPIO_PIN_5
#define beep_GPIO_Port GPIOB
#define btn_enc_Pin GPIO_PIN_7
#define btn_enc_GPIO_Port GPIOB
#define btn_1_Pin GPIO_PIN_8
#define btn_1_GPIO_Port GPIOB
#define btn_2_Pin GPIO_PIN_9
#define btn_2_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
