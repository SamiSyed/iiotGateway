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
#include "cmsis_os.h"
#include "main.h"
#include "task.h"

#include "app_subghz_phy.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "UartRingbuffer_multi.h"
#include "filter.h"
#include "subghz.h"
#include "system.h"
#include "usart.h"

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
    .priority = (osPriority_t)osPriorityNormal,
    .stack_size = 128 * 4};
/* Definitions for runFilter */
osThreadId_t runFilterHandle;
const osThreadAttr_t runFilter_attributes = {.name = "runFilter",
                                             .priority =
                                                 (osPriority_t)osPriorityNormal,
                                             .stack_size = 128 * 4};
/* Definitions for prepareData */
osThreadId_t prepareDataHandle;
const osThreadAttr_t prepareData_attributes = {
    .name = "prepareData",
    .priority = (osPriority_t)osPriorityNormal,
    .stack_size = 128 * 4};
/* Definitions for gmsTask */
osThreadId_t gmsTaskHandle;
const osThreadAttr_t gmsTask_attributes = {.name = "gmsTask",
                                           .priority =
                                               (osPriority_t)osPriorityHigh,
                                           .stack_size = 128 * 4};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void runFilterEntry(void *argument);
void prepareDataEntry(void *argument);
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
  defaultTaskHandle =
      osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of runFilter */
  runFilterHandle = osThreadNew(runFilterEntry, NULL, &runFilter_attributes);

  /* creation of prepareData */
  prepareDataHandle =
      osThreadNew(prepareDataEntry, NULL, &prepareData_attributes);

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
void StartDefaultTask(void *argument) {
  /* init code for SubGHz_Phy */
  MX_SubGHz_Phy_Init();
  /* USER CODE BEGIN StartDefaultTask */
  // uint16_t reading = 25;
  // HAL_StatusTypeDef status = HAL_ERROR;
  // printf("SUBGhz Init Done\r\n");
  if (IS_GATEWAY) {
    printf("*********GATEWAY*********\r\n");
  } else {
    printf("*********END_NODE*********\r\n");
  }
  osDelay(30000);
  /* Infinite loop */
  for (;;) {
    /* Lora test ===> */
    if (IS_GATEWAY == 0) {
      pingPong();
    }
    /* <===Lora test */

    /* Get Lora data ===> */
    if (IS_GATEWAY == 1) {
      listenForLoraNodes(LORA_LISTENING_DURATION);
      osDelay(1);
    }
    /* <=== Get Lora data */

    // MqttMessage_t *tempMqttMessage = getMqttMessageByIndex(sensorID_0);
    // printf("mqttTopic : %s\r\n", tempMqttMessage->topic);
    // printf("mqttValue : %i\r\n\r\n", tempMqttMessage->value);
    // osDelay(1000);
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
void runFilterEntry(void *argument) {
  /* USER CODE BEGIN runFilterEntry */
  initSensorFilter();
  /* Infinite loop */
  for (;;) {
    if (isRawDataReceived()) {
      runAllFilter();
      setRawDataReceived(false);
    }
    osDelay(1);
  }
  /* USER CODE END runFilterEntry */
}

/* USER CODE BEGIN Header_prepareDataEntry */
/**
 * @brief Function implementing the prepareData thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_prepareDataEntry */
void prepareDataEntry(void *argument) {
  /* USER CODE BEGIN prepareDataEntry */
  SystemError LocalError = NO_ERROR;
  setMqttTopic();
  /* Infinite loop */
  // if (isGSMReady()) {
  osDelay(30000);
  for (;;) {
    // Copy data
    // Prepare message
    prepareMqttMessageStruct(getMqttMessage());
    // put into variable or send
    LocalError = sendAllDataToMqttBroker(getMqttMessage());
    if (LocalError != NO_ERROR) {
      // Log Error
    }
    osDelay(1000);
  }
  // }
  /* USER CODE END prepareDataEntry */
}

/* USER CODE BEGIN Header_gsmTaskEntry */
/**
 * @brief Function implementing the gmsTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_gsmTaskEntry */
void gsmTaskEntry(void *argument) {
  /* USER CODE BEGIN gsmTaskEntry */
  if (IS_GATEWAY == 1) {
    // Ringbuf_init();
    osDelay(3000);
    SystemError localError = NO_ERROR;
    localError = atCommandCheck();

    if (localError != NO_ERROR) {
      // Log Error
    } else {
      localError = gsmInit();
    }

    if (localError != NO_ERROR) {
      // Log Error
    }
    /* Infinite loop */
    for (;;) {
      osDelay(1000);
      // gsmSend();
    }
  }
  /* USER CODE END gsmTaskEntry */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
