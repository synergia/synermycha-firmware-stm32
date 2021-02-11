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
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "fonts.h"
#include "ssd1306.h"
#include "synermycha-logo.h"
#include <stdio.h>
#include "string.h"
#include "VL53L0X.h"
#include "led.h"
#include "buzzer.hpp"
#include "mario_theme.hpp"
#include "usbd_cdc_if.h"
#include <stdlib.h>
#include "ws2812b.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define RIGHT_MOTOR_BACKWARD HAL_GPIO_WritePin(DRV8835_DIR_A_GPIO_Port, DRV8835_DIR_A_Pin, GPIO_PIN_SET);
#define RIGHT_MOTOR_FORWARD HAL_GPIO_WritePin(DRV8835_DIR_A_GPIO_Port, DRV8835_DIR_A_Pin, GPIO_PIN_RESET);
#define LEFT_MOTOR_BACKWARD	HAL_GPIO_WritePin(DRV8835_DIR_B_GPIO_Port, DRV8835_DIR_B_Pin, GPIO_PIN_RESET);
#define LEFT_MOTOR_FORWARD	HAL_GPIO_WritePin(DRV8835_DIR_B_GPIO_Port, DRV8835_DIR_B_Pin, GPIO_PIN_SET);
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
VL53L0X sensorL,sensorFL,sensorF,sensorFR,sensorR;
uint16_t distanceMeasured[5];
uint8_t DataToSend[20]; // Tablica zawierajaca dane do wyslania
uint8_t MessageCounter = 0; // Licznik wyslanych wiadomosci
uint8_t MessageLength = 0; // Zawiera dlugosc wysylanej wiadomosci
char pomiar_string[5][15];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
  if(htim->Instance == TIM14){ // Jeżeli przerwanie pochodzi od timera 14
    static bool colour = 0;
    uint16_t PomiarADC = 0;
    if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK)
    {
      PomiarADC = HAL_ADC_GetValue(&hadc1);
      HAL_ADC_Start(&hadc1);
    }
    SSD1306_DrawFilledRectangle(112,2,12,4,SSD1306_COLOR_BLACK);
    if(PomiarADC/340 > 0){
      SSD1306_DrawRectangle(110,0,16,8,SSD1306_COLOR_WHITE);
      SSD1306_DrawLine(127,3,127,5,SSD1306_COLOR_WHITE);
      SSD1306_DrawFilledRectangle(112,2,PomiarADC/340,4,SSD1306_COLOR_WHITE);
    }
    else
    {
      SSD1306_DrawRectangle(110,0,16,8,colour ? SSD1306_COLOR_WHITE : SSD1306_COLOR_BLACK );
      SSD1306_DrawLine(127,3,127,5,colour ? SSD1306_COLOR_WHITE : SSD1306_COLOR_BLACK);
      colour = !colour;
    }
  }
}

void setup_OLED(void){
  SSD1306_Init();  // initialise
  SSD1306_Clear();
  SSD1306_DrawBitmap(0,0,logo, 128, 64, SSD1306_COLOR_WHITE);
  SSD1306_GotoXY(36,53);
  SSD1306_Puts ((char*)"Starting", &Font_7x10, SSD1306_COLOR_WHITE);
  SSD1306_UpdateScreen();
}

/* USER CODE END PFP */
void debugPrint(UART_HandleTypeDef *huart, char _out[])
{ 
  HAL_UART_Transmit(huart, (uint8_t *) _out, strlen(_out)-1, 10); 
}
void debugPrintln(UART_HandleTypeDef *huart, char _out[])
{
  HAL_UART_Transmit(huart, (uint8_t *) _out, strlen(_out)-1, 10); 
  char newline[3] = "\r\n"; 
  HAL_UART_Transmit(huart, (uint8_t *) newline, 2, 10); 
}

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
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_I2C3_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM9_Init();
  MX_TIM12_Init();
  MX_USART1_UART_Init();
  MX_TIM14_Init();
  MX_USB_DEVICE_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
  uint8_t Received[3];

  setup_led();
  setup_OLED();
  // HAL_Delay(1100);
  // debugPrint(&huart1,"$$$");
  // HAL_Delay(1100);
  // HAL_UART_Receive_IT(&huart1, Received, 3);
  // CDC_Transmit_FS(Received, 3);
  // debugPrint(&huart1,"SN,Synermycha\r");
  // HAL_Delay(10);
  // debugPrint(&huart1,"R,1\r");
  // HAL_Delay(10);

  HAL_ADC_Start(&hadc1);

  HAL_NVIC_EnableIRQ(TIM8_TRG_COM_TIM14_IRQn);
  HAL_TIM_Base_Start_IT(&htim14);
  HAL_GPIO_WritePin(DRV8835_EN_GPIO_Port, DRV8835_EN_Pin, GPIO_PIN_SET);
  HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_2);
  TIM12->CCR1 = 0;
  TIM12->CCR2 = 0;
  HAL_GPIO_WritePin(DRV8835_DIR_A_GPIO_Port, DRV8835_DIR_A_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(DRV8835_DIR_B_GPIO_Port, DRV8835_DIR_B_Pin, GPIO_PIN_SET);

  HAL_GPIO_WritePin(RN4871_NRESET_GPIO_Port, RN4871_NRESET_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(RN4871_NON_GPIO_Port, RN4871_NON_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(RN4871_NFLASH_MODE_GPIO_Port, RN4871_NFLASH_MODE_Pin, GPIO_PIN_SET);

  HAL_GPIO_WritePin(VC53L0x_XSHUT_LEFT_GPIO_Port, VC53L0x_XSHUT_LEFT_Pin,GPIO_PIN_RESET);
  HAL_GPIO_WritePin(VC53L0x_XSHUT_FRONT_LEFT_GPIO_Port, VC53L0x_XSHUT_FRONT_LEFT_Pin,GPIO_PIN_RESET);
  HAL_GPIO_WritePin(VC53L0x_XSHUT_FRONT_GPIO_Port, VC53L0x_XSHUT_FRONT_Pin,GPIO_PIN_RESET);
  HAL_GPIO_WritePin(VC53L0x_XSHUT_FRONT_RIGHT_GPIO_Port, VC53L0x_XSHUT_FRONT_RIGHT_Pin,GPIO_PIN_RESET);
  HAL_GPIO_WritePin(VC53L0x_XSHUT_RIGHT_GPIO_Port, VC53L0x_XSHUT_RIGHT_Pin,GPIO_PIN_RESET);
  HAL_Delay(20);

  // WS2812B_Init(&hspi1);
  // WS2812B_Refresh();
  // HAL_Delay(100);
  // WS2812B_SetDiodeRGB(0,255,0,0);
  // WS2812B_SetDiodeRGB(1,255,127,0);
  // WS2812B_SetDiodeRGB(2,255,255,0);
  // WS2812B_SetDiodeRGB(3,127,255,0);
  // WS2812B_SetDiodeRGB(4,0,255,0);
  // WS2812B_SetDiodeRGB(5,0,255,127);
  // WS2812B_SetDiodeRGB(6,0,255,255);
  // WS2812B_SetDiodeRGB(7,0,127,255);
  // WS2812B_SetDiodeRGB(8,0,0,255);
  // WS2812B_SetDiodeRGB(9,255,0,255);
  // WS2812B_Refresh();
  // HAL_Delay(100);
  // WS2812B_SetDiodeRGB(10,0,0,0);
  // WS2812B_SetDiodeRGB(1,255,255,255);
  // WS2812B_SetDiodeRGB(2,255,255,255);
  // WS2812B_SetDiodeRGB(3,255,255,255);
  // WS2812B_SetDiodeRGB(4,255,255,255);
  // WS2812B_SetDiodeRGB(5,255,255,255);
  // WS2812B_SetDiodeRGB(6,255,255,255);
  // WS2812B_SetDiodeRGB(7,255,255,255);
  // WS2812B_SetDiodeRGB(8,255,255,255);
  // WS2812B_SetDiodeRGB(9,255,255,255);
  // WS2812B_SetDiodeRGB(1,255,255,255);
  // WS2812B_Refresh();


  setup_VL53L0X(&sensorL);
  HAL_GPIO_WritePin(VC53L0x_XSHUT_LEFT_GPIO_Port, VC53L0x_XSHUT_LEFT_Pin,GPIO_PIN_SET);
  HAL_Delay(10);
  init(&sensorL,true);
  setAddress(&sensorL, 0b0101010);
  setTimeout(&sensorL,35);
  startContinuous(&sensorL,33);

  setup_VL53L0X(&sensorFL);
  HAL_GPIO_WritePin(VC53L0x_XSHUT_FRONT_LEFT_GPIO_Port, VC53L0x_XSHUT_FRONT_LEFT_Pin,GPIO_PIN_SET);
  HAL_Delay(10);
  init(&sensorFL,true);
  setAddress(&sensorFL, 0b0101011);
  setTimeout(&sensorFL,35);
  startContinuous(&sensorFL,33);

  setup_VL53L0X(&sensorF);
  HAL_GPIO_WritePin(VC53L0x_XSHUT_FRONT_GPIO_Port, VC53L0x_XSHUT_FRONT_Pin,GPIO_PIN_SET);
  HAL_Delay(10);
  init(&sensorF,true);
  setAddress(&sensorF, 0b0101100);
  setTimeout(&sensorF,35);
  startContinuous(&sensorF,33);

  setup_VL53L0X(&sensorFR);
  HAL_GPIO_WritePin(VC53L0x_XSHUT_FRONT_RIGHT_GPIO_Port, VC53L0x_XSHUT_FRONT_RIGHT_Pin,GPIO_PIN_SET);
  HAL_Delay(10);
  init(&sensorFR,true);
  setAddress(&sensorFR, 0b0101101);
  setTimeout(&sensorFR,35);
  startContinuous(&sensorFR,33);

  setup_VL53L0X(&sensorR);
  HAL_GPIO_WritePin(VC53L0x_XSHUT_RIGHT_GPIO_Port, VC53L0x_XSHUT_RIGHT_Pin,GPIO_PIN_SET);
  HAL_Delay(10);
  init(&sensorR,true);
  setAddress(&sensorR, 0b0101111);
  setTimeout(&sensorR,35);
  startContinuous(&sensorR,33);

  HAL_GPIO_WritePin(RN4871_NON_GPIO_Port, RN4871_NON_Pin, GPIO_PIN_RESET);

  SSD1306_Clear();
  SSD1306_UpdateScreen();
  /* USER CODE END 2 */



  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

    // if (HAL_GPIO_ReadPin(BUTTON_OK_GPIO_Port, BUTTON_OK_Pin) == GPIO_PIN_RESET) {
    //   ++MessageCounter;
		// 	MessageLength = sprintf((char *)DataToSend, "Wiadomosc nr %d\n\r", MessageCounter);
		// 	CDC_Transmit_FS(DataToSend, MessageLength);
    //   HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, GPIO_PIN_SET);
    //   while (HAL_GPIO_ReadPin(BUTTON_OK_GPIO_Port, BUTTON_OK_Pin) == GPIO_PIN_RESET)
    //   {
        
    //   }
    //   HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, GPIO_PIN_RESET);
		// }
    // if (HAL_GPIO_ReadPin(BUTTON_DOWN_GPIO_Port, BUTTON_DOWN_Pin) == GPIO_PIN_RESET) {
		// 	MessageLength = sprintf((char *)DataToSend, "Wiadomosc nr %d\n\r", MessageCounter);
		// 	CDC_Transmit_FS(DataToSend, MessageLength);
    //   HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, GPIO_PIN_SET);
    //   while (HAL_GPIO_ReadPin(BUTTON_DOWN_GPIO_Port, BUTTON_DOWN_Pin) == GPIO_PIN_RESET)
    //   {
        
    //   }
    //   HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, GPIO_PIN_RESET);
		// }
    // if (HAL_GPIO_ReadPin(BUTTON_UP_GPIO_Port, BUTTON_UP_Pin) == GPIO_PIN_RESET) {
    //   --MessageCounter;
		// 	MessageLength = sprintf((char *)DataToSend, "Wiadomosc nr %d\n\r", MessageCounter);
		// 	CDC_Transmit_FS(DataToSend, MessageLength);
    //   HAL_GPIO_WritePin(LED_3_GPIO_Port, LED_3_Pin, GPIO_PIN_SET);
    //   while (HAL_GPIO_ReadPin(BUTTON_UP_GPIO_Port, BUTTON_UP_Pin) == GPIO_PIN_RESET)
    //   {
        
    //   }
    //   HAL_GPIO_WritePin(LED_3_GPIO_Port, LED_3_Pin, GPIO_PIN_RESET);
		// }
    
    distanceMeasured[0]=readRangeContinuousMillimeters(&sensorL);
    if(timeoutOccurred(&sensorL))
      startContinuous(&sensorL,33);
    distanceMeasured[1]=readRangeContinuousMillimeters(&sensorFL);
    if(timeoutOccurred(&sensorFL))
      startContinuous(&sensorFL,33);
    distanceMeasured[2]=readRangeContinuousMillimeters(&sensorF);
    if(timeoutOccurred(&sensorF))
      startContinuous(&sensorF,33);
    distanceMeasured[3]=readRangeContinuousMillimeters(&sensorFR);
    if(timeoutOccurred(&sensorFR))
      startContinuous(&sensorFR,33);
    distanceMeasured[4]=readRangeContinuousMillimeters(&sensorR);
    if(timeoutOccurred(&sensorR))
      startContinuous(&sensorR,33);

    sprintf(pomiar_string[0],"L  dist: %05d",distanceMeasured[0]);
    SSD1306_GotoXY(0,0);
    SSD1306_Puts (pomiar_string[0], &Font_7x10, SSD1306_COLOR_WHITE);
    sprintf(pomiar_string[1],"FL dist: %05d",distanceMeasured[1]);
    SSD1306_GotoXY(0,11);
    SSD1306_Puts (pomiar_string[1], &Font_7x10, SSD1306_COLOR_WHITE);
    sprintf(pomiar_string[2],"F  dist: %05d",distanceMeasured[2]);
    SSD1306_GotoXY(0,22);
    SSD1306_Puts (pomiar_string[2], &Font_7x10, SSD1306_COLOR_WHITE);
    sprintf(pomiar_string[3],"FR dist: %05d",distanceMeasured[3]);
    SSD1306_GotoXY(0,33);
    SSD1306_Puts (pomiar_string[3], &Font_7x10, SSD1306_COLOR_WHITE);
    sprintf(pomiar_string[4],"R  dist: %05d",distanceMeasured[4]);
    SSD1306_GotoXY(0,44);
    SSD1306_Puts (pomiar_string[4], &Font_7x10, SSD1306_COLOR_WHITE);

    SSD1306_UpdateScreen();
    HAL_Delay(35);
    // sprintf((char*)DataToSend,"%05d\t%05d\t%05d",distanceMeasured[0],distanceMeasured[1],distanceMeasured[2]);
    // debugPrintln(&huart1,(char*)DataToSend);
    debugPrint(&huart1,"SynerMycha wita BLE\r\n");

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
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
