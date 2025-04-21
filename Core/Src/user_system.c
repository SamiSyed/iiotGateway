#include "user_system.h"
#include "adc_if.h"
#include "filter.h"
#include "main.h"
#include "stdio.h"
#include "tim.h"
#include "usart.h"
#include <stdlib.h>
#include <string.h>

static bool rawDataReceived = false;
static SystemError systemError = NO_ERROR;
static bool sendingData = false;
static bool receivingData = false;
/* This initially has to be true to fetch first AT Reply*/
static bool lastCommandOK = true;
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

/* GSM variables */

/* Flag */
void setRawDataReceived(bool status) { rawDataReceived = status; }

bool isRawDataReceived(void) { return rawDataReceived; }

void setSendingFlag(bool flag) { sendingData = flag; }
bool getSendingFlag(void) { return sendingData; }

void setReceivingFlag(bool flag) { receivingData = flag; }
bool getReceivingFlag(void) { return receivingData; }

void setLastCommandOK(bool status) { lastCommandOK = status; }

bool isLastCommandOK(void) { return lastCommandOK; }

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

SystemError gsmInit(void) {
  SystemError status = ERROR_NO_AT_REPLY;
  // Delay_CustomTimer(1000);
  // sendATCommand("ATE1", AT_OK, NO_AT);
  status = sendATCommand("ATZ", "", AT_OK, NO_AT);

  if (!isLastCommandOK()) {
    return status;
  }
  // Delay_CustomTimer(500);
  status = sendATCommandRetry("cfun", "1,1", AT_OK, ADD_AT);
  if (!lastCommandOK) {
    return status;
  }
  Delay_CustomTimer(15000);

  // sendATCommandRetry("cpin?", "", AT_OK, ADD_AT);
  // Delay_CustomTimer(500);
  // sendATCommandRetry("csq", "", AT_OK, ADD_AT);
  // sendATCommandRetry("creg?", "", AT_OK, ADD_AT);
  // sendATCommandRetry("cgatt?", "", AT_OK, ADD_AT);
  /* Kept mute due to reply size more than rx_buffer size */
  // sendATCommand("cgdcont?", AT_OK, ADD_AT);

  /* MQTT */
  // Delay_CustomTimer(500);

  status = sendATCommandRetry(AT_SAPBR, "3,1,\"Contype\",\"GPRS\"", AT_SAPBR,
                              ADD_AT);
  if (!lastCommandOK) {
    return status;
  }
  Delay_CustomTimer(50);
  status = sendATCommandRetry(AT_SAPBR, "3,1,\"APN\",\"telia\"", AT_OK, ADD_AT);
  if (!lastCommandOK) {
    return status;
  }
  Delay_CustomTimer(100);
  status = sendATCommandRetry(AT_SAPBR, "3,1,\"USER\",\"\"", AT_OK, ADD_AT);
  if (!lastCommandOK) {
    return status;
  }
  Delay_CustomTimer(100);
  status = sendATCommandRetry(AT_SAPBR, "3,1,\"PWD\",\"\"", AT_OK, ADD_AT);
  if (!lastCommandOK) {
    return status;
  }
  Delay_CustomTimer(100);
  status = sendATCommandRetry(AT_SAPBR, "1,1", AT_OK, ADD_AT);

  if (!lastCommandOK) {
    return status;
  }
  Delay_CustomTimer(100);
  // status = sendATCommand(AT_SAPBR, "2,1", AT_SAPBR, ADD_AT);
  status = sendATCommandRetry(AT_SAPBR, "2,1", AT_OK, ADD_AT);

  if (!lastCommandOK) {
    return status;
  }
  Delay_CustomTimer(2000);
  status = sendATCommandRetry(AT_SMCONF, "\"URL\",broker.emqx.io:1883", AT_OK,
                              ADD_AT);

  if (!lastCommandOK) {
    return status;
  }
  Delay_CustomTimer(500);
  status = sendATCommandRetry(AT_SMCONF, "\"KEEPALIVE\",60", "OK", ADD_AT);
  if (!lastCommandOK) {
    return status;
  }
  status = sendATCommandRetry(AT_SMCONF, "\"CLEANSS\",1", AT_OK, ADD_AT);
  if (!lastCommandOK) {
    return status;
  }
  status = sendATCommandRetry(AT_SMCONN, "", AT_OK, ADD_AT);
  Delay_CustomTimer(2000);

  return status;
}

SystemError sendATCommandRetry(char *command, char *param, char *reply,
                               bool addAT) {
  SystemError status = ERROR_LAST_COMMAND_FAILED;
  for (uint8_t i = 0; i < 5; i++) {
    status = sendATCommand(command, param, reply, addAT);
    if (status != NO_ERROR) {
      HAL_UART_Transmit(&huart2, (uint8_t *)"AT\r\n", 5, 100);
      setReceivingFlag(false);
      setLastCommandOK(true);
      cleanAllBuffers();

      Delay_CustomTimer(2000);
    } else {
      return NO_ERROR;
      // break;
    }
  }
  setLastCommandOK(false);

  return ERROR_NO_AT_REPLY;
}

void sendDirectAT(char *command, char *param) {
  snprintf(p_wifCommand, sizeof(p_wifCommand), "AT+%s%s\r\n", command, param);
  HAL_UART_Transmit(wifi_uart, (uint8_t *)p_wifCommand, strlen(p_wifCommand),
                    100);
}

void sendRawAT(char *command, char *param) {
  snprintf(p_wifCommand, sizeof(p_wifCommand), "AT+%s%s", command, param);
  HAL_UART_Transmit(wifi_uart, (uint8_t *)p_wifCommand, strlen(p_wifCommand),
                    100);
}

SystemError sendATCommand(char *command, char *param, char *reply, bool addAT) {
  // Delay_CustomTimer(2000);
  SystemError status = ERROR_LAST_COMMAND_FAILED;
  HAL_StatusTypeDef halStatus = HAL_ERROR;
  if (isLastCommandOK()) {
    status = ERROR_NO_AT_REPLY;
    lastCommandOK = false;

    if (addAT) {
      if (param == "") {
        snprintf(p_command, sizeof(p_command), "AT + %s\r\n", command);
      } else {
        snprintf(p_command, sizeof(p_command), "AT + %s=%s\r\n", command,
                 param);
      }
    } else {
      sprintf(p_command, "%s\r\n", command);
    }

    if (getReceivingFlag() == false) {
      setReceivingFlag(true);
      // printf("Sending : %s\r\n", p_command);
      // printf("strlen(p_command) : %i\r\n", strlen(p_command));
      halStatus = HAL_UART_Transmit_IT(gsm_uart, (uint8_t *)p_command,
                                       strlen(p_command));
      if (HAL_OK == halStatus) {
        for (uint8_t i = 0; i < 10; i++) {
          Delay_CustomTimer(50);
          if (!getReceivingFlag()) {
            if (replyContains(reply)) {
              if (replyContains("SMPUB") && replyContains("ERROR")) {
                status = ERROR_REPLY;
              } else {
                status = NO_ERROR;
              }
            } else {
              status = ERROR_REPLY;
            }
            break; /* If it gets reply it will break the loop. It does not
                      matter OK or ERROR */
          }
        }

        /* Print message according to status */
        if (status == ERROR_NO_AT_REPLY) {
          printf("No AT Reply : %s\r\n", p_command);
          setReceivingFlag(false);
          initUart();
        } else if (status == ERROR_REPLY) {
          printf("Error : %s\r\n", p_command);
        } else if (status == NO_ERROR) {
          setLastCommandOK(true);
          printf("OK : %s\r\n", p_command);
        } else {
          setReceivingFlag(false);
        }
      } else {
        printf("Error in Transmit : %s\r\n", p_command);
        status = ERROR_UART_TRANSMIT;
      }
    }
  }
  return status;
}

// void sendATWiFi(char *command, char *param, char *reply, bool addAT)
// {
//     HAL_StatusTypeDef halStatus = HAL_ERROR;
//     // char *pReply =

//     if (!waitingForRx)
//     {
//         snprintf(p_command, sizeof(p_command), "AT + %s=%s\r\n", command,
//         param); halStatus = HAL_UART_Transmit_IT(wifi_uart, (uint8_t
//         *)p_command, strlen(p_command)); if (HAL_OK == halStatus)
//         {
//             waitingForRx = true;
//         }
//         else
//         {
//             printf("Error in Transmit : %s\r\n", p_command);
//         }
//     }
//     else
//     {
//         if (dataArrived)
//         {
//             bufferContain(reply);
//         }
//     }
// }

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

char *prepareLoraMessage(uint8_t sID) {
  uint8_t sIDString[SENSOR_ID_DIGIT];
  itoa(sID, sIDString, 10);

  snprintf(loraMessage_TX, sizeof(loraMessage_TX), "%s#%s", IOT_GATEWAY_KEY,
           sIDString);
  return loraMessage_TX;
}