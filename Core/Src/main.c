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
#include "adc.h"
#include "app_subghz_phy.h"
#include "dma.h"
#include "gpio.h"
#include "rtc.h"
#include "tim.h"
#include "usart.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "iwdg.h"
#include "stm32wlxx.h"
#include "subghz_phy_app.h"

#include "user_mqttFunctions.h"
#include "user_system.h"
#include "user_wifi.h"

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
HAL_StatusTypeDef uart2status = HAL_OK;
int32_t timer2sStatus = 0;
int32_t timer5sStatus = 0;
int32_t timer20sStatus = 0;
int32_t dataOkStatus = 0;
int32_t loraRecieveOkStatus = 0;
uint8_t mqttMessageCounter = 0;
uint8_t loraGetMsgCounter = 0;
uint8_t debugDifference = 0;
bool sendMqttData = false;
bool systemResetFlag = false;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int _write(int fd, unsigned char *buf, int len) {
  if (fd == 1 || fd == 2) { // stdout or stderr ?
    // HAL_UART_Transmit(&huart1, buf, len, 999); // Print to the UART
    uart2status =
        HAL_UART_Transmit(&huart1, buf, len, 999); // Print to the UART
                                                   // Orange wire => RX2
                                                   // Yellow wire => TX2
  }
  return len;
}

uint64_t mg_millis(void) { return HAL_GetTick(); }

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick.
   */
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
  MX_USART1_UART_Init();
  MX_RTC_Init();
  MX_SubGHz_Phy_Init();
  MX_USART2_UART_Init();
  MX_ADC_Init();
  MX_TIM2_Init();
  MX_LPUART1_UART_Init();
  /* USER CODE BEGIN 2 */

  /* Init ticks */
  HAL_TIM_Base_Start_IT(&htim2);
  initDelayCustomTimer();
  /* Long delay is given to spot if the gate way got reset */
  // Delay_CustomTimer(40000);
  timer2sStatus = getTick_CustomTimer_Sec();
  timer5sStatus = getTick_CustomTimer_Sec();
  timer20sStatus = getTick_CustomTimer_Sec();
  dataOkStatus = getTick_CustomTimer_Sec();
  loraRecieveOkStatus = getTick_CustomTimer_Sec();

  printf("\r\n\r\n**********IOT Gateway**********\r\n\r\n");
  initUart();
  Delay_CustomTimer(3000);

  // initWifi();
  // startServer();
  // Delay_CustomTimer(1000);
  // setGotHttpRequest(false);

  /* TODO: Replace While loop with button based logic 
    need to save the topics in some where in the memory
    */
  // while (true) {
  //   serveHttpRequest();
  // }

  // setLastCommandOK(true);
  Delay_CustomTimer(1000);
  sendATCommand("AT", "", AT_OK, NO_AT);

  gsmInit();
  initSensorFilter();
  setMqttTopic();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  printf("\r\n\r\n**********Main Loop**********\r\n\r\n");

  /* Init IWDG */
  initIwdg();

  while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    listenForLoraNodes();

    if (isLoraReceived()) {
      processIncomingLoraMessage();
      setLoraReceived(false);
    }

    /* Run filter in Data received through LoRa. rawDataReceived flag is set in
     * the LoRa interrupt callback OnRxDone() */
    if (isRawDataReceived()) {
      runAllFilter();
      setRawDataReceived(false);
      loraRecieveOkStatus = getTick_CustomTimer_Sec();
    }

    if (getTick_CustomTimer_Sec() - timer20sStatus >
        MQTT_DATA_SEND_MAIN_INTERVAL) {
      sendMqttData = true;
      timer20sStatus = getTick_CustomTimer_Sec();
    }

    if (sendMqttData) {
      if (getTick_CustomTimer_Sec() - timer5sStatus > MQTT_DATA_SEND_INTERVAL) {
        prepareMqttMessageStruct(mqttMessageCounter);
        if (sendDataToMqttBroker(mqttMessageCounter) != NO_ERROR) {
          printf("Error sending data to MQTT Broker\r\n");
        } else {
          dataOkStatus = getTick_CustomTimer_Sec();
        }

        /* MQTT Send counter updated based on NUMBER_OF_SENSORS */
        mqttMessageCounter++;
        mqttMessageCounter = mqttMessageCounter % NUMBER_OF_SENSORS;

        /* Update last tick */
        timer5sStatus = getTick_CustomTimer_Sec();
      }
      if (mqttMessageCounter == 0) {
        sendMqttData = false;
      }
    }

    if (getTick_CustomTimer_Sec() - dataOkStatus > MAX_TIME_CANNOT_SEND_MQTT) {
      printf("**MQTT data cannot be send for 2 min**\r\n");
      printf("**Please check Network and SIM card.**\r\n");
      systemResetFlag = true;
    }

    if (getTick_CustomTimer_Sec() - loraRecieveOkStatus >
        MAX_TIME_LORA_INCOMING_MISSING) {
      printf("**LoRa data not receiving for 5 min**\r\n");
      printf("**\r\nPlease check End nodes. Atleast one End node should be "
             "responding**\r\n");
      systemResetFlag = true;
    }

    if (getTick_CustomTimer_Sec() - timer2sStatus > 1) {
      getDataFromEndNode(sensorID_0 + loraGetMsgCounter, getSensorType());
      loraGetMsgCounter++;
      loraGetMsgCounter = loraGetMsgCounter % NUMBER_OF_SENSORS;

      timer2sStatus = getTick_CustomTimer_Sec();
    }

    if (systemResetFlag) {
      printf("\r\n*****************************\r\n");
      printf("\r\n**System Reset after 10 sec**\r\n");
      printf("\r\n*****************************\r\n");
      Delay_CustomTimer(10000);
      NVIC_SystemReset();
    }
    setLastCommandOK(true);
    cleanAllBuffers();

    refreshIwdg();
  }
  /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
   */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_OscInitStruct.OscillatorType =
      RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS_PWR;
  RCC_OscInitStruct.LSIDiv = RCC_LSI_DIV1;
  RCC_OscInitStruct.HSEDiv = RCC_HSE_DIV1;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    Error_Handler();
  }

  /** Configure the SYSCLKSource, HCLK, PCLK1 and PCLK2 clocks dividers
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK3 | RCC_CLOCKTYPE_HCLK |
                                RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 |
                                RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.AHBCLK3Divider = RCC_SYSCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK) {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void resetAllTimerState(void) {
  loraRecieveOkStatus = getTick_CustomTimer_Sec();
  dataOkStatus = getTick_CustomTimer_Sec();
}
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1) {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line) {
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line
     number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
