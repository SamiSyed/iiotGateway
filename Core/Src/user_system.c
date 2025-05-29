#include "../Inc/user_system.h"
#include "../Inc/user_gsm.h"
#include "adc_if.h"
#include "filter.h"
#include "main.h"
#include "stdio.h"
#include "tim.h"
#include "usart.h"
#include <stdlib.h>
#include <string.h>

/* Private variables */
static bool rawDataReceived = false;
static SystemError systemError = NO_ERROR;
static bool sendingData = false;
static bool receivingData = false;
static bool gsmReady = false;

/* This initially has to be true to fetch first AT Reply*/
bool lastCommandOK = true;
uint32_t timeStamp_timer = 0;

char loraMessage_TX[LORA_TX_BUFFER_SIZE + 1];

static uint16_t newValueBuffer[NUMBER_OF_SENSORS];
static uint16_t
    filteredSensorValues[NUMBER_OF_SENSORS]; /* Global Sensor Values*/
static MovingAverageFilter sensorFilters[NUMBER_OF_SENSORS];
static uint16_t filterBuffer[NUMBER_OF_SENSORS][FILTER_BUFFER_SIZE];
static Uart2Status_e uart2Status = UART2_RX_COMPLETE;
char p_command[MQTT_SEND_MESSAGE_SIZE];
char p_wifCommand[WIFI_SEND_MESSAGE_SIZE];

SoilSensorData_t soilSensorData = {0};

/* Sensor type variable */
uint8_t sensorType = EC_SENSOR;

uint8_t getSensorType(void) { return sensorType; }

/* Flag */
void setRawDataReceived(bool status) { rawDataReceived = status; }

bool isRawDataReceived(void) { return rawDataReceived; }

void setSendingFlag(bool flag) { sendingData = flag; }
bool getSendingFlag(void) { return sendingData; }

void setReceivingFlag(bool flag) { receivingData = flag; }
bool getReceivingFlag(void) { return receivingData; }

void setLastCommandOK(bool status) { lastCommandOK = status; }

bool isLastCommandOK(void) { return lastCommandOK; }

void setGMSReadinessFlag(bool status) {
  gsmReady = status;
}

bool isGSMReady(void) {
  return gsmReady;
}

SoilSensorData_t* getSoilSensorData(void) {
  return &soilSensorData;
}

/* Error */
void setSystemError(SystemError error) {
  /* Keep first error so it is not over written */
  if (systemError == NO_ERROR) {
    systemError = error;
  }
}

Uart2Status_e getUart2Status(void) { return uart2Status; }

void setUart2Status(Uart2Status_e status) { uart2Status = status; }

SystemError getSystemError(void) { return systemError; }

/* Filter */
SystemError initSensorFilter(void) {
  SystemError error = NO_ERROR;
  /* Create NUMBER_OF_SENSORS amount of filter */
  for (uint8_t sId = 0; sId < (uint8_t)NUMBER_OF_SENSORS; sId++) {
    error = MovingAverage_Init(&sensorFilters[sId], &filterBuffer[sId][0],
                               FILTER_BUFFER_SIZE);
    if (error != NO_ERROR) {
      break;
    }
  }

  return error;
}

void runAllFilter(void) {
  for (uint8_t sId = 0; sId < (uint8_t)NUMBER_OF_SENSORS; sId++) {
    filteredSensorValues[sId] =
        runFilter(&sensorFilters[sId], newValueBuffer[sId]);
  }
}

void setNewValueBuffer(uint16_t newValue, uint8_t sID) {
  sID = sID - sensorID_0;
  if (sID < NUMBER_OF_SENSORS) {
    newValueBuffer[sID] = newValue;
  }
}

/* Get value from global sensor array*/
uint16_t getFilteredValueByIndex(uint8_t index) {
  return filteredSensorValues[index];
}

uint16_t getSoilSensorTemperature(void) {
  return soilSensorData.temperature;
}

uint16_t getSoilSensorMoisture(void) {
  return soilSensorData.moisture;
}

uint16_t getSoilSensorConductivity(void) {
  return soilSensorData.conductivity;
}

uint16_t getSoilSensorPH(void) {
  return soilSensorData.pH;
}

uint16_t getSoilSensorNitrogen(void) {
  return soilSensorData.nitrogen;
}

uint16_t getSoilSensorPhosphorus(void) {
  return soilSensorData.phosphorus;
}

uint16_t getSoilSensorPotassium(void) {
  return soilSensorData.potassium;
}

void gsmInit(void) {
  // Initialize GSM module
  sendATCommand("AT", "", true);
  HAL_Delay(1000);
  sendATCommand("ATE0", "", true);  // Disable echo
  HAL_Delay(1000);
  
  // Configure GPRS connection
  sendATCommandRetry("cfun", "1,1", true);
  HAL_Delay(5000);
  
  // Configure bearer settings
  sendATCommandRetry("SAPBR", "3,1,\"Contype\",\"GPRS\"", true);
  HAL_Delay(1000);
  
  sendATCommandRetry("SAPBR", "3,1,\"APN\",\"telia\"", true);
  HAL_Delay(1000);
  
  sendATCommandRetry("SAPBR", "3,1,\"USER\",\"\"", true);
  HAL_Delay(1000);
  
  sendATCommandRetry("SAPBR", "3,1,\"PWD\",\"\"", true);
  HAL_Delay(1000);
  
  sendATCommandRetry("SAPBR", "1,1", true);
  HAL_Delay(2000);
  
  // Check bearer status
  sendATCommandRetry("SAPBR", "2,1", true);
  HAL_Delay(1000);
  
  // Configure MQTT settings
  sendATCommandRetry("SMCONF", "\"URL\",broker.emqx.io:1883", true);
  HAL_Delay(1000);
  
  sendATCommandRetry("SMCONF", "\"KEEPALIVE\",60", true);
  HAL_Delay(1000);
  
  sendATCommandRetry("SMCONF", "\"CLEANSS\",1", true);
  HAL_Delay(1000);
  
  // Connect to MQTT broker
  sendATCommandRetry("SMCONN", "", true);
  HAL_Delay(5000);
  
  gsmReady = true;
}

bool sendATCommand(const char *command, const char *param, bool addAT) {
  char p_command[64] = {0};
  
  if (addAT) {
    if (param[0] == '\0') {
      snprintf(p_command, sizeof(p_command), "AT + %s\r\n", command);
    } else {
      snprintf(p_command, sizeof(p_command), "AT + %s=%s\r\n", command, param);
    }
  } else {
    snprintf(p_command, sizeof(p_command), "%s\r\n", command);
  }
  
  HAL_UART_Transmit(&huart2, (uint8_t *)p_command, strlen(p_command), 1000);
  HAL_Delay(1000);
  
  return true;
}

bool sendATCommandRetry(const char *command, const char *param, bool addAT) {
  int retries = 3;
  bool result = false;
  
  while (retries > 0 && !result) {
    result = sendATCommand(command, param, addAT);
    if (!result) {
      HAL_Delay(1000);
      retries--;
    }
  }
  
  return result;
}

bool sendDirectAT(const char *command) {
  return sendATCommand(command, "", false);
}

bool sendRawAT(const char *command) {
  HAL_UART_Transmit(&huart2, (uint8_t *)command, strlen(command), 1000);
  HAL_Delay(1000);
  return true;
}

uint16_t GetTemperatureLevel(void) {
  uint16_t temperatureLevel = 0;

  temperatureLevel = (uint16_t)(SYS_GetTemperatureLevel() / 256);
  /* USER CODE BEGIN GetTemperatureLevel */

  /* USER CODE END GetTemperatureLevel */
  return temperatureLevel;
}

void Delay_CustomTimer(uint32_t delayMs) {
  timeStamp_timer = getTick_CustomTimer();
  while (1) {
    if (__HAL_TIM_GET_COUNTER(&htim2) - timeStamp_timer >= delayMs) {
      break;
    }
  }
}

uint32_t getTick_CustomTimer(void) { return __HAL_TIM_GET_COUNTER(&htim2); }

uint32_t getTick_CustomTimer_Sec(void) {
  return (__HAL_TIM_GET_COUNTER(&htim2) + 1) / 1000;
}

void initDelayCustomTimer(void) {
  timeStamp_timer = __HAL_TIM_GET_COUNTER(&htim2);
}

char *prepareLoraMessage(uint8_t sID, uint8_t sType) {
  char sIDString[SENSOR_ID_DIGIT_SIZE];
  char sTypeString[SENSOR_TYPE_DIGIT_SIZE];
  itoa(sID, sIDString, 10);
  itoa(sType, sTypeString, 10);

  snprintf(loraMessage_TX, sizeof(loraMessage_TX), "%s#%s#%s", IOT_GATEWAY_KEY,
           sIDString, sTypeString);
  return loraMessage_TX;
}