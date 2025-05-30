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
#include "subghz_phy_app.h"
#include "platform.h"
#include "radio.h"
#include "sys_app.h"

/* USER CODE BEGIN Includes */
#include "radio_driver.h"
#include "user_system.h"
/* USER CODE END Includes */

/* External variables
 * ---------------------------------------------------------*/
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
static volatile uint8_t BufferTx[LORA_TX_BUFFER_SIZE + 1];
static volatile uint8_t BufferRx[LORA_RX_BUFFER_SIZE + 1];

static volatile bool loraReceived = false;

uint16_t temperature = 0;
uint16_t batLevel = 0;

radio_status_t status = RADIO_STATUS_ERROR;
RadioOperatingModes_t mode = MODE_SLEEP;

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
static void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi,
                     int8_t LoraSnr_FskCfo);

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

/* Exported functions
 * ---------------------------------------------------------*/
void SubghzApp_Init(void) {
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
                    LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON, false, 0,
                    0, LORA_IQ_INVERSION_ON, 3000);

  Radio.SetRxConfig(MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                    LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                    LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON, 0, false,
                    0, 0, LORA_IQ_INVERSION_ON, true);

  Radio.SetMaxPayloadLength(MODEM_LORA, 255);

  /* USER CODE END SubghzApp_Init_2 */
}

/* USER CODE BEGIN EF */

/* USER CODE END EF */

/* Private functions ---------------------------------------------------------*/
static void OnTxDone(void) {
  /* USER CODE BEGIN OnTxDone */
  // printf("LORA: OnTxDone(void) : Called\r\n");

  /* USER CODE END OnTxDone */
}

static void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi,
                     int8_t LoraSnr_FskCfo) {
  /* USER CODE BEGIN OnRxDone */
  // printf("\r\n===> OnRxDone(void)\r\n");

  /* Clear BufferRx*/
  memset(BufferRx, 0, LORA_RX_BUFFER_SIZE);

  /* Record payload size*/
  memcpy(BufferRx, payload, LORA_RX_BUFFER_SIZE);
  setLoraReceived(true);
  // printf("LORA: BufferRx (%i): %s\r\n", size, BufferRx);

  /* Check buffer */

  // printf("Received Payload Size : %i\r\n", RxBufferSize);
  // printf("LORA: Received Payload : %i\r\n", BufferRx[0]);

  // printf("<=== OnRxDone(void)\r\n\r\n");

  /* USER CODE END OnRxDone */
}

static void OnTxTimeout(void) {
  /* USER CODE BEGIN OnTxTimeout */
  /* USER CODE END OnTxTimeout */
}

static void OnRxTimeout(void) {
  /* USER CODE BEGIN OnRxTimeout */
  /* USER CODE END OnRxTimeout */
}

static void OnRxError(void) {
  /* USER CODE BEGIN OnRxError */
  // printf("LORA: OnRxError(void) :\r\n");
  /* USER CODE END OnRxError */
}

/* USER CODE BEGIN PrFD */
void getDataFromEndNode(SensorId sensorID, uint8_t sensorType) {
  // HAL_Delay(Radio.GetWakeupTime() + 200);

  char *loraMessage_p = prepareLoraMessage(sensorID, sensorType);
  status = Radio.Send(loraMessage_p, LORA_TX_BUFFER_SIZE);
  /* After sending any length of LORA message, MAX_PAYLOAD_LENGTH updates with
   * the same for receiving. Hence need to reset with following code. */
  Radio.SetMaxPayloadLength(MODEM_LORA, LORA_RX_BUFFER_SIZE);

  printf("\r\nLORA: TO END NODE: %s\r\n", loraMessage_p);
}

void listenForLoraNodes(void) {
  if (Radio.GetStatus() == RF_IDLE) {
    Radio.Rx(LORA_LISTENING_DURATION);
    // printf("LORA: Radio.Rx() : RF_IDLE\r\n");
  }
}

void processIncomingLoraMessage(void) {
  char *message;
  SoilSensorData_t *soilSensorData_p;

  message = strtok(BufferRx, LORA_MESSAGE_DELIMITER);
  if (strstr(message, IOT_GATEWAY_KEY) != NULL) {
    message = strtok(NULL, LORA_MESSAGE_DELIMITER);
    uint8_t sID = atoi(message);
    if (sID >= sensorID_0 && sID < (sensorID_0 + NUMBER_OF_SENSORS)) {
      soilSensorData_p = getSoilSensorData();

      soilSensorData_p->temperature = atoi(strtok(NULL, LORA_MESSAGE_COMMA));
      soilSensorData_p->moisture = atoi(strtok(NULL, LORA_MESSAGE_COMMA)); 
      soilSensorData_p->conductivity = atoi(strtok(NULL, LORA_MESSAGE_COMMA));
      soilSensorData_p->pH = atoi(strtok(NULL, LORA_MESSAGE_COMMA));
      soilSensorData_p->nitrogen = atoi(strtok(NULL, LORA_MESSAGE_COMMA));
      soilSensorData_p->phosphorus = atoi(strtok(NULL, LORA_MESSAGE_COMMA));
      soilSensorData_p->potassium = atoi(strtok(NULL, LORA_MESSAGE_COMMA));

      printf("\r\nSoil Sensor Data:\r\n");
      printf("Temperature  : %d °C\r\n", soilSensorData_p->temperature);
      printf("Moisture     : %d %%\r\n", soilSensorData_p->moisture);
      printf("Conductivity : %d uS/cm\r\n", soilSensorData_p->conductivity);
      printf("pH           : %d\r\n", soilSensorData_p->pH);
      printf("Nitrogen     : %d mg/kg\r\n", soilSensorData_p->nitrogen);
      printf("Phosphorus   : %d mg/kg\r\n", soilSensorData_p->phosphorus);
      printf("Potassium    : %d mg/kg\r\n", soilSensorData_p->potassium);

      uint8_t sValue = atoi(message);
      printf("Correct Message :-) %i\r\n", sValue);
      setNewValueBuffer(sValue, sID);
      setRawDataReceived(true);
    }
  } else {
    printf("END NODE Key mismatch :-(\r\n");
  }
}
void setLoraReceived(bool status) { loraReceived = status; }

bool isLoraReceived(void) { return loraReceived; }

/* USER CODE END PrFD */
