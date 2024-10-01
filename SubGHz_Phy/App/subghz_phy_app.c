/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    subghz_phy_app.c
 * @author  MCD Application Team
 * @brief   Application of the SubGHz_Phy Middleware
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
#include "platform.h"
#include "sys_app.h"
#include "subghz_phy_app.h"
#include "radio.h"
#include "cmsis_os.h"

/* USER CODE BEGIN Includes */
#include "radio_driver.h"
#include "system.h"
/* USER CODE END Includes */

/* External variables ---------------------------------------------------------*/
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define PING "PING"

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* Radio events function pointer */
static RadioEvents_t RadioEvents;

/* USER CODE BEGIN PV */
static uint8_t BufferTx[MAX_APP_BUFFER_SIZE];
static uint8_t BufferRx[MAX_APP_BUFFER_SIZE];

uint16_t temperature = 0;
uint16_t batLevel = 0;

/* Last  Received Buffer Size*/
uint16_t RxBufferSize = 0;

radio_status_t status = RADIO_STATUS_ERROR;
RadioOperatingModes_t mode = MODE_SLEEP;

uint8_t increaseValue = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/*!
 * @brief Function to be executed on Radio Tx Done event
 */
static void OnTxDone(void);

/**
  * @brief Function to be executed on Radio Rx Done event
  * @param  payload ptr of buffer received
  * @param  size buffer size
  * @param  rssi
  * @param  LoraSnr_FskCfo
  */
static void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t LoraSnr_FskCfo);

/**
  * @brief Function executed on Radio Tx Timeout event
  */
static void OnTxTimeout(void);

/**
  * @brief Function executed on Radio Rx Timeout event
  */
static void OnRxTimeout(void);

/**
  * @brief Function executed on Radio Rx Error event
  */
static void OnRxError(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Exported functions ---------------------------------------------------------*/
void SubghzApp_Init(void)
{
  /* USER CODE BEGIN SubghzApp_Init_1 */

  /* USER CODE END SubghzApp_Init_1 */

  /* Radio initialization */
  RadioEvents.TxDone = OnTxDone;
  RadioEvents.RxDone = OnRxDone;
  RadioEvents.TxTimeout = OnTxTimeout;
  RadioEvents.RxTimeout = OnRxTimeout;
  RadioEvents.RxError = OnRxError;

  Radio.Init(&RadioEvents);

  /* USER CODE BEGIN SubghzApp_Init_2 */
  static int32_t random_delay;
  random_delay =
      (Radio.Random()) >> 22; /*10bits random e.g. from 0 to 1023 ms*/

  /* Radio Set frequency */
  Radio.SetChannel(RF_FREQUENCY);

  Radio.SetTxConfig(MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                    LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                    LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON, true, 0,
                    0, LORA_IQ_INVERSION_ON, 3000);

  Radio.SetRxConfig(MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                    LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                    LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON, 0, true, 0,
                    0, LORA_IQ_INVERSION_ON, true);

  Radio.SetMaxPayloadLength(MODEM_LORA, 255);

  /* USER CODE END SubghzApp_Init_2 */
}

/* USER CODE BEGIN EF */

/* USER CODE END EF */

/* Private functions ---------------------------------------------------------*/
static void OnTxDone(void)
{
  /* USER CODE BEGIN OnTxDone */
  printf("OnTxDone(void) :\r\n");

  /* USER CODE END OnTxDone */
}

static void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t LoraSnr_FskCfo)
{
  /* USER CODE BEGIN OnRxDone */
  // printf("\r\n===> OnRxDone(void)\r\n");

  /* Clear BufferRx*/
  memset(BufferRx, 0, MAX_APP_BUFFER_SIZE);

  /* Record payload size*/
  RxBufferSize = MAX_APP_BUFFER_SIZE;
  if (RxBufferSize <= MAX_APP_BUFFER_SIZE) {
    memcpy(BufferRx, payload, RxBufferSize);
    setNewValueBuffer(BufferRx[0]);
    setRawDataReceived(true);
  }

  // printf("Received Payload Size : %i\r\n", RxBufferSize);
  printf("PL : %i\r\n", BufferRx[0]);
  // printArray(BufferRx, RxBufferSize);

  // printf("<=== OnRxDone(void)\r\n\r\n");

  /* USER CODE END OnRxDone */
}

static void OnTxTimeout(void)
{
  /* USER CODE BEGIN OnTxTimeout */
  /* USER CODE END OnTxTimeout */
}

static void OnRxTimeout(void)
{
  /* USER CODE BEGIN OnRxTimeout */
  /* USER CODE END OnRxTimeout */
}

static void OnRxError(void)
{
  /* USER CODE BEGIN OnRxError */
  printf("OnRxError(void) :\r\n");
  /* USER CODE END OnRxError */
}

/* USER CODE BEGIN PrFD */
void pingPong(void) {
  HAL_Delay(Radio.GetWakeupTime() + 200);
  osDelay(2000);
  temperature = GetTemperatureLevel();
  // batLevel = SYS_GetBatteryLevel();

  printf("Temperature : %i\r\n", temperature);
  // printf("Battery Level : %i\r\n", batLevel);
  BufferTx[0] = temperature;
  BufferTx[1] = temperature >> 8;
  status = Radio.Send(BufferTx, MAX_APP_BUFFER_SIZE);
  printf("Radio.Tx() : %i\r\n", status);
}

void listenForLoraNodes(void) {
  if (Radio.GetStatus() == RF_IDLE) {
    Radio.Rx(LORA_LISTENING_DURATION);
    printf("Radio.Rx() : RF_IDLE\r\n");
  }
}

/* USER CODE END PrFD */
