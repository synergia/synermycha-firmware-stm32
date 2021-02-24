/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
extern "C"
{
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

    /* Exported functions prototypes ---------------------------------------------*/
    void Error_Handler(void);

    /* USER CODE BEGIN EFP */
    extern void SystemClock_Config(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define VC53L0x_XSHUT_FRONT_RIGHT_Pin       GPIO_PIN_13
#define VC53L0x_XSHUT_FRONT_RIGHT_GPIO_Port GPIOC
#define VC53L0x_XSHUT_FRONT_Pin             GPIO_PIN_14
#define VC53L0x_XSHUT_FRONT_GPIO_Port       GPIOC
#define LED_5_Pin                           GPIO_PIN_15
#define LED_5_GPIO_Port                     GPIOC
#define LED_4_Pin                           GPIO_PIN_0
#define LED_4_GPIO_Port                     GPIOC
#define LED_3_Pin                           GPIO_PIN_1
#define LED_3_GPIO_Port                     GPIOC
#define LED_2_Pin                           GPIO_PIN_2
#define LED_2_GPIO_Port                     GPIOC
#define LED_1_Pin                           GPIO_PIN_3
#define LED_1_GPIO_Port                     GPIOC
#define LED_MOUSE_EYE_Pin                   GPIO_PIN_2
#define LED_MOUSE_EYE_GPIO_Port             GPIOA
#define BUZZER_Pin                          GPIO_PIN_3
#define BUZZER_GPIO_Port                    GPIOA
#define VC53L0x_XSHUT_FRONT_LEFT_Pin        GPIO_PIN_4
#define VC53L0x_XSHUT_FRONT_LEFT_GPIO_Port  GPIOA
#define ENC_L_B_Pin                         GPIO_PIN_6
#define ENC_L_B_GPIO_Port                   GPIOA
#define ENC_L_A_Pin                         GPIO_PIN_7
#define ENC_L_A_GPIO_Port                   GPIOA
#define VC53L0x_XSHUT_LEFT_Pin              GPIO_PIN_4
#define VC53L0x_XSHUT_LEFT_GPIO_Port        GPIOC
#define RN4871_NON_Pin                      GPIO_PIN_5
#define RN4871_NON_GPIO_Port                GPIOC
#define BATT_VOLTAGE_Pin                    GPIO_PIN_0
#define BATT_VOLTAGE_GPIO_Port              GPIOB
#define RN4871_NRESET_Pin                   GPIO_PIN_1
#define RN4871_NRESET_GPIO_Port             GPIOB
#define RN4871_NFLASH_MODE_Pin              GPIO_PIN_2
#define RN4871_NFLASH_MODE_GPIO_Port        GPIOB
#define LCD_SCL_Pin                         GPIO_PIN_10
#define LCD_SCL_GPIO_Port                   GPIOB
#define DRV8835_EN_Pin                      GPIO_PIN_12
#define DRV8835_EN_GPIO_Port                GPIOB
#define DRV8835_DIR_A_Pin                   GPIO_PIN_13
#define DRV8835_DIR_A_GPIO_Port             GPIOB
#define DRV8835_PWM_A_Pin                   GPIO_PIN_14
#define DRV8835_PWM_A_GPIO_Port             GPIOB
#define DRV8835_PWM_B_Pin                   GPIO_PIN_15
#define DRV8835_PWM_B_GPIO_Port             GPIOB
#define DRV8835_DIR_B_Pin                   GPIO_PIN_6
#define DRV8835_DIR_B_GPIO_Port             GPIOC
#define IMU_SDA_Pin                         GPIO_PIN_9
#define IMU_SDA_GPIO_Port                   GPIOC
#define IMU_SCL_Pin                         GPIO_PIN_8
#define IMU_SCL_GPIO_Port                   GPIOA
#define RN4871_RX_Pin                       GPIO_PIN_9
#define RN4871_RX_GPIO_Port                 GPIOA
#define RN4871_TX_Pin                       GPIO_PIN_10
#define RN4871_TX_GPIO_Port                 GPIOA
#define STM32_D__Pin                        GPIO_PIN_11
#define STM32_D__GPIO_Port                  GPIOA
#define STM32_D_A12_Pin                     GPIO_PIN_12
#define STM32_D_A12_GPIO_Port               GPIOA
#define SWDIO_Pin                           GPIO_PIN_13
#define SWDIO_GPIO_Port                     GPIOA
#define SWCLK_Pin                           GPIO_PIN_14
#define SWCLK_GPIO_Port                     GPIOA
#define BUTTON_DOWN_Pin                     GPIO_PIN_15
#define BUTTON_DOWN_GPIO_Port               GPIOA
#define BUTTON_DOWN_EXTI_IRQn               EXTI15_10_IRQn
#define BUTTON_UP_Pin                       GPIO_PIN_10
#define BUTTON_UP_GPIO_Port                 GPIOC
#define BUTTON_UP_EXTI_IRQn                 EXTI15_10_IRQn
#define BUTTON_OK_Pin                       GPIO_PIN_11
#define BUTTON_OK_GPIO_Port                 GPIOC
#define BUTTON_OK_EXTI_IRQn                 EXTI15_10_IRQn
#define LCD_SDA_Pin                         GPIO_PIN_12
#define LCD_SDA_GPIO_Port                   GPIOC
#define VC53L0x_XSHUT_RIGHT_Pin             GPIO_PIN_2
#define VC53L0x_XSHUT_RIGHT_GPIO_Port       GPIOD
#define SWO_Pin                             GPIO_PIN_3
#define SWO_GPIO_Port                       GPIOB
#define ENC_R_B_Pin                         GPIO_PIN_6
#define ENC_R_B_GPIO_Port                   GPIOB
#define ENC_R_A_Pin                         GPIO_PIN_7
#define ENC_R_A_GPIO_Port                   GPIOB
#define VL53L0x_SCL_Pin                     GPIO_PIN_8
#define VL53L0x_SCL_GPIO_Port               GPIOB
#define VL53L0x_SDA_Pin                     GPIO_PIN_9
#define VL53L0x_SDA_GPIO_Port               GPIOB
    /* USER CODE BEGIN Private defines */

    /* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
