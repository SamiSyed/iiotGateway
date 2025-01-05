/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : app_freertos.c
 * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

#include "app_subghz_phy.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "UartRingbuffer_multi.h"
#include "filter.h"
#include "subghz.h"
#include "system.h"
#include "usart.h"
#include "user_mqttFunctions.h"

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
/* USER CODE BEGIN Variables */
uint8_t debugCounter = 0;

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for runFilter */
osThreadId_t runFilterHandle;
const osThreadAttr_t runFilter_attributes = {
  .name = "runFilter",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for gmsTask */
osThreadId_t gmsTaskHandle;
const osThreadAttr_t gmsTask_attributes = {
  .name = "gmsTask",
  .priority = (osPriority_t) osPriorityHigh,
  .stack_size = 300 * 4
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void runFilterEntry(void *argument);
void gsmTaskEntry(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of runFilter */
  runFilterHandle = osThreadNew(runFilterEntry, NULL, &runFilter_attributes);

  /* creation of gmsTask */
  gmsTaskHandle = osThreadNew(gsmTaskEntry, NULL, &gmsTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* init code for SubGHz_Phy */
  MX_SubGHz_Phy_Init();
  /* USER CODE BEGIN StartDefaultTask */
  initUart();
  cleanAllBuffers();

  if (IS_GATEWAY == 1) {
    printf("*********GATEWAY*********\r\n");
  }
  if (IS_GATEWAY == 0) {
    printf("*********END_NODE*********\r\n");
  }

  if (IS_GATEWAY == 1) {
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    printf("----------------------------------Start fetching sensor data\r\n");
  }
  if (IS_GATEWAY == 0) {
    printf("Start Sending data\r\n");
  }
  /* Release filter task */

  /* Infinite loop */
  for (;;) {
    /* Lora test ===> */
    if (IS_GATEWAY == 0) {
      pingPong();
    }
    /* <===Lora test */

    /* Get Lora data ===> */
    if (IS_GATEWAY == 0) {
      listenForLoraNodes(LORA_LISTENING_DURATION);
    }
    /* <=== Get Lora data */
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_runFilterEntry */
/**
 * @brief
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_runFilterEntry */
void runFilterEntry(void *argument)
{
  /* USER CODE BEGIN runFilterEntry */
  if (IS_GATEWAY == 0) {
    osDelay(500);
    initSensorFilter();
    printf("----------------------------------Filter loop started\r\n");
  }
  /* Infinite loop */
  for (;;) {
    // printf("Tick\r\n");
    // printfMainBuffer();

    if (IS_GATEWAY == 1) {
      if (isRawDataReceived()) {
        runAllFilter();
        setRawDataReceived(false);
      }
    }
    osDelay(5000);
  }
  /* USER CODE END runFilterEntry */
}

/* USER CODE BEGIN Header_gsmTaskEntry */
/**
 * @brief Function implementing the gmsTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_gsmTaskEntry */
void gsmTaskEntry(void *argument)
{
  /* USER CODE BEGIN gsmTaskEntry */
  // osDelay(3000);
  setLastCommandOK(true);
  if (IS_GATEWAY == 1) {

    SystemError status = ERROR_NO_AT_REPLY;
    for (uint8_t i = 0; i < 5; i++) {
      setLastCommandOK(true);
      status = sendATCommand("AT","", AT_OK, NO_AT);
      if (status == NO_ERROR) {
        break;
      }
    }

    if (sendATCommand("AT", "", AT_OK, NO_AT) == NO_ERROR) {
      if (gsmInit() != NO_ERROR) {
        printf("GSM Init Error\r\n");
      } else {
        /* If gsmInit() is successful then release Task to send / receive data*/
        xTaskNotifyGive(defaultTaskHandle);
      }
    }
    setMqttTopic();
  }

  /* Infinite loop */
  for (;;) {
    if (IS_GATEWAY == 1) {
      prepareMqttMessageStruct(getMqttMessage());
      if (sendAllDataToMqttBroker(getMqttMessage()) != NO_ERROR) {
        printf("Error sending data to MQTT Broker\r\n");
      }
    }
    osDelay(5000);
  }
  /* USER CODE END gsmTaskEntry */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
