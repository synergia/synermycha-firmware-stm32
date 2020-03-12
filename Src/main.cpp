/* USER CODE BEGIN Header */
/**
 *
                     ,gKRMRKWw                                  ]M`"MKWw,
                  zR"  ,,,,   `9w                              #M,R"*KWwJ"MNm,
                ,K` aM"`  `"9W   *N                           #G,R      `"MNm,`K
               ]M #M          "N,  9w                        #`,R       rY   1N K
              ]M,K`             "N  "N                      K`,K     .m"      Vp1N
              K K                 9p  K  ,w#MM""""""*RWw, ,K ,K      ] \/      K 9p
             .N]N                  1m  R"`           ]R``*R ,K       A ]  ,/~  'N K
             jN'N                   ]                K      9W    ,* a" z*`  ,gM` ]N
              K 1W                ,K`                0       "K,A`,* `"`  ,aR" ,#M`
               K,`Vw             #"                  'N        YN` /~   aK" z#M`
                "Nw "Km,       ]M                     K         `9D  ,#*,wR"
                   "RWw,J""*MMM`    ,gw              ]M   ,~~,    "K*,gR"
                        ``""1M      KKKK             K    H         9C
                           ]M        ""              K     ""`       9m
                          ]M                         *Nw,        ,,wgR9p
                         ,R                    ,www     `"*MMMM*"``    0
                         #    -~^"``J2g       'KKKK       .w&J``"^~-    N
                        ]N     .~^``,am#        "^        #mw,``"~.     ]H
                        K        -^`                          `"-        K
                       jN                                                ]H
                       #                                                  N
                       K                                                  K
                       K                .  ,w#KKW,"KNw  ,-                $
                      jN                  *KKKR*""M YKKM                  ]H
                      ]N            .       ]N      #M       .            ]H
                      ]H             `*Ww,,,]K      0W,,,w#M`             ]H
                      ]H                 KKK`        ]KKK                 ]N
                      ]H                 `KKN y,  ,w#KKR                  ]H
                      ]N                   "KKw`9KKKKR^                   ]H
                      ,K                      ``"                         K
                ,,w#KKKKN                                                #
          ,w#KKKKKKKRM"``N                                             ,#`
       ,#KKKRM"``         YW                                          #M
     ,KKR"                  YW,                                    ,dM
    ]KK`                      `RNw                             ,w#M`
    KKH                       gM` `"RWw,,               ,,,g#M"` `9w
    1KN                      K      ,#` ``""***MMMM***""`` `N       K
     "KKw                    `9WmmmR"                        "Nmmm#M`
       `*KKNmw,
            `""*MRRKKKN##mmmmmmmm<<<<<=-.


  ____   ____  _ ___ ___
/' _| `v' /  \| | __| _ \
`._`.`. .'| | ' | _|| v /
|___/ !_! |_|\__|___|_|_\
 __ ____   __ ____  _  __
|  V  \ `v' // _/ || |/  \
| \_/ |`. .'| \_| >< | /\ |
|_| |_| !_!  \__/_||_|_||_|
 *
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_otg.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "fonts.h"
#include "ssd1306.h"
#include "synermycha-logo.h"
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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_I2C3_Init();
  MX_SPI1_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM9_Init();
  MX_TIM12_Init();
  MX_USART1_UART_Init();
  MX_USB_OTG_FS_PCD_Init();
  /* USER CODE BEGIN 2 */
  SSD1306_Init();  // initialise
  SSD1306_Clear();
  SSD1306_DrawBitmap(0,0,logo, 128, 64, SSD1306_COLOR_WHITE);
  SSD1306_GotoXY(36,53);
  SSD1306_Puts ("Starting", &Font_7x10, SSD1306_COLOR_WHITE);
  SSD1306_UpdateScreen();
  /* USER CODE END 2 */
 
 

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
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLRCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_TIM|RCC_PERIPHCLK_CLK48;
  PeriphClkInitStruct.PLLSAI.PLLSAIM = 8;
  PeriphClkInitStruct.PLLSAI.PLLSAIN = 96;
  PeriphClkInitStruct.PLLSAI.PLLSAIQ = 2;
  PeriphClkInitStruct.PLLSAI.PLLSAIP = RCC_PLLSAIP_DIV4;
  PeriphClkInitStruct.PLLSAIDivQ = 1;
  PeriphClkInitStruct.TIMPresSelection = RCC_TIMPRES_ACTIVATED;
  PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48CLKSOURCE_PLLSAIP;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
